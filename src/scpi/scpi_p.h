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
    ScpiNode* createNode(const QString &name, quint8 type, cSCPIObject *scpiObject);
    void delItemAndParents(ScpiNode* Item);
    void appendScpiNodeXmlInfo(ScpiNode* rootItem, QDomDocument &doc, QDomElement &rootElement, const QStringList parentNames);
    static QString scpiTypeToString(quint8 scpiType);
    ScpiNode *findOrCreateChildParentItem(ScpiNode *parentItem, const QStringList& parentnodeNames);
    bool foundItem(ScpiNode *parentItem, ScpiNode** scpiChildItem, QChar* pInput, bool caseSensitive);
    bool isNodeTypeOnly(ScpiNode *item);
    QString makeValidXmlTag(QString xmlTag);

    ScpiNode m_invisibleRootItem;
    QString m_interfaceName;
    cParse m_Parser;
};

#endif // SCPI_P_H
