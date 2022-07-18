#include "notificationstring.h"


cNotificationString::cNotificationString()
    :m_sValue("")
{
}


cNotificationString::cNotificationString(QString str)
    :m_sValue(str)
{
}


QString cNotificationString::getString()
{
    return m_sValue;
}


void cNotificationString::operator =(QString str)
{
    if (m_sValue != str)
        emit valueChanged();
    m_sValue = str;
}


void cNotificationString::forceTrigger()
{
    emit valueChanged();
}

