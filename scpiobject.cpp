#include "scpiobject.h"
#include "scpiobject_p.h"

cSCPIObject::cSCPIObject()
    :d_ptr( new cSCPIObjectPrivate)
{
}

cSCPIObject::cSCPIObject(const QString &name, quint8 type)
    :d_ptr(new cSCPIObjectPrivate)
{
    d_ptr->m_sName = name;
    d_ptr->m_nType = type;
}


const QString cSCPIObject::getName()
{
    return d_ptr->m_sName;
}


void cSCPIObject::setName(const QString& name)
{
    d_ptr->m_sName = name;
}


quint8 cSCPIObject::getType()
{
    return d_ptr->m_nType;
}


void cSCPIObject::setType(const quint8 type)
{
    d_ptr->m_nType = type;
}
