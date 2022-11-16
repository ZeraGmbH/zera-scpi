#include "scpinode.h"
#include "scpi.h"

int ScpiNode::m_instanceCount = 0;

ScpiNode::ScpiNode()
{
    m_instanceCount++;
}

ScpiNode::ScpiNode(const QString& scpiHeader, cSCPIObject* pSCPIObject) :
    m_pScpiObject(pSCPIObject)
{
    adjustScpiHeaders(scpiHeader);
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

quint8 ScpiNode::getType() const
{
    if(m_pScpiObject)
        return m_pScpiObject->getType();
    return SCPI::isNode;
}

const QString &ScpiNode::getFullHeader() const
{
    return m_sScpiHeaderFull;
}

const QString &ScpiNode::getShortHeader() const
{
    return m_sScpiHeaderShort;
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

void ScpiNode::adjustScpiHeaders(QString scpiHeader)
{
    m_sScpiHeaderFull = scpiHeader.toUpper();
    if(m_sScpiHeaderFull.length() < 4)
        m_sScpiHeaderShort = m_sScpiHeaderFull;
    else if(isLastShortAVowel())
        m_sScpiHeaderShort = m_sScpiHeaderFull.left(3);
    else {
        m_sScpiHeaderShort = m_sScpiHeaderFull.left(4);
    }
}

bool ScpiNode::isLastShortAVowel()
{
    return QString("AEIOU").contains(m_sScpiHeaderFull.mid(3, 1));
}
