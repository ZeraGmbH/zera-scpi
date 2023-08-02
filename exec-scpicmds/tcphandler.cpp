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
    QStringList cmdsSplitted = combinedCmd.split("|");
    int numAnsw = cmdsSplitted.size();
    m_indexAnswers = 0;
    m_cmdIsQuery.clear();
    QStringList cmdWithQueriesSplitted;
    for(auto &cmd : cmdsSplitted) {
        cmdWithQueriesSplitted.append(cmd);
        if (cmd.trimmed().endsWith("?") ) {
            m_cmdIsQuery.append(true);
        }
        else if (cmd.trimmed().endsWith(";")) {
            m_cmdIsQuery.append(false);
            // Add *OPC? as command-synchronization-barrier for the controller. Its result will get ignored later on.
            // This is necessary as a timeout gets used for each command and a queue filled
            // with long running commands erroneously might make working commands running into a timeout.
            cmdWithQueriesSplitted.append("*OPC?");
        }
    }
    m_tcpSocket.write(cmdWithQueriesSplitted.join("|").toLocal8Bit() + "\n");
    m_tcpSocket.waitForBytesWritten();

    while(numAnsw > 0) {
        numAnsw -= onReceive();
    }
}

void TcpHandler::disconnectFromHost()
{
    m_tcpSocket.disconnect();
    m_tcpSocket.close();
    if(m_tcpSocket.state() == QAbstractSocket::UnconnectedState || m_tcpSocket.waitForDisconnected())
        qInfo("TCP connection was closed!");
    m_tcpSocket.deleteLater();
}

int TcpHandler::onReceive()
{
    int numAnsw = 0;
    m_tcpSocket.waitForReadyRead(3000);
    m_receiveBuffer.append(m_tcpSocket.readAll());
    while(m_receiveBuffer.contains("\n")) {
        numAnsw ++;
        QString answer = m_receiveBuffer.left(m_receiveBuffer.indexOf("\n"));
        m_receiveBuffer.remove(0, answer.length()+1);
        if(m_cmdIsQuery[m_indexAnswers]) {
            QDateTime now = QDateTime::currentDateTime();
            QString strOut = QStringLiteral("[%1]: %2").arg(now.toString("HH:mm:ss"),answer);
            qInfo("%s", qPrintable(strOut));
        }
        m_indexAnswers ++;
    }
    return numAnsw;
}

void TcpHandler::onDisconnect()
{
    qFatal("Server closed connection unexpected!");
}
