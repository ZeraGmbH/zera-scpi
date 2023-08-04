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
    void parseCmdFile(QString strFileName, unsigned int handleErroneousMessages);

signals:
    void done(int exitCode);

private:
    bool msgsAreValid();
    void sendMsgs();
    void removeInvalidMsgs(bool silent);

    TcpHandler& m_tcpHandler;
    QList<std::shared_ptr<MessageData>> m_msgs;
};

#endif // PARSECMDSINFILE_H
