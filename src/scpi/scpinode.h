#ifndef SCPINODE_H
#define SCPINODE_H

#include "scpiobject.h"
#include "parse.h"
#include <QDomElement>
#include <QDomDocument>
#include <QStringList>

class ScpiNode
{
public:
    ScpiNode();
    ScpiNode(const QString& scpiHeader, cSCPIObject* pSCPIObject);
    virtual ~ScpiNode();

    cSCPIObject* getScpiObject() const;
    void setScpiObject(cSCPIObject* pScpiObject);

    quint8 getType() const;
    const QString &getFullHeader() const;
    const QString &getShortHeader() const;

    ScpiNode *findChildShort(QString shortHeader) const;
    ScpiNode *findChildFull(QString fullHeader) const;
    ScpiNode *parent() const;
    void removeChild(ScpiNode *child);

    bool isEmpty();
    void add(ScpiNode *node);
    static void addNodeAndChildrenToXml(ScpiNode* node, QDomDocument &doc, QDomElement &rootElement, const QStringList parentNames);

    static int getInstanceCount();
private:
    void adjustScpiHeaders(QString scpiHeader);
    bool isLastShortAVowel();
    ScpiNode *child(int row) const;
    int rowCount() const;
    int row() const;
    void removeRow(int row);

    cSCPIObject* m_pScpiObject = nullptr;
    QString m_sScpiHeaderShort;
    QString m_sScpiHeaderFull;

    ScpiNode *m_parent = nullptr;

    int m_row = 0;
    QList<ScpiNode*> m_children;

    static int m_instanceCount;
};

#endif // SCPINODE_H
