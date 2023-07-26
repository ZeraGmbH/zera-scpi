#include "tcphandler.h"


TcpHandler::TcpHandler()
{

}

bool TcpHandler::connectTCP(QString hostName, quint16 port)
{
    m_tcpSocket.connectToHost(hostName, port);

    if(!m_tcpSocket.waitForConnected()) {
        return false;
    }
    else
    {
        connect(&m_tcpSocket, &QTcpSocket::disconnected, this, &TcpHandler::onDisconnect);
        if(m_tcpSocket.state() != QAbstractSocket::ConnectedState)
            return false;
        return true;
    }
}

void TcpHandler::sendCommand(QString cmd)
{
    qInfo("-->  %s ", qPrintable(cmd));
    qint64 res = m_tcpSocket.write(cmd.toLocal8Bit() + "\n");
    m_tcpSocket.waitForBytesWritten();

    if (cmd.endsWith("?"))
        onReceive();
}

void TcpHandler::onReceive()
{
    m_tcpSocket.waitForReadyRead(1000);
    QString answer = m_tcpSocket.readAll();
    answer.replace("\n", "");
    qInfo("<--  %s ", qPrintable(answer));
}

void TcpHandler::onDisconnect()
{
    qFatal("Server closed connection unexpected!");
}
