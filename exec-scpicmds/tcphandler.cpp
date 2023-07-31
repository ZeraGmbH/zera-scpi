#include "tcphandler.h"
#include <QDateTime>

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

void TcpHandler::sendCommand(QString combinedCmd)
{
    qInfo("-->  %s ", qPrintable(combinedCmd));
    m_tcpSocket.write(combinedCmd.toLocal8Bit() + "\n");
    m_tcpSocket.waitForBytesWritten();

    int numAnsw = 0;
    for(auto &cmd : combinedCmd.split("|")) {
        if (cmd.trimmed().endsWith("?"))
            numAnsw ++;
    }
    while(numAnsw > 0) {
        numAnsw -=onReceive();
    }
}

void TcpHandler::disconnectFromHost()
{
    m_tcpSocket.disconnect();
    m_tcpSocket.close();
    if(m_tcpSocket.state() == QAbstractSocket::UnconnectedState || m_tcpSocket.waitForDisconnected())
        qInfo("disconnected");
    m_tcpSocket.deleteLater();
}

int TcpHandler::onReceive()
{
    int numAnsw = 0;
    m_tcpSocket.waitForReadyRead(1000000);
    m_receiveBuffer.append(m_tcpSocket.readAll());
    while(m_receiveBuffer.contains("\n")) {
        numAnsw ++;
        QString answer = m_receiveBuffer.left(m_receiveBuffer.indexOf("\n"));
        m_receiveBuffer.remove(0, answer.length()+1);
        QDateTime now = QDateTime::currentDateTime();
        QString strOut = QStringLiteral("[%1]: %2").arg(now.toString("HH:mm:ss"),answer);
        qInfo("%s", qPrintable(strOut));
    }
    return numAnsw;
}

void TcpHandler::onDisconnect()
{
    qFatal("Server closed connection unexpected!");
}
