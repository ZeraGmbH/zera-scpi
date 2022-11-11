#include "scpiitemmodel.h"

ScpiItemModel::ScpiItemModel(QObject *parent) :
    QObject(parent),
    m_invisibleRootItem(new ScpiItem)
{
}

ScpiItemModel::~ScpiItemModel()
{
    delete m_invisibleRootItem;
}

ScpiItem *ScpiItemModel::invisibleRootItem() const
{
    return m_invisibleRootItem;
}
