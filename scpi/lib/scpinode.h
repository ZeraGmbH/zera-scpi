#ifndef SCPINODE_H
#define SCPINODE_H

#include "scpiobject.h"
#include <QDomElement>
#include <QDomDocument>
#include <QStringList>
#include <QList>

class ScpiNode;
typedef std::shared_ptr<ScpiNode> ScpiNodePtr;

class ScpiNode
{
public:
    ScpiNode(const QString& scpiHeader, const ScpiObjectPtr &scpiObject);
    virtual ~ScpiNode();

    const ScpiObjectPtr &getScpiObject() const;
    void setScpiObject(const ScpiObjectPtr &pScpiObject);

    quint8 getType() const;
    const QString &getFullHeader() const;
    const QString &getShortHeader() const;

    ScpiNodePtr findChildShort(const QString &shortHeader) const;
    QList<ScpiNodePtr> findAllChildrenShort(const QString &shortHeader) const;
    ScpiNodePtr findChildFull(const QString &fullHeader) const;
    const ScpiNodePtr &parent() const;
    void removeChild(const ScpiNodePtr &child);
    void removeAllChildren();

    bool isEmpty() const;
    void add(const ScpiNodePtr &node, const ScpiNodePtr &parent);
    static void addNodeAndChildrenToXml(const ScpiNodePtr &node, QDomDocument &doc, QDomElement &rootElement, const QStringList &parentNames);
    static void addNodeAndChildrenToNameListFull(const ScpiNodePtr &node, const QStringList &parentNames, QList<QStringList> &scpiPathList);
    static QString createShortHeader(const QString &scpiHeader);

    static int getInstanceCount();

private:
    void adjustScpiHeaders(const QString &scpiHeader);
    static bool isLastShortAVowel(const QString &scpiHeader);
    int row() const;
    void removeRow(int row);
    static QDomElement createCmdTag(QStringList childNames, QDomDocument &doc, const QString &childName, const ScpiNodePtr childNode);
    static void addNodeSpecificAttributes(const ScpiNodePtr &childNode, QDomElement &cmdTag);
    static void addTypeAttribute(QDomElement &cmdTag, const ScpiNodePtr childNode, const QStringList &parentNames);

    ScpiObjectPtr m_scpiObject;
    QString m_sScpiHeaderShort;
    QString m_sScpiHeaderFull;

    ScpiNodePtr m_parent = nullptr;

    int m_row = 0;
    QList<ScpiNodePtr> m_children;

    static int m_instanceCount;
};


#endif // SCPINODE_H
