#include <QFile>
#include <QTextStream>
#include <QObject>
#include "commandparser.h"


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
            if(!strLine.isEmpty() && !strLine.startsWith("#"))
            {
                QStringList cmds;
                for (QString &cmd : strLine.split("|", Qt::SkipEmptyParts))
                    cmds.append(cmd.trimmed());
                m_strCmdList.append(cmds);
            }
        }
        executeFile.close();
        // Check commands
        // checkCmds();
        // Send
        sendCmds();
    }
    else
        qInfo("Execution file %s could not be opened!", qPrintable(strFileName));
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
    for (QStringList &line : m_strCmdList)
    {
        for (QString &cmd : line)
            m_tcpHandler.sendCommand(cmd);
    }
    m_tcpHandler.disconnectFromHost();
}
