#include <QStringList>
#include <scpi.h>
#include <scpiobject.h>
#include <QDebug>

#include "scpidelegate.h"


cSCPIDelegate::cSCPIDelegate(QString cmdParent, QString cmd, quint8 type, cSCPI *scpiInterface, quint16 cmdCode)
    :cSCPIObject(cmd, type), m_nCmdCode(cmdCode)
{
    m_sCommand = QString("%1:%2").arg(cmdParent).arg(cmd);
    scpiInterface->genSCPICmd(cmdParent.split(":"), this);
}


bool cSCPIDelegate::executeSCPI(const QString &sInput, QString &sOutput)
{
    emit execute(m_nCmdCode,(QString&) sInput, sOutput);
    return true;
}


bool cSCPIDelegate::executeSCPI(cProtonetCommand *protoCmd)
{
    emit execute(m_nCmdCode, protoCmd);
    return true;
}


QString cSCPIDelegate::getCommand()
{
    return m_sCommand;
}


