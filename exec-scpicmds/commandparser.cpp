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

void CommandParser::parseCmdFile(QString strFileName, bool ignoreErroneousCommands)
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
                QString& msg = line; // Just to make clear, this line is a messages
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

        // Check commands
        Logging::LogMsg(QString("Checking commands..."));
        if (cmdsAreValid()) {

            Logging::LogMsg(QString("... All commands look fine."), LoggingColor::GREEN);
            sendCmds();
        }
        else {
            if (ignoreErroneousCommands) {
                Logging::LogMsg(QString("... There are erroneous commands. Trying to execute them anyway."), LoggingColor::RED);
                sendCmds();
            }
            else {
                Logging::LogMsg(QString("... Invalid command(s) found!"), LoggingColor::RED);
            }
        }
    }
    else
    {
        Logging::LogMsg(QString("Command file \"%1\" could not be opened!").arg(strFileName), LoggingColor::RED);
    }

    // Cleanup
    m_msgs.clear();
}

bool CommandParser::cmdsAreValid()
{
    bool cmsAreValid = true;

    for (auto &msg : m_msgs) {
        for (auto &cmd : msg->m_cmds) {
            if (cmd->m_cmdType == CommandType::EMPTY)
            {
                Logging::LogMsg(QString(" > Empty command found in line %1:%2!").arg(QString::number(msg->m_fileLineNumber), QString::number(cmd->m_posInMsg)), LoggingColor::RED);
                cmsAreValid = false;
            }
            else if (cmd->m_cmdType == CommandType::UNKNOWN)
            {
                Logging::LogMsg(QString(" > Command \"%1\" of unknown type found in line %2:%3! Is there a missing \";\"?").arg(cmd->m_cmdTrimmed, QString::number(msg->m_fileLineNumber), QString::number(cmd->m_posInMsg)), LoggingColor::RED);
                cmsAreValid = false;
            }
            // Check for ASCII characters and allowed symboles only
            //bool containsNonASCII = myString.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]"))); // a-zA-z0-9.:,;*?-+[[:blank:]] (even more symbols?)
        }
    }

    return cmsAreValid;
}

void CommandParser::sendCmds()
{
    for (auto &msg : m_msgs)
        m_tcpHandler.sendMessage(*msg);
    m_tcpHandler.disconnectFromHost();
}
