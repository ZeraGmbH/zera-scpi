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
    void sendCommand(QString cmd);

signals:
    void cmdFinish();

private slots:
    void onReceive();
    void onDisconnect();

private:
    QTcpSocket m_tcpSocket;
};

#endif // TCPHANDLER_H
