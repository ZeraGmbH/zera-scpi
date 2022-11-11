#ifndef SCPIITEMMODEL_H
#define SCPIITEMMODEL_H

#include "scpiitem.h"
#include <QObject>

class ScpiItemModel : public QObject
{
    Q_OBJECT
public:
    explicit ScpiItemModel(QObject *parent = nullptr);
    ~ScpiItemModel();
    ScpiItem *invisibleRootItem() const;
    bool removeRow(int row);
private:
    ScpiItem *m_invisibleRootItem;
};

#endif // SCPIITEMMODEL_H
