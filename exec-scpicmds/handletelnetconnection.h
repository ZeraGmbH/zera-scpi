#ifndef HANDLETELNETCONNECTION_H
#define HANDLETELNETCONNECTION_H

#include <QObject>
#include <QTcpSocket>

class HandleTelnetConnection : public QObject
{
    Q_OBJECT
public:
    HandleTelnetConnection(QObject *parent);
    void setConnection(QString hostName, quint16 port);
signals:
    void OperationFinish(bool bError, QString strMessage);

private:
    QTcpSocket *m_TcpSocket;
};

#endif // HANDLETELNETCONNECTION_H
