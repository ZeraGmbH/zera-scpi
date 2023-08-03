#ifndef LOGGING_H
#define LOGGING_H

#include <QObject>


enum class LoggingColor
{
    NONE = 0,
    RED,
    GREEN,
    BLUE,
};


class Logging : public QObject
{
    Q_OBJECT
public:
    enum Color {
        LOG_COLOUR_NONE = 0,
        LOG_COLOUR_RED,
        LOG_COLOUR_GREEN,
        LOG_COLOUR_BLUE,
        };

    explicit Logging(QObject *parent = nullptr);
    static QString ColorString(QString strMsg, LoggingColor color = LoggingColor::NONE);
    static void LogMsg(QString strMsg, LoggingColor color = LoggingColor::NONE);

signals:

private:
    static const bool m_enableGlobalColorLogging = true;
};

#endif // LOGGING_H
