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

void CommandParser::setIgnoreExistingVariables(bool ignoreExistingVariables)
{
    m_ignoreExistingVariables = ignoreExistingVariables;
}


void CommandParser::parseCmdFile(QString strFileName)
{
    QFile cmdFile(strFileName);
    std::stack<IfNode*> ifNodes;
    std::stack<ContainerType> ctrTypes;
    std::vector<ICondition*> conds;
    int lastElseNodeLineNumber = 0;

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
                        bool ok = false;
                        quoteCount = 0;

                        QStringList parts = tmpLine.split("\"");
                        for (int i = 0; i < parts.count(); i++) {
                            if ((i % 2) == 0) { // keywords, variables
                                for(auto &part : parts[i].split(QRegExp("\\s+"), Qt::SkipEmptyParts))
                                    fields.append(part.trimmed());
                            }
                            else { // strings
                                fields.append(parts[i].replace("\1", "\""));
                            }
                        }

                        if (fields.size() == 0) {
                            Logging::logMsg(QString("[L%1] Command line with no statement. Exit program.").arg(QString::number(fileLineNumber)), LoggingColor::RED);
                            exit(1);
                        }
                        QString stmtUpper= fields[0].toUpper();

                        // Handle keywords
                        CommandParserContext cpc(fields, ifNodes, ctrTypes, conds, fileLineNumber, lastElseNodeLineNumber);
                        if (stmtUpper == "VAR") {
                            parseVarStatement(cpc);
                        } else if (stmtUpper == "SET") {
                            parseSetStatement(cpc);
                        } else if (stmtUpper == "ADD") {
                            parseAddStatement(cpc);
                        } else if (stmtUpper == "LOOP") {
                            parseLoopStatement(cpc);
                        } else if (stmtUpper == "BREAK") {
                            parseBreakStatement(cpc);
                        } else if (stmtUpper == "EXIT") {
                            parseExitStatement(cpc);
                        } else if (stmtUpper == "PRINT") {
                            parsePrintStatement(cpc);
                        } else if (stmtUpper == "IF") {
                            parseIfStatement(cpc);
                        } else if (stmtUpper == "ELSE") {
                            parseElseStatement(cpc);
                        } else if (stmtUpper == "END") {
                            parseEndStatement(cpc);
                        } else {
                            Logging::logMsg(QString("[L%1] The first token (%2) is not a valid keyword. Exit program.").arg(QString::number(fileLineNumber), fields[0]), LoggingColor::RED);
                            exit(1);
                        }
                    } else {
                        Logging::logMsg(QString("[L%1] No even number of quotes (\") detected. Exit program.").arg(fileLineNumber), LoggingColor::RED);
                        exit(1);
                    }
                } else {
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
    gc.clear();
    for (auto cond : conds)
        delete cond;
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

void CommandParser::parseVarStatement(CommandParserContext &cpc)
{
    if (cpc.fields.size() - 1 == 3) { // 3 arguments; format = VAR <VAR_NAME> <VAR_TYPE> <VALUE>
        if (!Variable::strIsKeyword(cpc.fields[1].toStdString())) {
            if (Variable::varNameIsValid(cpc.fields[1].toStdString())) {
                if (!gc.varExists(cpc.fields[1].toStdString())) {
                    VariableType type;
                    if (Variable::strToVarType(cpc.fields[2].toStdString(), type)) {
                        if (!Variable::strIsKeyword(cpc.fields[1].toStdString())) {
                            Variable *tmp = Variable::parseToVar(cpc.fields[1].toStdString(), cpc.fields[3].toStdString(), type);
                            if (tmp != nullptr) {
                                   gc.addVar(tmp); // No need to check the return value, as we already check before, if the variable already exists
                            } else {
                                Logging::logMsg(QString("[L%1] VAR statement value in 3rd argument (%2) is not of type in 2nd argument (%3). Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[3], cpc.fields[2]), LoggingColor::RED);
                                exit(1);
                            }
                        } else {
                            Logging::logMsg(QString("[L%1] VAR statement variable name in 1st argument (%2) is a reserved keyword and therefore not valid. Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[1], cpc.fields[2]), LoggingColor::RED);
                            exit(1);
                        }
                    } else {
                        Logging::logMsg(QString("[L%1] VAR statement variable type in 2nd argument (%2) is not of {INT, FLOAT, BOOL, STRING}. Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[2]), LoggingColor::RED);
                        exit(1);
                    }
                } else {
                    if (!m_ignoreExistingVariables) {
                        Logging::logMsg(QString("[L%1] VAR statement variable name in 1st argument (%2) already exists. Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[1]), LoggingColor::RED);
                        exit(1);
                    }
                }
            } else {
                Logging::logMsg(QString("[L%1] VAR statement variable name in 1st argument (%2) is invalid. Needs to be of (regex) format \"[0-9A-Za-z_]+\". Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[1]), LoggingColor::RED);
                exit(1);
            }
        } else {
            Logging::logMsg(QString("[L%1] VAR statement variable name in 1st argument (%2) cannot be a reserved keyword. Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[1]), LoggingColor::RED);
            exit(1);
        }
    } else {
        Logging::logMsg(QString("[L%1] VAR statement has invalid number of arguments (%2). Needs to be 3. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.fields.size() - 1)), LoggingColor::RED);
        exit(1);
    }
}

void CommandParser::parseSetStatement(CommandParserContext &cpc)
{
    if (cpc.fields.size() - 1 == 2) { // 2 arguments; format = SET <LEFT_VAR_NAME> <RIGHT_VALUE>
        Variable *lVal = nullptr;
        if ((lVal = gc.getVar(cpc.fields[1].toStdString())) != nullptr) { // Variable found
            Variable *rVal = nullptr;
            if ((rVal = gc.getVar(cpc.fields[2].toStdString())) == nullptr) { // Variable found
                rVal = Variable::parseToVar("", cpc.fields[2].toStdString(), lVal->getType());
                if (rVal != nullptr) {
                    gc.addVar(rVal);
                    m_tree.append(new SetNode(m_tree.getCurrentContainer(), *lVal, *rVal));
                } else {
                    Logging::logMsg(QString("[L%1] SET statement value in 2nd argument (%2) does not match the type of variable in 1st argument. Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[2]), LoggingColor::RED);
                    exit(1);
                }
            }
        } else {
            Logging::logMsg(QString("[L%1] SET statement variable name in 1st argument (%2) is not defined. Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[1]), LoggingColor::RED);
            exit(1);
        }
    } else {
        Logging::logMsg(QString("[L%1] SET statement has invalid number of arguments (%2). Needs to be 2. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.fields.size() - 1)), LoggingColor::RED);
        exit(1);
    }
}

void CommandParser::parseAddStatement(CommandParserContext &cpc)
{
    if (cpc.fields.size() - 1 == 2) { // 2 arguments; format = ADD <LEFT_VAR_NAME> <RIGHT_VALUE>
        Variable *lVal = nullptr;
        if ((lVal = gc.getVar(cpc.fields[1].toStdString())) != nullptr) { // Variable found
            if (lVal->getType() != VariableType::BOOL) { // ADD is not defined for BOOL
                Variable *rVal = nullptr;
                if ((rVal = gc.getVar(cpc.fields[2].toStdString())) == nullptr) { // Variable not found
                    rVal = Variable::parseToVar("", cpc.fields[2].toStdString(), lVal->getType());
                    if (rVal != nullptr) {
                        gc.addVar(rVal);
                        m_tree.append(new AddNode(m_tree.getCurrentContainer(), *lVal, *rVal));
                    } else {
                        Logging::logMsg(QString("[L%1] ADD statement value in 2nd argument does not match the type of variable in 1st argument. Exit program.").arg(cpc.fileLineNumber), LoggingColor::RED);
                        exit(1);
                    }
                }
            } else {
                Logging::logMsg(QString("[L%1] ADD statement is not defined for BOOL variables. Exit program.").arg(cpc.fileLineNumber), LoggingColor::RED);
                exit(1);
            }
        } else {
            Logging::logMsg(QString("[L%1] ADD statement variable name in 1st argument (%2) is not defined. Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[1]), LoggingColor::RED);
            exit(1);
        }
    } else {
        Logging::logMsg(QString("[L%1] ADD statement has invalid number of arguments (%2). Needs to be 2. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.fields.size() - 1)), LoggingColor::RED);
        exit(1);
    }
}

void CommandParser::parseLoopStatement(CommandParserContext &cpc)
{
    if (cpc.fields.size() - 1 == 0) { // 0 arguments
        m_tree.enterContainer(new LoopNode(m_tree.getCurrentContainer()));
        cpc.ctrTypes.push(ContainerType::LOOP);
    } else if (cpc.fields.size() - 1 == 1) { // 1 argument; format = LOOP <N_REP>
        Variable *var = nullptr;
        if ((var = gc.getVar(cpc.fields[1].toStdString())) == nullptr) { // Variable not found
            if ((var = Variable::parseToVar("", cpc.fields[1].toStdString(), VariableType::INT)) == nullptr) {
                Logging::logMsg(QString("[L%1] LOOP statement value in 1st argument is not of type INT. Exit program.").arg(cpc.fileLineNumber), LoggingColor::RED);
                exit(1);
            }
        } else {
            if (var->getType() != VariableType::INT) {
                Logging::logMsg(QString("[L%1] LOOP statement variable in 1st argument is not of type INT, but of type %2. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::fromStdString(var->getTypeString())), LoggingColor::RED);
                delete var;
                exit(1);
            }
        }
        if (var != nullptr) {
            gc.addVar(var);
            IntVariable *x = dynamic_cast<IntVariable*>(var);
            LoopNode *loopNode = new LoopNode(m_tree.getCurrentContainer(), *x);
            m_tree.enterContainer(loopNode);
            cpc.ctrTypes.push(ContainerType::LOOP);
        }
    } else {
        Logging::logMsg(QString("[L%1] LOOP statement has invalid number of arguments (%2). Needs to be 0 or 1. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.fields.size() - 1)), LoggingColor::RED);
        exit(1);
    }

}

void CommandParser::parseBreakStatement(CommandParserContext &cpc)
{
    if (cpc.ctrTypes.size() > 0) { // inside a container (LOOP, IF)
        if (cpc.fields.size() - 1 == 0) { // 0 arguments
            m_tree.append(new BreakNode(m_tree.getCurrentContainer()));
        } else {
            Logging::logMsg(QString("[L%1] BREAK statement does not expect any arguments, but got %2. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.fields.size() - 1)), LoggingColor::RED);
            exit(1);
        }
    } else {
        Logging::logMsg(QString("[L%1] BREAK statement is only allowed inside LOOP or IF. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.fields.size() - 1)), LoggingColor::RED);
        exit(1);
    }
}

void CommandParser::parseExitStatement(CommandParserContext &cpc)
{
    if (cpc.fields.size() - 1 == 0) { // 0 arguments
        m_tree.append(new ExitNode(m_tree.getCurrentContainer()));
    } else {
        Logging::logMsg(QString("[L%1] EXIT statement does not expect any arguments, but got %2. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.fields.size() - 1)), LoggingColor::RED);
        exit(1);
    }
}

void CommandParser::parsePrintStatement(CommandParserContext &cpc)
{
    if (cpc.fields.size() - 1 >= 1) { // >= 1 argument(s); format = PRINT <STR> [, <STR> [...]]
        std::vector<Variable*> *values = new std::vector<Variable*>;
        Variable *var = nullptr;
        for (int f = 1; f < cpc.fields.size(); f++) {
            if ((var = gc.getVar(cpc.fields[f].toStdString())) == nullptr) { // Variable not found
                var = new StringVariable("", cpc.fields[f].toStdString());
                gc.addVar(var);
            }
            values->push_back(var);
        }
        std::function<void(std::string&)> cbLog = [&](std::string &str){ Logging::logMsg(QString::fromStdString(str)); };
        m_tree.append(new PrintNode(m_tree.getCurrentContainer(), values, cbLog));
    } else {
        Logging::logMsg(QString("[L%1] PRINT statement has invalid number of arguments. Needs to be >=1. Exit program.").arg(cpc.fileLineNumber), LoggingColor::RED);
        exit(1);
    }
}

void CommandParser::parseIfStatement(CommandParserContext &cpc)
{
    QString stmtUpper= cpc.fields[0].toUpper();

    if (stmtUpper == "IF")
    {
        if (cpc.fields.size() - 1 == 1) // 1 argument; format = IF <CONST_BOOL | BOOL_EXPR>
        {
            QString varName = cpc.fields[1];
            Variable *var = nullptr;
            if (varName.toUpper() == "TRUE" || varName.toUpper() == "FALSE") {
                var = new BoolVariable("", varName.toUpper() == "TRUE");
            } else if ((var = gc.getVar(varName.toStdString())) != nullptr) {
                if (var->getType() != VariableType::BOOL) {
                    delete var;
                    var = nullptr;
                }
            }
            if (var != nullptr) {
                gc.addVar(var);
                ICondition *cond = new BoolCondition(*dynamic_cast<BoolVariable*>(var));
                cpc.conds.push_back(cond);
                IfNode *ifNode = new IfNode(m_tree.getCurrentContainer(), *cond);
                m_tree.enterContainer(ifNode);
                cpc.ctrTypes.push(ContainerType::IF);
                cpc.ifNodes.push(ifNode);
            } else {
                Logging::logMsg(QString("[L%1] IF statement has an invalid argument. Needs to be TRUE, FALSE or an existing BOOL variable. Exit program.").arg(cpc.fileLineNumber), LoggingColor::RED);
                exit(1);
            }
        } else if (cpc.fields.size() - 1 == 3) { // 3 arguments; format = IF <LEFT_VAR_NAME> {LT, GT, LE, GE, EQ, NE} <RIGHT_VALUE>
            QString varName;
            // Check 1st parameter
            Variable *lVal = nullptr;
            varName = cpc.fields[1];
            if ((lVal = gc.getVar(varName.toStdString())) == nullptr) { // Variable not found
                Logging::logMsg(QString("[L%1] IF statement has invalid 1st argument. Needs to be an existing variable. Exit program.").arg(cpc.fileLineNumber), LoggingColor::RED);
                exit(1);
            }
            // Check 3rd parameter
            Variable *rVal = nullptr;
            varName = cpc.fields[3];
            if ((rVal = gc.getVar(varName.toStdString())) != nullptr) { // Variable found
                if (lVal->getType() != rVal->getType()) { // both variables need to be of the same type, as this is mandatory for out comparison operations later on
                    Logging::logMsg(QString("[L%1] IF statement variable in 3rd argument is not of same type as variable in 1st argument. Exit program.").arg(cpc.fileLineNumber), LoggingColor::RED);
                    exit(1);
                }
            } else { // Variable not found
                if ((rVal = Variable::parseToVar("", cpc.fields[3].toStdString(), lVal->getType())) != nullptr) { // Parse 3rd argument to same type as variable in 1st argument
                    gc.addVar(rVal);
                } else {
                    Logging::logMsg(QString("[L%1] IF statement value in 3rd argument is not interpretable to same type as variable in 1st argument (%2). Exit program.").arg(QString::number(cpc.fileLineNumber), QString::fromStdString(lVal->getTypeString())), LoggingColor::RED);
                    exit(1);
                }
            }
            // Check 2nd parameter
            ComparisonType compType;
            if (ComparisonCondition::getComparisonTypeFromString(cpc.fields[2].trimmed().toStdString(), compType)) {
                if (!ComparisonCondition::comparisonTypeValidForVariableType(compType, lVal->getType())) {
                    Logging::logMsg(QString("[L%1] IF statement comparison rule in 2nd argument (%2) is not compatible with the type of the variable in 1st argument (%3). Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[2].trimmed().toUpper(), QString::fromStdString(lVal->getTypeString())), LoggingColor::RED);
                    exit(1);
                }
            } else {
                Logging::logMsg(QString("[L%1] IF statement comparison rule in 2nd argument (%2) is not of {LT, GT, LE, GE, EQ, NE}. Exit program.").arg(QString::number(cpc.fileLineNumber), cpc.fields[2].trimmed().toUpper()), LoggingColor::RED);
                exit(1);
            }
            // Add IF container with comparison condition
            ICondition *cond = new ComparisonCondition(*lVal, *rVal, compType);
            cpc.conds.push_back(cond);
            IfNode *ifnode = new IfNode(m_tree.getCurrentContainer(), *cond);
            m_tree.enterContainer(ifnode);
            cpc.ifNodes.push(ifnode);
            cpc.ctrTypes.push(ContainerType::IF);
        } else {
            Logging::logMsg(QString("[L%1] IF statement has invalid number of arguments. Needs to be 1 or 3. Exit program.").arg(cpc.fileLineNumber), LoggingColor::RED);
            exit(1);
        }
    }
}

void CommandParser::parseElseStatement(CommandParserContext &cpc)
{
    if (cpc.ctrTypes.top() == ContainerType::IF) {
        IfNode *ifNode = cpc.ifNodes.top();
        if (cpc.fields.size() - 1 == 0) { // 0 arguments
            if (!ifNode->isInElseBranch()) {
                ifNode->switchToElseBranch();
                cpc.lastElseNodeLineNumber = cpc.fileLineNumber;
            } else {
                Logging::logMsg(QString("[L%1] ELSE statement (L%2) already found in current IF statement. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.lastElseNodeLineNumber)), LoggingColor::RED);
                exit(1);
            }
        } else {
            Logging::logMsg(QString("[L%1] ELSE statement does not expect any arguments, but got %2. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.fields.size() - 1)), LoggingColor::RED);
            exit(1);
        }
    } else {
        Logging::logMsg(QString("[L%1] ELSE statement outside IF block. Exit program.").arg(QString::number(cpc.fileLineNumber)), LoggingColor::RED);
        exit(1);
    }
}

void CommandParser::parseEndStatement(CommandParserContext &cpc)
{
    if (cpc.fields.size() - 1 == 0)  {// 0 arguments
        if (cpc.ctrTypes.size() > 0) {
            m_tree.leaveContainer();
            if (cpc.ctrTypes.top() == ContainerType::IF)
                cpc.ifNodes.pop();
            cpc.ctrTypes.pop();
        } else {
            Logging::logMsg(QString("[L%1] END statement outside LOOP or IF block. Exit program.").arg(QString::number(cpc.fileLineNumber)), LoggingColor::RED);
            exit(1);
        }
    } else {
        Logging::logMsg(QString("[L%1] END statement does not expect any arguments, but got %2. Exit program.").arg(QString::number(cpc.fileLineNumber), QString::number(cpc.fields.size() - 1)), LoggingColor::RED);
        exit(1);
    }
}
