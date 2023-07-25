#include <QFile>
#include <QTextStream>
#include "commandparser.h"


CommandParser::CommandParser(QObject *parent)
    : QObject{parent}
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
                m_strCmdList.append(strLine.split("|"));
        }
        executeFile.close();

        // Check commands
        checkCmds();

        // Send
        sendCmds();
    }
    else {
        qInfo("Execution file %s could not be opened!", qPrintable(strFileName));
        //emit done(-1);
    }

}

void CommandParser::checkCmds()
{
    // ... only ascii; no empty commands "| MEAS:VOLT:MAX 70;| öäüß |, ...
}

void CommandParser::sendCmds()
{
    for (QStringList &line : m_strCmdList)
    {
        for (QString &cmd : line)
        {
            qInfo("%s", qPrintable(cmd));
        }
        qInfo("");
    }
}
