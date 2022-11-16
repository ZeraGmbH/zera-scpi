#include "scpiobject.h"
#include "scpiobject_p.h"

cSCPIObject::cSCPIObject() :
    d_ptr( new cSCPIObjectPrivate)
{
}

cSCPIObject::cSCPIObject(const QString &name, quint8 type)
    :d_ptr(new cSCPIObjectPrivate)
{
    d_ptr->m_sName = name;
    d_ptr->m_nType = type;
}

cSCPIObject::~cSCPIObject()
{
    delete d_ptr;
}


const QString cSCPIObject::getName() const
{
    return d_ptr->m_sName.toUpper();
}


quint8 cSCPIObject::getType() const
{
    return d_ptr->m_nType;
}

void cSCPIObject::setXmlAttribute(const QString &key, const QString &value)
{
    d_ptr->m_xmlAtttibuteMap[key] = value;
}

const cSCPIObject::XmlKeyValueMap &cSCPIObject::getXmlAttibuteMap()
{
    return d_ptr->m_xmlAtttibuteMap;
}
