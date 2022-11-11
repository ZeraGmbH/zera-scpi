#ifndef SCPINODE_H
#define SCPINODE_H

#include "scpiobject.h"

class cSCPINode
{
public:
    cSCPIObject* m_pSCPIObject; // !!!!!!!!

    cSCPINode();
    cSCPINode(const QString& sNodeName, quint8 t, cSCPIObject* pSCPIObject);
    virtual ~cSCPINode();

    quint8 getType();
    void setType(quint8 type);
    const QString &getName();

    cSCPINode *child(int row) const;
    cSCPINode *parent() const;

    int rowCount() const;
    int row() const;
    void appendRow(cSCPINode *item);
    void removeRow(int row);

    static int getInstanceCount();
private:
    QString m_sNodeName;
    quint8 m_nType;

    cSCPINode *m_parent = nullptr;
    int m_row = 0;
    QList<cSCPINode*> m_rowItems;

    static int m_instanceCount;
};

#endif // SCPINODE_H
