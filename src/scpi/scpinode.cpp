#include "scpinode.h"
#include "scpi.h"

cSCPINode::cSCPINode(const QString& sNodeName, quint8 t, cSCPIObject* pSCPIObject)
    :m_pSCPIObject(pSCPIObject), m_sNodeName(sNodeName), m_nType(t)
{
}


quint8 cSCPINode::getType()
{
    return m_nType;
}


void cSCPINode::setType(quint8 type)
{
    m_nType = type;
}

const QString &cSCPINode::getName()
{
    return m_sNodeName;
}
