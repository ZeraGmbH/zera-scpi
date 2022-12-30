#include "notificationvalue.h"

cNotificationValue::cNotificationValue()
    :m_nValue(0)
{
}

cNotificationValue::cNotificationValue(quint32 val)
    :m_nValue(val)
{
}

quint32 cNotificationValue::getValue()
{
    return m_nValue;
}

void cNotificationValue::setValue(quint32 val)
{
    quint32 edge;
    if ((edge = ((m_nValue ^ val) & val)) > 0)
        emit risingEdge(edge);
    m_nValue |= val;
}

void cNotificationValue::clrValue(quint32 val)
{
    m_nValue = m_nValue & ~val;
}
