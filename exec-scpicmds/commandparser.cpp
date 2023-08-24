#include <QFile>
#include <QTextStream>
#include <QObject>
#include <memory>
#include <functional>
#include "commandparser.h"
#include "logging.h"
#include "inode.h"
#include "loopnode.h"
#include "breaknode.h"
#include "exitnode.h"
#include "ifnode.h"
#include "setnode.h"
#include "printnode.h"
#include "scpimsgnode.h"


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
                    QStringList fields = line.split(QRegExp("\\s+"));

                    if (fields[0].toUpper() == "VAR") // TODO check for not allowing keywords (also such that might get used in the future) as variale names
                    {
                        if (fields.size() - 1 == 3) { // 3 arguments{
                            // TODO check if 1st argument is a valid name and it does not exist yet
                            // TODO check if 2nd argument is one of valid strings (like INT, FLOAT, STRING)
                            // TODO check if 3rd argument?
                            if (fields[2].toUpper() == "INT")
                                gc.addVar(new Variable(fields[1], VariableType::INT, new int(fields[3].toInt())));
                            else if (fields[2].toUpper() == "FLOAT")
                                gc.addVar(new Variable(fields[1], VariableType::FLOAT, new float(fields[3].toFloat())));
                            else if (fields[2].toUpper() == "BOOL")
                                gc.addVar(new Variable(fields[1], VariableType::BOOL, new bool(fields[3].toInt() != 0)));
                            else if (fields[2].toUpper() == "STRING")
                                gc.addVar(new Variable(fields[1], VariableType::STRING, new QString(fields[3])));
                            else
                                ; // TODO print error message
                        }
                        else {
                            // TODO print error message
                        }
                    }
                    else if (fields[0].toUpper() == "SET") // TODO split up this whole block into smaller functions
                    {
                        if (fields.size() - 1 == 2) { // 2 arguments
                            // TODO check if 1st argument is a valid and existing variable name
                            // TODO check if 2nd argument can be interpreted to the variables corresponding type
                            bool ok = false;
                            Variable *lVar = nullptr;
                            if ((lVar = gc.getVar(fields[1])) != nullptr) { // Variable found
                                Variable *rVar = nullptr;
                                if ((rVar = gc.getVar(fields[2])) != nullptr) { // Variable found
                                    m_tree.append(new SetNode(m_tree.getCurrentContainer(), *lVar, *rVar));
                                }
                                else {
                                    Variable *constVal = nullptr;

                                    switch (lVar->getType()) {
                                    case INT: {
                                        int tmp = fields[2].toInt(&ok); // TODO all these checks here might get put into its own function. They are used in a similar way in the VAR command (see above)
                                        if (ok)
                                            constVal = new Variable("", VariableType::INT, new int(tmp));
                                            //lVar->setValue(new int(tmp));
                                        else
                                            ; // TODO print error message
                                        break;
                                    }
                                    case FLOAT: {
                                        float tmp = fields[2].toFloat(&ok);
                                        if (ok)
                                            constVal = new Variable("", VariableType::FLOAT, new float(tmp));
                                            //lVar->setValue(new float(tmp));
                                        break;
                                    }
                                    case BOOL: {
                                        if (fields[2].toUpper() == "TRUE") // TODO       toInt(&ok); // here and on other locations: don't use int for bools, but TRUE and FALSE string... (toUpper())
                                            constVal = new Variable("", VariableType::BOOL, new bool(true));
                                            //lVar->setValue(new bool(true));
                                        else if (fields[2].toUpper() == "FALSE")
                                            constVal = new Variable("", VariableType::BOOL, new bool(true));
                                            //lVar->setValue(new bool(false));
                                        else
                                            ; // TODO print error message
                                        break;
                                    }
                                    case STRING: {
                                        constVal = new Variable("", VariableType::STRING, new QString(fields[2]));
                                        //lVar->setValue(new QString(fields[2]));
                                        break;
                                    }
                                    }

                                    if (constVal != nullptr) {
                                        gc.addVar(constVal);
                                        m_tree.append(new SetNode(m_tree.getCurrentContainer(), *lVar, *constVal));
                                    }
                                }
                            }
                            else {
                                ; // TODO print error message (variable not found)
                            }

                            if (fields[2].toUpper() == "INT")
                                gc.addVar(new Variable(fields[1], VariableType::INT, new int(fields[3].toInt())));
                            else if (fields[2].toUpper() == "FLOAT")
                                gc.addVar(new Variable(fields[1], VariableType::FLOAT, new float(fields[3].toFloat())));
                            else if (fields[2].toUpper() == "BOOL")
                                gc.addVar(new Variable(fields[1], VariableType::BOOL, new bool(fields[3].toInt() != 0)));
                            else if (fields[2].toUpper() == "STRING")
                                gc.addVar(new Variable(fields[1], VariableType::STRING, new QString(fields[3])));
                            else
                                ; // TODO print error message
                        }
                        else {
                            // TODO print error message (wrong number of arguments)
                        }
                    }
                    else if (fields[0].toUpper() == "LOOP")
                    {
                        if (fields.size() - 1 == 0) // 0 arguments
                        {
                            // Endless loop (only makes sense when BREAK is implemented
                        }
                        else if (fields.size() - 1 == 1) // 1 argument
                        {
                            m_tree.enterContainer(new LoopNode(m_tree.getCurrentContainer(), fields[1].toUInt()));
                            ctrTypes.push(ContainerType::LOOP);
                        }
                    }
                    else if (fields[0].toUpper() == "BREAK")
                    {
                        // TODO check if there are nodes in the BREAK's parent tree as they are unreachable
                        if (fields.size() - 1 == 0) // 0 arguments
                        {
                            m_tree.append(new BreakNode(m_tree.getCurrentContainer()));
                        }
                    }
                    else if (fields[0].toUpper() == "EXIT")
                    {
                        if (fields.size() - 1 == 0) // 0 arguments
                        {
                            m_tree.append(new ExitNode(m_tree.getCurrentContainer()));
                        }
                    }
                    else if (fields[0].toUpper() == "PRINT")
                    {
                        if (fields.size() - 1 >= 1) // >= 1 argument(s)
                        {
                            std::vector<Variable*> *values = new std::vector<Variable*>;
                            Variable *var = nullptr;
                            for (int f = 1; f < fields.size(); f++) {
                                if ((var = gc.getVar(fields[f])) != nullptr) { // Variable found
                                    values->push_back(var);
                                }
                                else {
                                    var = new Variable("", VariableType::STRING, new QString(fields[f]));
                                    gc.addVar(var);
                                    values->push_back(var);
                                }
                            }
                            m_tree.append(new PrintNode(m_tree.getCurrentContainer(), values));
                        }
                    }
                    else if (fields[0].toUpper() == "IF")
                    {
                        if (fields.size() - 1 == 1) // 1 argument
                        {
                            IfNode *ifnode = new IfNode(m_tree.getCurrentContainer(),fields[1].toInt() != 0);
                            m_tree.enterContainer(ifnode);
                            ifnodes.push(ifnode);
                            ctrTypes.push(ContainerType::IF);
                        }
                        else
                        {
                            // TODO print error message
                        }
                    }
                    else if (fields[0].toUpper() == "ELSE")
                    {
                        ifnodes.top()->switchToElseBranch();
                    }
                    else if (fields[0].toUpper() == "END")
                    {
                        m_tree.leaveContainer();
                        if (ctrTypes.top() == ContainerType::IF)
                            ifnodes.pop();
                        ctrTypes.pop();
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
