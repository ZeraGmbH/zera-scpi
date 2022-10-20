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


const QString cSCPIObject::getName()
{
    return d_ptr->m_sName;
}


quint8 cSCPIObject::getType()
{
    return d_ptr->m_nType;
}

void cSCPIObject::setDescription(const QString &desc)
{
    d_ptr->m_description = desc;
}

const QString &cSCPIObject::getDescription()
{
    return d_ptr->m_description;
}
