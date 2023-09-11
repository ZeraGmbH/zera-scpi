#ifndef SCPINODE_H
#define SCPINODE_H

#include "scpiobject.h"
#include "parse.h"
#include <QDomElement>
#include <QDomDocument>
#include <QStringList>
#include <QList>

class ScpiNode
{
public:
    ScpiNode(const QString& scpiHeader, cSCPIObject* pSCPIObject);
    virtual ~ScpiNode();

    cSCPIObject* getScpiObject() const;
    void setScpiObject(cSCPIObject* pScpiObject);

    quint8 getType() const;
    const QString &getFullHeader() const;
    const QString &getShortHeader() const;

    ScpiNode *findChildShort(const QString &shortHeader) const;
    QList<ScpiNode *> findAllChildrenShort(const QString &shortHeader) const;
    ScpiNode *findChildFull(const QString &fullHeader) const;
    ScpiNode *parent() const;
    void removeChild(ScpiNode *child);

    bool isEmpty() const;
    void add(ScpiNode *node);
    static void addNodeAndChildrenToXml(const ScpiNode *node, QDomDocument &doc, QDomElement &rootElement, const QStringList parentNames);

    static int getInstanceCount();
private:
    void adjustScpiHeaders(QString scpiHeader);
    bool isLastShortAVowel();
    int row() const;
    void removeRow(int row);
    static QDomElement createCmdTag(QStringList childNames, QDomDocument &doc, QString childName, const ScpiNode *childNode);
    static void addNodeSpecificAttributes(const ScpiNode *childNode, QDomElement &cmdTag);
    static void addTypeAttribute(QDomElement &cmdTag, const ScpiNode *childNode, const QStringList parentNames);

    cSCPIObject* m_pScpiObject = nullptr;
    QString m_sScpiHeaderShort;
    QString m_sScpiHeaderFull;

    ScpiNode *m_parent = nullptr;

    int m_row = 0;
    QList<ScpiNode*> m_children;

    static int m_instanceCount;
};

#endif // SCPINODE_H
