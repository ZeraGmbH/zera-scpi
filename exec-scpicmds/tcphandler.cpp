#include "tcphandler.h"
#include "logging.h"

TcpHandler::TcpHandler(unsigned int ms) :
    m_timeout(ms)
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

void TcpHandler::sendMessage(MessageData &msg)
{
    QString lineStr = QString::number(msg.m_fileLineNumber).rightJustified(2, '0');
    Logging::logMsg(QString("--> [L%1] %2").arg(lineStr,  msg.m_oriMsg));

    QString msgPartPos;
    int posNum = 1;
    for (auto &cmd : msg.m_cmds)
    {
        msgPartPos = msgPartPos.leftJustified(cmd->m_posInMsg - 1, ' ');
        msgPartPos += QString("[%1]").arg(QString::number(posNum++));
    }
    Logging::logMsg(QString("%1%2").arg(QString("").leftJustified(lineStr.length() + 8), msgPartPos), LoggingColor::GREEN);

    int numAnsw = msg.m_cmds.count();
    m_indexAnswers = 0;
    QStringList cmdWithQueriesSplitted;
    for(auto &cmd : msg.m_cmds) {
        cmdWithQueriesSplitted.append(cmd->m_cmd);
        if (cmd->m_cmdType == CommandType::CMD) {
            // Add *OPC? as command-synchronization-barrier for the controller. Its result will get ignored later on.
            // This is necessary as a timeout gets used for each command and a queue filled
            // with long running commands erroneously might make working commands running into a timeout.
            cmdWithQueriesSplitted.append("*OPC?");
        }
    }
    m_tcpSocket.write(cmdWithQueriesSplitted.join("|").toLocal8Bit() + "\n");
    m_tcpSocket.waitForBytesWritten();

    while(numAnsw > 0) {
        int numRecvAnsw = receiveAnswers(msg);
        if (numRecvAnsw >= 0) {
            numAnsw -= numRecvAnsw;
        }
        else {
            Logging::logMsg(QString("Waiting for response timed out."), LoggingColor::RED);
            break;
        }
    }
}

void TcpHandler::sendMessageRaw(QString msg)
{
    m_tcpSocket.write(msg.toLocal8Bit());
    m_tcpSocket.waitForBytesWritten();
}

void TcpHandler::disconnectFromHost()
{
    m_tcpSocket.disconnect();
    m_tcpSocket.close();
    if(m_tcpSocket.state() == QAbstractSocket::UnconnectedState || m_tcpSocket.waitForDisconnected())
        Logging::logMsg(QString("TCP connection was closed."));
}

int TcpHandler::receiveAnswers(MessageData &msg)
{
    int numAnsw = 0;
    if (!m_tcpSocket.waitForReadyRead(m_timeout))
        return -1;
    m_receiveBuffer.append(m_tcpSocket.readAll());
    while(m_receiveBuffer.contains("\n")) {
        numAnsw++;
        QString answer = m_receiveBuffer.left(m_receiveBuffer.indexOf("\n"));
        m_receiveBuffer.remove(0, answer.length() + 1);
        if (m_indexAnswers < msg.m_cmds.count()) {
            if (msg.m_cmds[m_indexAnswers]->m_cmdType == CommandType::QUERY)
                Logging::logMsg(QString(" <-[%1] %2").arg(QString::number(m_indexAnswers + 1).rightJustified(msg.m_cmdCountStrWidth, '0'), answer));
        }
        else {
            Logging::logMsg(QString(" <-[%1] %2 -> Out of expected answer range (1-%3)!").arg(QString::number(m_indexAnswers + 1), answer, QString::number(msg.m_cmds.count())), LoggingColor::RED);
        }
        m_indexAnswers++;
    }
    return numAnsw;
}

QStringList TcpHandler::receiveAnswersRaw(unsigned int answersCnt)
{
    int numAnsw = 0;
    QString receiveBuffer;
    QStringList answers;


    while(true) {
        if (!m_tcpSocket.waitForReadyRead(m_timeout))
            break;

        receiveBuffer.append(m_tcpSocket.readAll());

        if (receiveBuffer.contains("\n")) {
            QString answer = receiveBuffer.left(receiveBuffer.indexOf("\n"));
            receiveBuffer.remove(0, answer.length() + 1);
            answers.append(answer);
            numAnsw++;

            if (numAnsw == answersCnt)
                break;
        }
    }

    return answers;
}

void TcpHandler::onDisconnect()
{
    Logging::logMsg(QString("Server closed connection unexpected!"), LoggingColor::RED);
}
