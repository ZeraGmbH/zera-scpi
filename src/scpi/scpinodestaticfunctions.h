#ifndef SCPINODESTATICFUNCTIONS_H
#define SCPINODESTATICFUNCTIONS_H

#include "scpiobject.h"
#include "scpinode.h"

class ScpiNodeStaticFunctions
{
public:
    static ScpiNode* createNode(const QString &name, cSCPIObject *scpiObject);
    static void addOrReplaceChild(ScpiNode* node, cSCPIObject *pSCPIObject);
    static void delNodeAndEmptyParents(ScpiNode* delNode);
    static ScpiNode* findNode(ScpiNode *parentNode, cParse* parser, QChar* pInput);

    static bool isNodeTypeOnly(ScpiNode *node);
    static QString scpiTypeToString(quint8 scpiType);
    static QString makeValidXmlTag(QString xmlTag);
    static QString appendTypeString(QString typeInfo, const QString &infoAppend);
};

#endif // SCPINODESTATICFUNCTIONS_H
