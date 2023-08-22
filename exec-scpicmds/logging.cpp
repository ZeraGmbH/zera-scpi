#include <QDateTime>
#include "logging.h"

Logging::Logging(QObject *parent)
    : QObject{parent}
{
}

QString Logging::colorString(QString strMsg, LoggingColor color, LoggingStyle style)
{
    QString colorPre, colorPost, stylePre, stylePost;

    if(m_enableGlobalColorLogging)
    {
        switch(color)
        {
        case LoggingColor::NONE:
            break;
        case LoggingColor::RED:
            colorPre = QStringLiteral("\033[0;31m");
            colorPost = QStringLiteral("\033[0;0m");
            break;
        case LoggingColor::GREEN:
            colorPre = QStringLiteral("\033[0;32m");
            colorPost = QStringLiteral("\033[0;0m");
            break;
        case LoggingColor::BLUE:
            colorPre = QStringLiteral("\033[0;34m");
            colorPost = QStringLiteral("\033[0;0m");
            break;
        case LoggingColor::YELLOW:
            colorPre = QStringLiteral("\033[0;33m");
            colorPost = QStringLiteral("\033[0;0m");
            break;
        }

        switch(style)
        {
        case LoggingStyle::NONE:
            break;
        case LoggingStyle::BOLD:
            stylePre = QStringLiteral("\033[1m");
            stylePost = QStringLiteral("\033[0m");
            break;
        case LoggingStyle::ITALIC:
            stylePre = QStringLiteral("\033[1m");
            stylePost = QStringLiteral("\033[0m");
            break;
        case LoggingStyle::UNDERLINE:
            stylePre = QStringLiteral("\033[4m");
            stylePost = QStringLiteral("\033[0m");
            break;
        case LoggingStyle::STRIKETHROUGH:
            stylePre = QStringLiteral("\033[9m");
            stylePost = QStringLiteral("\033[0m");
            break;
        }
    }

    QString strOut = QStringLiteral("%2%3%4%5%6")
            .arg(stylePre, colorPre, strMsg, colorPost, stylePost);

    return strOut;
}

void Logging::logMsg(QString strMsg, LoggingColor color, LoggingStyle style)
{
    QString strOut = QStringLiteral("[%1]: %2")
            .arg(QDateTime::currentDateTime().toString("HH:mm:ss.zzz"), colorString(strMsg, color, style));

    qInfo("%s", qPrintable(strOut));
}
