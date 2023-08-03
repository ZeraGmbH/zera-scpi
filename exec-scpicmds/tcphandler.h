#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include "messagedata.h"


class TcpHandler : public QObject
{
    Q_OBJECT
public:
    TcpHandler();
    bool connectTCP(QString hostName, quint16 port);
    void sendMessage(MessageData &msg);
    void disconnectFromHost();

signals:
    void cmdFinish();

private slots:
    int receiveAnswers(MessageData &msg);
    void onDisconnect();

private:
    QTcpSocket m_tcpSocket;
    QString m_receiveBuffer;
    int m_indexAnswers;
};

#endif // TCPHANDLER_H
