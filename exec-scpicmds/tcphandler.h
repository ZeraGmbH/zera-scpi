#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QObject>
#include <QTcpSocket>


class TcpHandler : public QObject
{
    Q_OBJECT
public:
    TcpHandler();
    bool connectTCP(QString hostName, quint16 port);
    void sendCommand(QString combinedCmd);
    void disconnectFromHost();

signals:
    void cmdFinish();

private slots:
    int onReceive();
    void onDisconnect();

private:
    QTcpSocket m_tcpSocket;
    QString m_receiveBuffer;
};

#endif // TCPHANDLER_H
