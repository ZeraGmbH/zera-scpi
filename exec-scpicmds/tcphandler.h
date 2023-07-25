#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QObject>
#include <QTcpSocket>


class TcpHandler : public QObject
{
    Q_OBJECT
public:
    TcpHandler(QObject *parent);
    void connectTCP(QString hostName, quint16 port);
    void sendCommand(QString cmd);

signals:
    void OperationFinish(bool bError, QString strMessage);
    void cmdFinish();

private slots:
    void onReceive();
    void onDisconnect();

private:
    QTcpSocket *m_TcpSocket;
};

#endif // TCPHANDLER_H
