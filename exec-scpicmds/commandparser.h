#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QObject>
#include "tcphandler.h"
#include "messagedata.h"
#include "nodetree.h"
#include "variable.h"
#include "context.h"


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
    void sendMsgs(unsigned int iterNo = 0);
    void loopAndSendMsgs();
    void removeInvalidMsgs(bool silent);

    unsigned int m_handleErroneousMessages = 0;
    bool m_checkErrorQueue = false;
    TcpHandler& m_tcpHandler;
    quint8 m_numLoops = 1;
    NodeTree m_tree;
    Context gc; // global context
};

#endif // COMMANDPARSER_H
