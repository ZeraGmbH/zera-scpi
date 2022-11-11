#ifndef SCPIITEM_H
#define SCPIITEM_H

#include <QList>

class ScpiItem
{
public:
    ScpiItem();
    virtual ~ScpiItem();

    ScpiItem *child(int row) const;
    ScpiItem *parent() const;

    int rowCount() const;
    int row() const;
    void appendRow(ScpiItem *item);
    void removeRow(int row);

private:
    ScpiItem *m_parent = nullptr;
    int m_row = 0;
    QList<ScpiItem*> m_rowItems;
};

#endif // SCPIITEM_H
