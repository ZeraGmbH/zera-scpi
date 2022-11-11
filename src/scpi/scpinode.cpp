#include "scpinode.h"
#include "scpi.h"

int cSCPINode::m_instanceCount = 0;

cSCPINode::cSCPINode()
{
    m_instanceCount++;
}

cSCPINode::cSCPINode(const QString& sNodeName, quint8 t, cSCPIObject* pSCPIObject) :
    m_pSCPIObject(pSCPIObject),
    m_sNodeName(sNodeName),
    m_nType(t)
{
    m_instanceCount++;
}

cSCPINode::~cSCPINode()
{
    while(!m_rowItems.isEmpty())
        delete m_rowItems.takeFirst();
    m_instanceCount--;
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

cSCPINode *cSCPINode::child(int row) const
{
    return m_rowItems[row];
}

cSCPINode *cSCPINode::parent() const
{
    return m_parent;
}

int cSCPINode::rowCount() const
{
    return m_rowItems.count();
}

int cSCPINode::row() const
{
    return m_row;
}

void cSCPINode::appendRow(cSCPINode *item)
{
    item->m_parent = this;
    item->m_row = m_rowItems.count();
    m_rowItems.append(item);
}

void cSCPINode::removeRow(int row)
{
    delete m_rowItems.takeAt(row);
    for(int row=0; row<m_rowItems.count(); ++row) {
        m_rowItems.at(row)->m_row = row;
    }
}

int cSCPINode::getInstanceCount()
{
    return m_instanceCount;
}
