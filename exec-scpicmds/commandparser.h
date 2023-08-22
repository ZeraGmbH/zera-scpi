#ifndef PARSECMDSINFILE_H
#define PARSECMDSINFILE_H

#include <QObject>
#include "tcphandler.h"
#include "messagedata.h"


class CommandParser : public QObject
{
    Q_OBJECT
public:
    CommandParser(TcpHandler& tcpHandler);
    void setHandleErroneousMessages(unsigned int handleErroneousMessages);
    void setCheckErrorQueue(bool checkErrorQueue);
    void setLoopNumber(quint8 numLoops);
    void parseCmdFile(QString strFileName);

signals:
    void done(int exitCode);

private:
    bool msgsAreValid();
    void sendMsgs();
    void loopAndSendMsgs();
    void removeInvalidMsgs(bool silent);

    unsigned int m_handleErroneousMessages = 0;
    bool m_checkErrorQueue = 0;
    TcpHandler& m_tcpHandler;
    QList<std::shared_ptr<MessageData>> m_msgs;
    quint8 m_numLoops;
};

#endif // PARSECMDSINFILE_H
