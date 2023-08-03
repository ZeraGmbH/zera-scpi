#include <QFile>
#include <QTextStream>
#include <QObject>
#include "commandparser.h"
#include "logging.h"


CommandParser::CommandParser(TcpHandler &tcpHandler) :
    m_tcpHandler(tcpHandler)
{
}

void CommandParser::parseCmdFile(QString strFileName)
{
    QFile executeFile(strFileName);

    if(executeFile.open(QIODevice::ReadOnly)) {
        m_strCmdList.clear();

        // Read lines and split into single commands (per line)
        QTextStream textStream(&executeFile);

        while (!textStream.atEnd()) {
            QString strLine = textStream.readLine().trimmed();
            if(!strLine.isEmpty() && !strLine.startsWith("#")) {
                m_cmds.append(strLine);
            }
        }
        executeFile.close();
        // Check commands
        // checkCmds();
        // Send
        sendCmds();
    }
    else
        Logging::LogMsg(QString("Command file \"%1\" could not be opened!").arg(strFileName), LoggingColor::RED);
}

void CommandParser::checkCmds()
{
    for (QStringList &line : m_strCmdList)
    {
        for (QString &cmd : line)
        {
            if (cmd.isEmpty())
                ; // TODO handle error

            // Check for ASCII characters and allowed symboles only
            //bool containsNonASCII = myString.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]"))); // a-zA-z0-9.:,;*?-+[[:blank:]] (even more symbols?)

            qInfo("%s", qPrintable(cmd));
        }
        qInfo("");
    }
}

void CommandParser::sendCmds()
{
    for (QString &cmd : m_cmds)
        m_tcpHandler.sendCommand(cmd);
    m_tcpHandler.disconnectFromHost();
}
