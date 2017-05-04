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


int cSCPINode::type()
{
    return QStandardItem::UserType;
}


QVariant cSCPINode::data(int role) const
{
    QString ttip;

    switch (role)
    {
    case Qt::DisplayRole:
        return m_sNodeName;
    case Qt::ToolTipRole:
        ttip = "";

        if (m_nType & SCPI::isNode)
            ttip = SCPI::scpiNodeType[SCPI::Node];

        if (m_nType & SCPI::isQuery)
        {
            if (ttip.length() > 0)
                ttip += ",";
            ttip += SCPI::scpiNodeType[SCPI::Query];
        }

        if (m_nType & SCPI::isCmd)
        {
            if (ttip.length() > 0)
                ttip += ",";
            ttip += SCPI::scpiNodeType[SCPI::Cmd];
        }

        if (m_nType & SCPI::isCmdwP)
        {
            if (ttip.length() > 0)
                ttip += ",";
            ttip += SCPI::scpiNodeType[SCPI::CmdwP];
        }

        return ttip;
    }
    return QVariant();
}

