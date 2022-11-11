#include "scpiitem.h"

ScpiItem::ScpiItem()
{
}

ScpiItem::~ScpiItem()
{
    while(!m_rowItems.isEmpty())
        delete m_rowItems.takeFirst();
}

ScpiItem *ScpiItem::child(int row) const
{
    return m_rowItems[row];
}

ScpiItem *ScpiItem::parent() const
{
    return m_parent;
}

int ScpiItem::rowCount() const
{
    return m_rowItems.count();
}

int ScpiItem::row() const
{
    return m_row;
}

void ScpiItem::appendRow(ScpiItem *item)
{
    item->m_parent = this;
    item->m_row = m_rowItems.count();
    m_rowItems.append(item);
}

void ScpiItem::removeRow(int row)
{
    delete m_rowItems.takeAt(row);
    for(int row=0; row<m_rowItems.count(); ++row) {
        m_rowItems.at(row)->m_row = row;
    }
}
