#include <QFile>
#include <QTextStream>
#include <QObject>
#include <memory>
#include "commandparser.h"
#include "logging.h"


CommandParser::CommandParser(TcpHandler &tcpHandler) :
    m_tcpHandler(tcpHandler)
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

void CommandParser::parseCmdFile(QString strFileName)
{
    QFile cmdFile(strFileName);

    if(cmdFile.open(QIODevice::ReadOnly)) {
        // Read lines and split into single commands (per line)
        QTextStream textStream(&cmdFile);
        int fileLineNumber = 0;

        // Iterate over all lines (messages) in file
        while (!textStream.atEnd()) {
            QString line = textStream.readLine().trimmed();
            fileLineNumber++;
            if(!line.isEmpty() && !line.startsWith("#")) {
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
                    cmdData->m_testRule = ""; // TODO
                    msgData->m_cmds.append(cmdData);
                }
                msgData->m_cmdCountStrWidth = QString::number(msgData->m_cmds.count()).length();
                m_msgs.append(msgData);
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
                sendMsgs();
            break;

        case 1:
            if (!msgsValid)
                Logging::logMsg(QString("... -> Trying to execute them anyway."), LoggingColor::YELLOW);
            sendMsgs();
            break;

        case 2:
            if (!msgsValid) {
                Logging::logMsg(QString("... -> Removing invalid messages and executing the rest."), LoggingColor::YELLOW);
                removeInvalidMsgs(false);
            }
            sendMsgs();
            break;

        case 3:
            if (!msgsValid) {
                Logging::logMsg(QString("... -> Removing invalid messages (silently) and executing the rest."), LoggingColor::YELLOW);
                removeInvalidMsgs(true);
            }
            sendMsgs();
            break;
        }
    }
    else
    {
        Logging::logMsg(QString("Command file \"%1\" could not be opened!").arg(strFileName), LoggingColor::RED);
    }

    // Cleanup
    m_msgs.clear();
}

bool CommandParser::msgsAreValid()
{
    bool msgsAreValid = true;

    for (auto &msg : m_msgs) {
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

void CommandParser::sendMsgs()
{
    if (!m_msgs.isEmpty())
    {
        Logging::logMsg(QString("Sending messages..."), LoggingColor::BLUE);

        // Initially clear target's error queue
        if (m_checkErrorQueue) {
            m_tcpHandler.sendMessageRaw("SYSTEM:ERROR:ALL?\n");
            QStringList answers = m_tcpHandler.receiveAnswersRaw(1); // Ignore result, just for sync
        }

        for (auto &msg : m_msgs) {
            m_tcpHandler.sendMessage(*msg);

            if (m_checkErrorQueue) {
                // Check target's error queue after current message //XXX what happens if in the cmd file itself there's a system:error:*? command?
                m_tcpHandler.sendMessageRaw("SYSTEM:ERROR:ALL?\n");
                QStringList answers = m_tcpHandler.receiveAnswersRaw(1);
                if (answers.length() == 1 && answers[0] != "+0,No error")
                    Logging::logMsg(QString("Message produced error(s) \"%1\".").arg(answers[0]), LoggingColor::RED);
            }
        }
    }
    else
    {
        Logging::logMsg(QString("No messages to send."), LoggingColor::BLUE);
    }
}


void CommandParser::removeInvalidMsgs(bool silent)
{
    int i = 0;
    while (i < m_msgs.length()) {
        auto &msg = m_msgs[i];
        if (!msg->m_isValid) {
            if (!silent)
                Logging::logMsg(QString(" Remove invalid message [L%1] \"%2\"").arg(QString::number(msg->m_fileLineNumber).rightJustified(2, '0'), msg->m_oriMsg), LoggingColor::YELLOW);
            m_msgs.removeAt(i);
        }
        else {
            i++;
        }
    }
}
