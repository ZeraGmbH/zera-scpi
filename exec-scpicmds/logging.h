#ifndef LOGGING_H
#define LOGGING_H

#include <QObject>


enum class LoggingColor
{
    NONE = 0,
    RED,
    GREEN,
    BLUE,
    YELLOW,
};


class Logging : public QObject
{
    Q_OBJECT
public:
    explicit Logging(QObject *parent = nullptr);
    static QString colorString(QString strMsg, LoggingColor color = LoggingColor::NONE);
    static void logMsg(QString strMsg, LoggingColor color = LoggingColor::NONE);

signals:

private:
    static const bool m_enableGlobalColorLogging = true;
};

#endif // LOGGING_H
