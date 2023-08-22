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

enum class LoggingStyle
{
    NONE = 0,
    BOLD,
    ITALIC,
    UNDERLINE,
    STRIKETHROUGH,
};


class Logging : public QObject
{
    Q_OBJECT
public:
    explicit Logging(QObject *parent = nullptr);
    static QString colorString(QString strMsg, LoggingColor color = LoggingColor::NONE, LoggingStyle style = LoggingStyle::NONE);
    static void logMsg(QString strMsg, LoggingColor color = LoggingColor::NONE, LoggingStyle style = LoggingStyle::NONE);

signals:

private:
    static const bool m_enableGlobalColorLogging = true;
};

#endif // LOGGING_H
