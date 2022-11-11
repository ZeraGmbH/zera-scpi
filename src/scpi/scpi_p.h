#ifndef SCPI_P_H
#define SCPI_P_H

#include "scpistring.h"
#include "parse.h"
#include "scpiobject.h"
#include "scpinode.h"
#include <QStringList>
#include <QDomDocument>

class cSCPIPrivate
{
public:
    cSCPIPrivate(const QString& interfaceName);
    void insertScpiCmd(const QStringList& parentnodeNames, cSCPIObject* pSCPIObject);
    void delSCPICmds(const QString& cmd);
    cSCPIObject* getSCPIObject(const QString& input, bool caseSensitive = false);
    void exportSCPIModelXML(QString &sxml);

private:
    cSCPINode* createNode(const QString &name, quint8 type, cSCPIObject *scpiObject);
    void delItemAndParents(cSCPINode* Item);
    void appendScpiNodeXmlInfo(cSCPINode* rootItem, QDomDocument &doc, QDomElement &rootElement, const QStringList parentNames);
    static QString scpiTypeToString(quint8 scpiType);
    cSCPINode *findOrCreateChildParentItem(cSCPINode *parentItem, const QStringList& parentnodeNames);
    bool foundItem(cSCPINode *parentItem, cSCPINode** scpiChildItem, QChar* pInput, bool caseSensitive);
    bool isNodeTypeOnly(cSCPINode *item);
    QString makeValidXmlTag(QString xmlTag);

    cSCPINode m_invisibleRootItem;
    QString m_interfaceName;
    cParse m_Parser;
};

#endif // SCPI_P_H
