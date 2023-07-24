#include "parsecmdsinfile.h"
#include <QFile>
#include <QTextStream>

ParseCmdsInFile::ParseCmdsInFile(QObject *parent)
    : QObject{parent}
{

}

void ParseCmdsInFile::StartFileExecution(QString strFileName)
{
    QFile executeFile(strFileName);
    if(executeFile.open(QIODevice::ReadOnly)) {
        m_strCmdList.clear();
        QTextStream textStream(&executeFile);
        while (!textStream.atEnd()) {
            QString strLine = textStream.readLine();
            if(!strLine.isEmpty())
                m_strCmdList.append(strLine);
        }
        for(int iter= 0; iter<m_strCmdList.size(); iter++) {
            if(m_strCmdList[iter].contains("|")) {
                handleCombinedCmds(m_strCmdList[iter]);
            }
        }
        executeFile.close();
        //emit next();
    }
    else {
        qInfo("Execution file %s could not be opened!", qPrintable(strFileName));
        //emit done(-1);
    }

}

void ParseCmdsInFile::handleCombinedCmds(QString combCmd)
{
    //we have this in scpiclient
}
