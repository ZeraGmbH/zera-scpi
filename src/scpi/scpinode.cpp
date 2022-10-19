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

QString cSCPINode::getTypeInfo()
{
    QString typeInfo;
    if (m_nType & SCPI::isNode)
        typeInfo = SCPI::scpiNodeType[SCPI::Node];

    if (m_nType & SCPI::isQuery) {
        if (typeInfo.length() > 0)
            typeInfo += ",";
        typeInfo += SCPI::scpiNodeType[SCPI::Query];
    }

    if (m_nType & SCPI::isCmd) {
        if (typeInfo.length() > 0)
            typeInfo += ",";
        typeInfo += SCPI::scpiNodeType[SCPI::Cmd];
    }

    if (m_nType & SCPI::isCmdwP) {
        if (typeInfo.length() > 0)
            typeInfo += ",";
        typeInfo += SCPI::scpiNodeType[SCPI::CmdwP];
    }
    return typeInfo;
}
