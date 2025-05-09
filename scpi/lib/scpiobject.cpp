#include "scpiobject.h"

int ScpiObject::m_instanceCount = 0;

ScpiObject::ScpiObject()
{
    m_instanceCount++;
}

ScpiObject::ScpiObject(const QString &name, quint8 type) :
    m_sName(name),
    m_nType(type)
{
    m_instanceCount++;
}

ScpiObject::~ScpiObject()
{
    m_instanceCount--;
}

const QString ScpiObject::getName() const
{
    return m_sName.toUpper();
}

quint8 ScpiObject::getType() const
{
    return m_nType;
}

void ScpiObject::setXmlAttribute(const QString &key, const QString &value)
{
    m_xmlAtttibuteMap[key] = value;
}

int ScpiObject::getInstanceCount()
{
    return m_instanceCount;
}

const ScpiObject::XmlKeyValueMap &ScpiObject::getXmlAttibuteMap()
{
    return m_xmlAtttibuteMap;
}
