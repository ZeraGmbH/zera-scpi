#include "tcphandler.h"


TcpHandler::TcpHandler(QObject *parent)
    : QObject{parent}
{

}

void TcpHandler::connectTCP(QString hostName, quint16 port)
{
    m_TcpSocket->connectToHost(hostName, port);
    if(!m_TcpSocket->waitForConnected()) {
        QString strError = QStringLiteral("IP connection %i:%2 could not be opened!").arg(hostName).arg(port);
        emit OperationFinish(true, strError);
    }
    else {
        qInfo("IP connection %s:%i was opened successfully!", qPrintable(hostName), port);
        connect(m_TcpSocket, &QTcpSocket::readyRead, this, &TcpHandler::onReceive);
        connect(m_TcpSocket, &QTcpSocket::disconnected, this, &TcpHandler::onDisconnect);
        emit OperationFinish(false, "");
    }
}

void TcpHandler::sendCommand(QString cmd)
{

}

void TcpHandler::onReceive()
{
    if(m_TcpSocket) {
        QString answer = m_TcpSocket->readAll();
        answer.replace("\n", "");
        bool error = answer.contains(",ERROR", Qt::CaseInsensitive);
        qInfo("<--  %s ", qPrintable(answer));
        if(!error)
            emit cmdFinish();
        else {
            qInfo("Abort on external error!");
            //emit kill(-1);
        }
    }
}

void TcpHandler::onDisconnect()
{
    qFatal("Server closed connection unexpected!");
}
