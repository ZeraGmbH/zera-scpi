#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QObject>

class CommandParser : public QObject
{
    Q_OBJECT
public:
    CommandParser(QObject *parent = nullptr);
    void StartFileExecution(QString strFileName);

signals:
    void done(int exitCode);

private:
    void handleCombinedCmds(QString combCmd);

    QList<QString> m_strCmdList;
};

#endif // COMMANDPARSER_H
