#include <QDateTime>
#include "logging.h"

Logging::Logging(QObject *parent)
    : QObject{parent}
{

}

QString Logging::colorString(QString strMsg, LoggingColor color)
{
    QString colorPre, colorPost;

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
        }
    }

    QString strOut = QStringLiteral("%2%3%4")
            .arg(colorPre,
                 strMsg,
                 colorPost);

    return strOut;
}

void Logging::logMsg(QString strMsg, LoggingColor color)
{
    QString strOut = QStringLiteral("[%1]: %2")
            .arg(QDateTime::currentDateTime().toString("HH:mm:ss.zzz"), colorString(strMsg, color));

    qInfo("%s", qPrintable(strOut));
}