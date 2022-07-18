#ifndef SCPIDELEGATE_H
#define SCPIDELEGATE_H

#include <QObject>
#include <QString>

#include "scpiobject.h"

class cSCPI;
class cProtonetCommand;

class cSCPIDelegate: public QObject, public cSCPIObject
{
   Q_OBJECT

public:
    cSCPIDelegate(QString cmdParent, QString cmd, quint8 type, cSCPI *scpiInterface, quint16 cmdCode);
    virtual bool executeSCPI(const QString& sInput, QString& sOutput);
    virtual bool executeSCPI(cProtonetCommand* protoCmd);
    QString getCommand();

signals:
    void execute(int cmdCode, QString& sInput, QString& sOutput);
    void execute(int cmdCode, cProtonetCommand* protoCmd);

private:
    quint16 m_nCmdCode;
    QString m_sCommand;
};


#endif // SCPIDELEGATE_H
