#ifndef SCPINODE_H
#define SCPINODE_H

#include "scpiobject.h"

class ScpiNode
{
public:
    ScpiNode();
    ScpiNode(const QString& sNodeName, quint8 t, cSCPIObject* pSCPIObject);
    virtual ~ScpiNode();

    cSCPIObject* getScpiObject() const;
    void setScpiObject(cSCPIObject*pScpiObject);

    quint8 getType();
    void setType(quint8 type);
    const QString &getName();

    ScpiNode *child(int row) const;
    ScpiNode *parent() const;

    int rowCount() const;
    int row() const;
    void appendRow(ScpiNode *item);
    void removeRow(int row);

    static int getInstanceCount();
private:
    cSCPIObject* m_pScpiObject;
    QString m_sNodeName;
    quint8 m_nType;

    ScpiNode *m_parent = nullptr;
    int m_row = 0;
    QList<ScpiNode*> m_rowItems;

    static int m_instanceCount;
};

#endif // SCPINODE_H
