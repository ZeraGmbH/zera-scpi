#include "scpiobject.h"

cSCPIObject::cSCPIObject()
{
}

cSCPIObject::cSCPIObject(const QString &name, quint8 type) :
    m_sName(name),
    m_nType(type)
{
}

cSCPIObject::~cSCPIObject()
{
}

const QString cSCPIObject::getName() const
{
    return m_sName.toUpper();
}

quint8 cSCPIObject::getType() const
{
    return m_nType;
}

void cSCPIObject::setXmlAttribute(const QString &key, const QString &value)
{
    m_xmlAtttibuteMap[key] = value;
}

const cSCPIObject::XmlKeyValueMap &cSCPIObject::getXmlAttibuteMap()
{
    return m_xmlAtttibuteMap;
}
