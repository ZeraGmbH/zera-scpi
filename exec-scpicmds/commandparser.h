#ifndef PARSECMDSINFILE_H
#define PARSECMDSINFILE_H

#include <QObject>
#include "tcphandler.h"


class CommandParser : public QObject
{
    Q_OBJECT
public:
    CommandParser(TcpHandler& tcpHandler);
    void parseCmdFile(QString strFileName);

signals:
    void done(int exitCode);

private:
    void checkCmds();
    void sendCmds();

    QList<QStringList> m_strCmdList;
    TcpHandler& m_tcpHandler;
    QStringList m_cmds;
};

#endif // PARSECMDSINFILE_H
