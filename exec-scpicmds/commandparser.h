#ifndef PARSECMDSINFILE_H
#define PARSECMDSINFILE_H

#include <QObject>
#include "tcphandler.h"


class CommandParser : public QObject
{
    Q_OBJECT
public:
    CommandParser(QObject *parent = nullptr);
    void parseCmdFile(QString strFileName);

signals:
    void done(int exitCode);

private:
    void checkCmds();
    void sendCmds();

    QList<QStringList> m_strCmdList;
};

#endif // PARSECMDSINFILE_H
