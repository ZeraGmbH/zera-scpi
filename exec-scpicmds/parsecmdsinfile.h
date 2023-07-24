#ifndef PARSECMDSINFILE_H
#define PARSECMDSINFILE_H

#include <QObject>

class ParseCmdsInFile : public QObject
{
    Q_OBJECT
public:
    ParseCmdsInFile(QObject *parent = nullptr);
    void StartFileExecution(QString strFileName);

signals:
    void done(int exitCode);

private:
    void handleCombinedCmds(QString combCmd);

    QList<QString> m_strCmdList;
};

#endif // PARSECMDSINFILE_H
