#ifndef SCPICONNECTION_H
#define SCPICONNECTION_H

#include "scpidelegate.h"
#include <QList>
#include "notificationstring.h"
#include "notificationvalue.h"

class cSCPIConnection: public QObject // pure virtual base class for scpi model interfaces
{
    Q_OBJECT
public:
    cSCPIConnection(cSCPI* scpiInterface);
    virtual ~cSCPIConnection();
    virtual void initSCPIConnection(QString leadingNodes) = 0;
signals:
    void strNotifier(cNotificationString* notifier);
    void valNotifier(cNotificationValue* notifier);
    void cmdExecutionDone(cProtonetCommand* protoCmd);
protected:
    void removeSCPIConnections();
    cSCPI* m_pSCPIInterface;
    QList<cSCPIDelegate*> m_DelegateList;
protected slots:
    virtual void executeCommand(int, QString&, QString&);
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd) = 0;
};

#endif // SCPICONNECTION_H
