#include "scpiitem.h"

ScpiItem::ScpiItem()
{
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
    m_rowItems.removeAt(row);
}
