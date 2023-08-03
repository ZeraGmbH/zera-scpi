#include "messagedata.h"

CommandData::CommandData(QObject *parent)
    : QObject{parent}
{

}

CommandType CommandData::determineCommandType(QString cmd)
{
    QString cmdTrimmed = cmd.trimmed();

    if (cmdTrimmed.endsWith(";"))
        return CommandType::CMD;
    else if (cmdTrimmed.endsWith("?"))
        return CommandType::QUERY;
    else if (cmdTrimmed.isEmpty())
        return CommandType::EMPTY;
    else
        return CommandType::UNKNOWN;
}

MessageData::MessageData(QObject *parent)
    : QObject{parent}
{

}
