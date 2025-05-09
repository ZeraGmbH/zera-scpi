#include "scpiobject.h"

int cSCPIObject::m_instanceCount = 0;

cSCPIObject::cSCPIObject()
{
    m_instanceCount++;
}

cSCPIObject::cSCPIObject(const QString &name, quint8 type) :
    m_sName(name),
    m_nType(type)
{
    m_instanceCount++;
}

cSCPIObject::~cSCPIObject()
{
    m_instanceCount--;
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

int cSCPIObject::getInstanceCount()
{
    return m_instanceCount;
}

const cSCPIObject::XmlKeyValueMap &cSCPIObject::getXmlAttibuteMap()
{
    return m_xmlAtttibuteMap;
}
