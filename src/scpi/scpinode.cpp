#include "scpinode.h"

int ScpiNode::m_instanceCount = 0;

ScpiNode::ScpiNode()
{
    m_instanceCount++;
}

ScpiNode::ScpiNode(const QString& sNodeName, quint8 t, cSCPIObject* pSCPIObject) :
    m_pScpiObject(pSCPIObject),
    m_sNodeName(sNodeName),
    m_nType(t)
{
    m_instanceCount++;
}

ScpiNode::~ScpiNode()
{
    while(!m_rowItems.isEmpty())
        delete m_rowItems.takeFirst();
    m_instanceCount--;
}

cSCPIObject *ScpiNode::getScpiObject() const
{
    return m_pScpiObject;
}

void ScpiNode::setScpiObject(cSCPIObject *pScpiObject)
{
    m_pScpiObject = pScpiObject;
}

quint8 ScpiNode::getType()
{
    return m_nType;
}

void ScpiNode::setType(quint8 type)
{
    m_nType = type;
}

const QString &ScpiNode::getName()
{
    return m_sNodeName;
}

ScpiNode *ScpiNode::child(int row) const
{
    return m_rowItems[row];
}

ScpiNode *ScpiNode::parent() const
{
    return m_parent;
}

int ScpiNode::rowCount() const
{
    return m_rowItems.count();
}

int ScpiNode::row() const
{
    return m_row;
}

void ScpiNode::appendRow(ScpiNode *item)
{
    item->m_parent = this;
    item->m_row = m_rowItems.count();
    m_rowItems.append(item);
}

void ScpiNode::removeRow(int row)
{
    delete m_rowItems.takeAt(row);
    for(int row=0; row<m_rowItems.count(); ++row) {
        m_rowItems.at(row)->m_row = row;
    }
}

int ScpiNode::getInstanceCount()
{
    return m_instanceCount;
}
