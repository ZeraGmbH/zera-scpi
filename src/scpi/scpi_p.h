#ifndef SCPI_P_H
#define SCPI_P_H

#include "parse.h"
#include "scpiobject.h"
#include "scpinode.h"
#include <QStringList>
#include <QDomDocument>

class cSCPIPrivate
{
public:
    cSCPIPrivate(const QString& interfaceName);
    void insertScpiCmd(const QStringList& parentNodeNames, cSCPIObject* pSCPIObject);
    void delSCPICmds(const QString& cmd);
    cSCPIObject* getSCPIObject(const QString& input);
    void exportSCPIModelXML(QString &sxml);
private:
    void addOrReplaceChild(ScpiNode* node, cSCPIObject *pSCPIObject);
    void addNodeAndChildrenToXml(ScpiNode* node, QDomDocument &doc, QDomElement &rootElement, const QStringList parentNames);
    bool foundNode(ScpiNode *parentNode, ScpiNode** scpiChildNode, QChar* pInput);
    ScpiNode *findParentAndCreatePath(const QStringList& parentNodePath);
    void findAndDeleteNode(const QStringList &nodePath);
    static ScpiNode* createNode(const QString &name, cSCPIObject *scpiObject);
    static void delNodeAndParents(ScpiNode* delNode);
    static bool hasStillChildren(ScpiNode* node);
    static QString scpiTypeToString(quint8 scpiType);
    static QString appendTypeString(QString typeInfo, const QString &infoAppend);
    static bool isNodeTypeOnly(ScpiNode *node);
    static QString makeValidXmlTag(QString xmlTag);

    ScpiNode m_invisibleRootNode;
    QString m_interfaceName;
    cParse m_Parser;
};

#endif // SCPI_P_H
