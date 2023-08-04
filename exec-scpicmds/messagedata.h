#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include <QObject>
#include <memory>

enum class CommandType
{
    UNKNOWN = 0,
    CMD,
    QUERY,
    EMPTY,
};

class CommandData : public QObject
{
    Q_OBJECT
public:
    explicit CommandData(QObject *parent = nullptr);
    static CommandType determineCommandType(QString cmd);
    QString m_cmd;
    QString m_cmdTrimmed;
    CommandType m_cmdType;
    int m_posInMsg;
    QString m_result;
    QString m_testRule;

signals:
};


class MessageData : public QObject
{
    Q_OBJECT
public:
    explicit MessageData(QObject *parent = nullptr);
    QString m_oriMsg;
    int m_fileLineNumber;
    QString m_testRules;
    bool m_isValid;
    QList<std::shared_ptr<CommandData>> m_cmds;
    int m_cmdCountStrWidth;

signals:
};

#endif // MESSAGEDATA_H
