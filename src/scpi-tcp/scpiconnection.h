#ifndef SCPICONNECTION_H
#define SCPICONNECTION_H

#include "scpidelegate.h"
#include <QObject>
#include <QList>

class cSCPI;
class cNotificationString;


class cSCPIConnection: public QObject // pure virtual base class for scpi model interfaces
{
    Q_OBJECT

public:
    cSCPIConnection(QObject* parent=0);
    virtual ~cSCPIConnection();
    virtual void initSCPIConnection(QString leadingNodes) = 0;
    virtual void removeSCPIConnections();

signals:
    void notifier(cNotificationString* notifier);
    void cmdExecutionDone(cProtonetCommand* protoCmd);

protected:
    cSCPI* m_pSCPIInterface;
    QList<cSCPIDelegate*> m_DelegateList;


protected slots:
    virtual void executeCommand(int, QString&, QString&);
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd) = 0;
};

#endif // SCPICONNECTION_H
