#include <QFile>
#include <QTextStream>
#include <QObject>
#include <memory>
#include <functional>
#include <string>
#include "commandparser.h"
#include "logging.h"
#include "inode.h"
#include "loopnode.h"
#include "breaknode.h"
#include "exitnode.h"
#include "ifnode.h"
#include "setnode.h"
#include "addnode.h"
#include "printnode.h"
#include "scpimsgnode.h"
#include "condition.h"


CommandParser::CommandParser(TcpHandler &tcpHandler) :
    m_tcpHandler(tcpHandler), m_tree(nullptr)
{
}

void CommandParser::setHandleErroneousMessages(unsigned int handleErroneousMessages)
{
    m_handleErroneousMessages = handleErroneousMessages;
}

void CommandParser::setCheckErrorQueue(bool checkErrorQueue)
{
    m_checkErrorQueue = checkErrorQueue;
}

void CommandParser::setLoopNumber(quint8 numLoops)
{
    m_numLoops = numLoops;
}

void CommandParser::parseCmdFile(QString strFileName)
{
    enum class ContainerType
    {
        LOOP = 0,
        IF,
    };

    QFile cmdFile(strFileName);
    std::stack<IfNode *> ifnodes;
    std::stack<ContainerType> ctrTypes;

    if(cmdFile.open(QIODevice::ReadOnly)) {
        // Read lines and split into single commands (per line)
        QTextStream textStream(&cmdFile);
        int fileLineNumber = 0;

        // Iterate over all lines (messages) in file
        while (!textStream.atEnd()) {
            QString line = textStream.readLine().trimmed();
            fileLineNumber++;
            if(!line.isEmpty() && !line.startsWith("#")) {
                if (line.startsWith(">"))
                {
                    line = line.remove(0, 1).trimmed();
                    QString tmpLine = line.replace("\\\"", "\1"); // we use \1 as placeholder, as it unlikely that this in part of the string (we just don't allow it!). \0 didn't work, as it terminates the string.
                    int quoteCount = tmpLine.count("\"");
                    if (quoteCount % 2 == 0) {
                        // Split line while taking care of strings (in quotes)
                        QStringList fields;
                        qsizetype lastIdx = 0;
                        qsizetype prevIdx = 0;
                        quoteCount = 0;

                        QStringList parts = tmpLine.split("\"");
                        for (int i = 0; i < parts.count(); i++) {
                            if ((i % 2) == 0) { // keywords, variables
                                for(auto &part : parts[i].split(QRegExp("\\s+"), Qt::SkipEmptyParts)) {
                                    fields.append(part.trimmed());
                                }
                            }
                            else { // strings
                                fields.append(parts[i].replace("\1", "\""));
                            }
                        }

                        // The commands...
                        if (fields[0].toUpper() == "VAR") // TODO check for not allowing keywords (also such that might get used in the future) as variale names and also check the variable name is of regex [0-9A-Za-z_]+
                        {
                            if (fields.size() - 1 == 3) { // 3 arguments
                                // TODO check if 1st argument is a valid name and it does not exist yet
                                // TODO check if 2nd argument is one of valid strings (like INT, FLOAT, STRING)
                                // TODO check if 3rd argument?
                                if (fields[2].toUpper() == "INT")
                                    gc.addVar(new Variable(fields[1].toStdString(), VariableType::INT, new int(fields[3].toInt())));
                                else if (fields[2].toUpper() == "FLOAT")
                                    gc.addVar(new Variable(fields[1].toStdString(), VariableType::FLOAT, new float(fields[3].toFloat())));
                                else if (fields[2].toUpper() == "BOOL")
                                    gc.addVar(new Variable(fields[1].toStdString(), VariableType::BOOL, new bool(fields[3].toUpper() == "TRUE")));
                                else if (fields[2].toUpper() == "STRING")
                                    gc.addVar(new Variable(fields[1].toStdString(), VariableType::STRING, new QString(fields[3])));
                                else {
                                    Logging::logMsg(QString("[L%1] VAR statement variable type in 3rd argument is not of {INT, FLOAT, BOOL, STRING}. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                    exit(1);
                                }
                            }
                            else {
                                Logging::logMsg(QString("[L%1] VAR statement has invalid number of arguments. Needs to be 1 or 3. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                        else if (fields[0].toUpper() == "SET") // TODO split up this whole block into smaller functions
                        {
                            if (fields.size() - 1 == 2) { // 2 arguments
                                bool ok = false;
                                Variable *lVal = nullptr;
                                if ((lVal = gc.getVar(fields[1].toStdString())) != nullptr) { // Variable found
                                    Variable *rVal = nullptr;
                                    if ((rVal = gc.getVar(fields[2].toStdString())) != nullptr) { // Variable found
                                        m_tree.append(new SetNode(m_tree.getCurrentContainer(), *lVal, *rVal));
                                    }
                                    else {
                                        Variable *constVal = nullptr;

                                        switch (lVal->getType()) {
                                        case INT: {
                                            int tmp = fields[2].toInt(&ok); // TODO all these checks here might get put into its own function. They are used in a similar way in the VAR command (see above).
                                            if (ok)
                                                constVal = new Variable("", VariableType::INT, new int(tmp));
                                            else
                                                ; // TODO print error message
                                            break;
                                        }
                                        case FLOAT: {
                                            float tmp = fields[2].toFloat(&ok);
                                            if (ok)
                                                constVal = new Variable("", VariableType::FLOAT, new float(tmp));
                                            break;
                                        }
                                        case BOOL: {
                                            if (fields[2].toUpper() == "TRUE")
                                                constVal = new Variable("", VariableType::BOOL, new bool(true));
                                            else if (fields[2].toUpper() == "FALSE")
                                                constVal = new Variable("", VariableType::BOOL, new bool(true));
                                            else {
                                                Logging::logMsg(QString("[L%1] SET statement value in 2nd argument is not of {TRUE, FALSE}. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                                exit(1);
                                            }
                                            break;
                                        }
                                        case STRING: {
                                            constVal = new Variable("", VariableType::STRING, new std::string(fields[2].toStdString()));
                                            break;
                                        }
                                        }

                                        if (constVal != nullptr) {
                                            gc.addVar(constVal);
                                            m_tree.append(new SetNode(m_tree.getCurrentContainer(), *lVal, *constVal));
                                        }
                                    }
                                }
                                else {
                                    ; // TODO print error message (variable not found)
                                }

                                if (fields[2].toUpper() == "INT")
                                    gc.addVar(new Variable(fields[1].toStdString(), VariableType::INT, new int(fields[3].toInt())));
                                else if (fields[2].toUpper() == "FLOAT")
                                    gc.addVar(new Variable(fields[1].toStdString(), VariableType::FLOAT, new float(fields[3].toFloat())));
                                else if (fields[2].toUpper() == "BOOL")
                                    gc.addVar(new Variable(fields[1].toStdString(), VariableType::BOOL, new bool(fields[3].toInt() != 0)));
                                else if (fields[2].toUpper() == "STRING")
                                    gc.addVar(new Variable(fields[1].toStdString(), VariableType::STRING, new std::string(fields[3].toStdString())));
                                else
                                    ; // TODO print error message
                            }
                            else {
                                Logging::logMsg(QString("[L%1] SET statement has invalid number of arguments. Needs to be 2. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                        else if (fields[0].toUpper() == "ADD")
                        {
                            if (fields.size() - 1 == 2) // 2 arguments
                            {
                                bool ok = false;
                                Variable *lVal = nullptr;
                                if ((lVal = gc.getVar(fields[1].toStdString())) != nullptr) { // Variable found

                                    // TODO the whole block is almost the same as in the SET command. Put it into a separate function?
                                    Variable *rVal = nullptr;
                                    if ((rVal = gc.getVar(fields[2].toStdString())) == nullptr) { // Variable not found
                                        switch (lVal->getType()) {
                                        case INT: {
                                            int tmp = fields[2].toInt(&ok); // TODO all these checks here might get put into its own function. They are used in a similar way in the VAR command (see above)
                                            if (ok)
                                                rVal = new Variable("", VariableType::INT, new int(tmp));
                                            else
                                                ; // TODO print error message
                                            break;
                                        }
                                        case FLOAT: {
                                            float tmp = fields[2].toFloat(&ok);
                                            if (ok)
                                                rVal = new Variable("", VariableType::FLOAT, new float(tmp));
                                            break;
                                        }
                                        case BOOL: {
                                            ; // TODO print error message (cannot use add on boolean value
                                            break;
                                        }
                                        case STRING: {
                                            rVal = new Variable("", VariableType::STRING, new std::string(fields[2].toStdString()));
                                            break;
                                        }
                                        }

                                        if (rVal != nullptr) {
                                            gc.addVar(rVal);
                                            m_tree.append(new AddNode(m_tree.getCurrentContainer(), *lVal, *rVal));
                                        }
                                        else {
                                             // TODO print error message (lValue and rValue not of same type)
                                        }
                                    }
                                }
                                else {
                                    ; // TODO print error message variable not found
                                }
                            }
                            else
                            {
                                Logging::logMsg(QString("[L%1] ADD statement has invalid number of arguments. Needs to be 2. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                        else if (fields[0].toUpper() == "LOOP")
                        {
                            if (fields.size() - 1 == 0) // 0 arguments
                            {
                                // TODO implement and document: Endless loop (only makes sense when BREAK is used)
                            }
                            else if (fields.size() - 1 == 1) // 1 argument
                            {

                                Variable *var = nullptr;
                                if ((var = gc.getVar(fields[1].toStdString())) == nullptr) { // Variable not found
                                    var = new Variable("", VariableType::INT, new int(fields[1].toUInt())); // TODO check for toUInt(ok)
                                }

                                if (var != nullptr) {
                                    gc.addVar(var);
                                    m_tree.enterContainer(new LoopNode(m_tree.getCurrentContainer(), *var));
                                    ctrTypes.push(ContainerType::LOOP);
                                }
                            }
                            else
                            {
                                Logging::logMsg(QString("[L%1] LOOP statement has invalid number of arguments. Needs to be 0 or 1. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                        else if (fields[0].toUpper() == "BREAK")
                        {
                            if (fields.size() - 1 == 0) // 0 arguments
                            {
                                m_tree.append(new BreakNode(m_tree.getCurrentContainer()));
                            }
                            else
                            {
                                Logging::logMsg(QString("[L%1] BREAK statement does not expect any arguments. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                        else if (fields[0].toUpper() == "EXIT")
                        {
                            if (fields.size() - 1 == 0) // 0 arguments
                            {
                                m_tree.append(new ExitNode(m_tree.getCurrentContainer()));
                            }
                            else
                            {
                                Logging::logMsg(QString("[L%1] EXIT statement does not expect any arguments. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                        else if (fields[0].toUpper() == "PRINT")
                        {
                            if (fields.size() - 1 >= 1) // >= 1 argument(s)
                            {
                                std::vector<Variable*> *values = new std::vector<Variable*>;
                                Variable *var = nullptr;
                                for (int f = 1; f < fields.size(); f++) {
                                    if ((var = gc.getVar(fields[f].toStdString())) != nullptr) { // Variable found
                                        values->push_back(var);
                                    }
                                    else {
                                        var = new Variable("", VariableType::STRING, new QString(fields[f]));
                                        gc.addVar(var);
                                        values->push_back(var);
                                    }
                                }
                                std::function<void(std::string&)> cbLog = [&](std::string &str){Logging::logMsg(QString::fromStdString(str)); };
                                m_tree.append(new PrintNode(m_tree.getCurrentContainer(), values, cbLog));
                            }
                            else
                            {
                                Logging::logMsg(QString("[L%1] PRINT statement has invalid number of arguments. Needs to be >=1. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                        else if (fields[0].toUpper() == "IF")
                        {
                            if (fields.size() - 1 == 1) // 1 argument
                            {
                                QString varName = fields[1];
                                Variable *var = nullptr;
                                if (varName.toUpper() == "TRUE" || varName.toUpper() == "FALSE") {
                                    var = new Variable("", VariableType::BOOL, new bool(varName.toUpper() == "TRUE"));
                                }
                                else if ((var = gc.getVar(varName.toStdString())) != nullptr) {
                                    ; // Do nothing more, as var already got found (in the if-statement above)
                                }
                                else {
                                    Logging::logMsg(QString("[L%1] IF statement has invalid arguments. Needs to be TRUE, FALSE or an existing variable. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                    exit(1);
                                }

                                if (var != nullptr) {
                                    gc.addVar(var);
                                    IfNode *ifnode = new IfNode(m_tree.getCurrentContainer(), *(new BoolCondition(*var)));
                                    m_tree.enterContainer(ifnode);
                                    ifnodes.push(ifnode);
                                    ctrTypes.push(ContainerType::IF);
                                }
                            }
                            else if (fields.size() - 1 == 3) // 3 arguments
                            {
                                QString varName;

                                // Check 1st parameter
                                Variable *lVal = nullptr;
                                varName = fields[1];
                                if ((lVal = gc.getVar(varName.toStdString())) == nullptr) {
                                    Logging::logMsg(QString("[L%1] IF statement has invalid 1st argument. Needs to be an existing variable. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                    exit(1);
                                }

                                // Check 3rd parameter
                                Variable *rVal = nullptr; // TODO move this variable parsing into separate function
                                varName = fields[3];
                                if ((rVal = gc.getVar(varName.toStdString())) != nullptr) {
                                    if (lVal->getType() != rVal->getType()) {
                                        Logging::logMsg(QString("[L%1] IF statement variable in 3rd argument is not of same type as variable in 1st argument. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                        exit(1);
                                    }
                                }
                                else {
                                    bool ok;
                                    switch (lVal->getType()) {
                                    case INT: {
                                        int tmp = fields[3].toInt(&ok); // TODO all these checks here might get put into its own function. They are used in a similar way in the VAR command (see above)
                                        if (ok)
                                            rVal = new Variable("", VariableType::INT, new int(tmp));
                                        else
                                            ; // TODO print error message
                                        break;
                                    }
                                    case FLOAT: {
                                        float tmp = fields[3].toFloat(&ok);
                                        if (ok)
                                            rVal = new Variable("", VariableType::FLOAT, new float(tmp));
                                        break;
                                    }
                                    case BOOL: {
                                        if (fields[3].toUpper() == "TRUE" || fields[3].toUpper() == "FALSE") {
                                            rVal = new Variable("", VariableType::BOOL, new bool(fields[3].toUpper() == "TRUE"));
                                        break;
                                    }
                                    case STRING: {
                                        rVal = new Variable("", VariableType::STRING, new std::string(fields[3].toStdString()));
                                        break;
                                    }
                                    }

                                    if (rVal != nullptr) {
                                        gc.addVar(rVal);
                                        m_tree.append(new AddNode(m_tree.getCurrentContainer(), *lVal, *rVal));
                                    }
                                    else {
                                        Logging::logMsg(QString("[L%1] IF statement value in 3rd argument is not interpretable to same type as variable in 1st argument. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                        exit(1);
                                    }
                                    }
                                }

                                // Check 2nd parameter
                                ComparisonType compType;
                                if (ComparisonCondition::getComparisonTypeFromString(fields[2].trimmed().toStdString(), compType)) {
                                    bool errorFound = false;
                                    switch (lVal->getType()) {
                                    case INT:
                                        ; // Do nothing as all comparisons are valid
                                        break;
                                    case FLOAT:
                                        ; // Do nothing as all comparisons are valid
                                        break;
                                    case BOOL:
                                        if (!(compType == ComparisonType::EQ || compType == ComparisonType::NE))
                                            errorFound = true;
                                        break;
                                    case STRING:
                                        if (!(compType == ComparisonType::EQ || compType == ComparisonType::NE))
                                            errorFound = true;
                                        break;
                                    }

                                    if(errorFound) {
                                        // TODO add strings in the following message to show of which type each variable is.
                                        Logging::logMsg(QString("[L%1] IF statement comparison rule in 2nd argument is not compitible with the type of the variable in 1st argument. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                        exit(1);
                                    }

                                }
                                else {
                                    Logging::logMsg(QString("[L%1] IF statement comparison rule in 2nd argument is not of {LT, GT, LE, GE, EQ, NE}. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                    exit(1);
                                }

                                gc.addVar(lVal);
                                IfNode *ifnode = new IfNode(m_tree.getCurrentContainer(), *(new ComparisonCondition(*lVal, *rVal, compType)));
                                m_tree.enterContainer(ifnode);
                                ifnodes.push(ifnode);
                                ctrTypes.push(ContainerType::IF);
                            }
                            else
                            {
                                Logging::logMsg(QString("[L%1] IF statement has invalid number of arguments. Needs to be 1 or 3. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                        else if (fields[0].toUpper() == "ELSE")
                        {
                            if (fields.size() - 1 == 0) // 0 arguments
                            {
                                ifnodes.top()->switchToElseBranch();
                            }
                            else
                            {
                                Logging::logMsg(QString("[L%1] ELSE statement does not expect any arguments. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                        else if (fields[0].toUpper() == "END")
                        {
                            if (fields.size() - 1 == 0) // 0 arguments
                            {                            m_tree.leaveContainer();
                                if (ctrTypes.top() == ContainerType::IF)
                                    ifnodes.pop();
                                ctrTypes.pop();
                            }
                            else
                            {
                                Logging::logMsg(QString("[L%1] END statement does not expect any arguments. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                                exit(1);
                            }
                        }
                    }
                    else {
                        Logging::logMsg(QString("[L%1] No even number of quotes (\") detected. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                        exit(1);
                    }
                }
                else
                {
                    QString& msg = line; // Just to make clear, this line is a message
                    qsizetype lastCmdPos = 0;

                    auto msgData = std::make_shared<MessageData>();
                    msgData->m_oriMsg = msg;
                    msgData->m_fileLineNumber = fileLineNumber;

                    // Iterate over all message parts (commands) in message
                    for(auto &cmd : msg.split("|")) {
                        auto cmdData = std::make_shared<CommandData>();
                        cmdData->m_cmd = cmd;
                        cmdData->m_cmdTrimmed = cmd.trimmed();
                        cmdData->m_cmdType = CommandData::determineCommandType(cmd);
                        cmdData->m_posInMsg = lastCmdPos + 1;
                        lastCmdPos = msg.indexOf("|", lastCmdPos) + 1;
                        cmdData->m_testRule = ""; // TODO implement (as a list of rules?)
                        msgData->m_cmds.append(cmdData);
                    }
                    msgData->m_cmdCountStrWidth = QString::number(msgData->m_cmds.count()).length();
                    m_tree.append(new ScpiMsgNode(m_tree.getCurrentContainer(), msgData));
                }
            }
        }
        cmdFile.close();

        // Check messages
        Logging::logMsg(QString("Checking messages..."));
        bool msgsValid = msgsAreValid();
        if (msgsValid)
            Logging::logMsg(QString("... All messages look fine."), LoggingColor::GREEN);
        else
            Logging::logMsg(QString("... Invalid messages(s) found!"), LoggingColor::RED);

        switch (m_handleErroneousMessages)
        {
        case 0:
        default:
            if (!msgsValid)
                Logging::logMsg(QString("... -> No messages will get executed."), LoggingColor::YELLOW);
            else
                loopAndSendMsgs();
            break;

        case 1:
            if (!msgsValid)
                Logging::logMsg(QString("... -> Trying to execute them anyway."), LoggingColor::YELLOW);
            loopAndSendMsgs();
            break;

        case 2:
            if (!msgsValid) {
                Logging::logMsg(QString("... -> Removing invalid messages and executing the rest."), LoggingColor::YELLOW);
                removeInvalidMsgs(false);
            }
            loopAndSendMsgs();
            break;

        case 3:
            if (!msgsValid) {
                Logging::logMsg(QString("... -> Removing invalid messages (silently) and executing the rest."), LoggingColor::YELLOW);
                removeInvalidMsgs(true);
            }
            loopAndSendMsgs();
            break;
        }
    }
    else
    {
        Logging::logMsg(QString("Command file \"%1\" could not be opened!").arg(strFileName), LoggingColor::RED);
    }

    // Cleanup
    m_tree.clear();
}

bool CommandParser::msgsAreValid()
{
    bool msgsAreValid = true;

    for (auto &it : *m_tree.getRoot()) {
        std::shared_ptr<MessageData> msg = dynamic_cast<ScpiMsgNode&>(it).getMsgData();

        msg->m_isValid = true;
        for (auto &cmd : msg->m_cmds) {
            if (cmd->m_cmdType == CommandType::EMPTY)
            {
                Logging::logMsg(QString(" > Empty command found in line %1:%2!").arg(QString::number(msg->m_fileLineNumber), QString::number(cmd->m_posInMsg)), LoggingColor::RED);
                msg->m_isValid = false;
                msgsAreValid = false;
            }
            else if (cmd->m_cmdType == CommandType::UNKNOWN)
            {
                Logging::logMsg(QString(" > Command \"%1\" of unknown type found in line %2:%3! Is there a missing \";\"?").arg(cmd->m_cmdTrimmed, QString::number(msg->m_fileLineNumber), QString::number(cmd->m_posInMsg)), LoggingColor::RED);
                msg->m_isValid = false;
                msgsAreValid = false;
            }
            // Check for ASCII characters and allowed symboles only
            //bool containsNonASCII = myString.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]"))); // a-zA-z0-9.:,;*?-+[[:blank:]] (even more symbols?)
        }
    }

    return msgsAreValid;
}

void CommandParser::sendMsgs(unsigned int iterNo)
{
    if (m_tree.hasLeaves())
    {
        if (m_numLoops == 1)
            Logging::logMsg(QString("Sending messages..."), LoggingColor::BLUE);
        else
            Logging::logMsg(QString("Sending messages (#%1/%2)...").arg(iterNo + 1).arg(m_numLoops), LoggingColor::BLUE);

        // Initially clear target's error queue
        if (m_checkErrorQueue) {
            m_tcpHandler.sendMessageRaw("SYSTEM:ERROR:ALL?\n");
            QStringList answers = m_tcpHandler.receiveAnswersRaw(1); // Ignore result, just for sync
        }

        std::function<void (INode *)> f = [this] (INode *node) {
            ScpiMsgNode *n = dynamic_cast<ScpiMsgNode*>(node);
            m_tcpHandler.sendMessage(*n->getMsgData());

            if (m_checkErrorQueue) {
                // Check target's error queue after current message // TODO Check what happens if in the cmd file itself there's a system:error:*? command?
                m_tcpHandler.sendMessageRaw("SYSTEM:ERROR:ALL?\n");
                QStringList answers = m_tcpHandler.receiveAnswersRaw(1);
                if (answers.length() == 1 && answers[0] != "+0,No error")
                    Logging::logMsg(QString("Message produced error(s) \"%1\".").arg(answers[0]), LoggingColor::RED);
            }
        };

        m_tree.exec(f);
    }
    else
    {
        Logging::logMsg(QString("No messages to send."), LoggingColor::BLUE);
    }
}

void CommandParser::loopAndSendMsgs()
{
    if(m_numLoops == 0)
        Logging::logMsg(QString("The file will be executed 0 times (option -l 0)!").arg(m_numLoops), LoggingColor::GREEN);
    else if(m_numLoops > 1)
        Logging::logMsg(QString("The file will be executed %1 times.").arg(m_numLoops), LoggingColor::GREEN);

    for (unsigned int curIter = 0; curIter < m_numLoops; curIter++)
        sendMsgs(curIter);
}

void CommandParser::removeInvalidMsgs(bool silent)
{
    CtrNode &root = *m_tree.getRoot();
    for (auto &it : root) {
        std::shared_ptr<MessageData> msg = dynamic_cast<ScpiMsgNode&>(it).getMsgData();
        if (!msg->m_isValid) {
            if (!silent)
                Logging::logMsg(QString(" Remove invalid message [L%1] \"%2\"").arg(QString::number(msg->m_fileLineNumber).rightJustified(2, '0'), msg->m_oriMsg), LoggingColor::YELLOW);
            root.remove(&it);
        }
    }

    root.prune();
}
