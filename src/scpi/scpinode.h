#ifndef SCPINODE_H
#define SCPINODE_H

#include "scpiobject.h"

class ScpiNode
{
public:
    ScpiNode();
    ScpiNode(const QString& scpiHeader, cSCPIObject* pSCPIObject);
    virtual ~ScpiNode();

    cSCPIObject* getScpiObject() const;
    void setScpiObject(cSCPIObject*pScpiObject);

    quint8 getType() const;
    const QString &getFullHeader() const;
    const QString &getShortHeader() const;

    ScpiNode *child(int row) const;
    ScpiNode *parent() const;

    int rowCount() const;
    int row() const;
    void appendRow(ScpiNode *item);
    void removeRow(int row);

    static int getInstanceCount();
private:
    void adjustScpiHeaders(QString scpiHeader);
    bool isLastShortAVowel();

    cSCPIObject* m_pScpiObject = nullptr;
    QString m_sScpiHeaderShort;
    QString m_sScpiHeaderFull;

    ScpiNode *m_parent = nullptr;
    int m_row = 0;
    QList<ScpiNode*> m_rowItems;

    static int m_instanceCount;
};

#endif // SCPINODE_H
