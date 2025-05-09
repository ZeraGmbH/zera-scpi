#ifndef SCPINODESTATICFUNCTIONS_H
#define SCPINODESTATICFUNCTIONS_H

#include "scpiobject.h"
#include "scpinode.h"
#include "parse.h"

class ScpiNodeStaticFunctions
{
public:
    static ScpiNodePtr createNode(const QString &name, ScpiObjectPtr scpiObject);
    static void addOrReplaceChild(ScpiNodePtr node, ScpiObjectPtr pSCPIObject);
    static void delNodeAndEmptyParents(ScpiNodePtr delNode);
    static ScpiNodePtr findNode(const ScpiNodePtr parentNode, cParse* parser, const QChar *pInput);

    static bool isNodeTypeOnly(const ScpiNodePtr node);
    static QString scpiTypeToString(const quint8 scpiType);
    static QString makeValidXmlTag(QString xmlTag);
    static QString appendTypeString(QString typeInfo, const QString &infoAppend);
};

#endif // SCPINODESTATICFUNCTIONS_H
