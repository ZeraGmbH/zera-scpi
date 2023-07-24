#include "handletelnetconnection.h"

HandleTelnetConnection::HandleTelnetConnection(QObject *parent)
    : QObject{parent}
{

}

void HandleTelnetConnection::setConnection(QString hostName, quint16 port)
{
    m_TcpSocket->connectToHost(hostName, port);
    if(!m_TcpSocket->waitForConnected()) {
        QString strError = QStringLiteral("IP connection %i:%2 could not be opened!").arg(hostName).arg(port);
        emit OperationFinish(true, strError);
    }
    else {
        qInfo("IP connection %s:%i was opened successfully!", qPrintable(hostName), port);
        //SelectSocket(pNewSocket);
        emit OperationFinish(false, "");
    }
}
