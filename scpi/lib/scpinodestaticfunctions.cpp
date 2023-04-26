#include "scpinodestaticfunctions.h"
#include "scpi.h"
#include <QString>

ScpiNode *ScpiNodeStaticFunctions::createNode(const QString &name, cSCPIObject *scpiObject)
{
    return new ScpiNode(name, scpiObject);
}

void ScpiNodeStaticFunctions::addOrReplaceChild(ScpiNode *node, cSCPIObject *pSCPIObject)
{
    QString fullHeader = pSCPIObject->getName();
    ScpiNode *childNode = node->findChildFull(fullHeader);
    if(childNode)
        childNode->setScpiObject(pSCPIObject);
    else
        node->add(createNode(pSCPIObject->getName(), pSCPIObject));
}

void ScpiNodeStaticFunctions::delNodeAndEmptyParents(ScpiNode *delNode)
{
    while(delNode->parent()) {
        ScpiNode *parent = delNode->parent();
        parent->removeChild(delNode);
        if(!parent->isEmpty())
            return;
        delNode = parent;
    }
}

ScpiNode *ScpiNodeStaticFunctions::findNode(const ScpiNode *parentNode, cParse *parser, const QChar *pInput)
{
    QString searchHeader = parser->GetKeyword(&pInput).toUpper();
    ScpiNode *childNode = parentNode->findChildFull(searchHeader);
    if(childNode) {
        if(*pInput == ':') // in case input is not parsed completely
            return findNode(childNode, parser, pInput);
        else
            return childNode;
    }
    QList<ScpiNode*> shortChildren = parentNode->findAllChildrenShort(searchHeader);
    if(!shortChildren.isEmpty()) {
        if(*pInput == ':') { // in case input is not parsed completely
            while(!shortChildren.isEmpty()) {
                ScpiNode* shorChild = shortChildren.takeFirst();
                childNode = findNode(shorChild, parser, pInput);
                if(childNode)
                    return childNode;
            }
        }
        else // yes we mean it
            return shortChildren.first();
    }
    return nullptr;
}

bool ScpiNodeStaticFunctions::isNodeTypeOnly(const ScpiNode *node)
{
    return node->getType() == SCPI::isNode;
}

QString ScpiNodeStaticFunctions::scpiTypeToString(const quint8 scpiType)
{
    static const QString scpiNodeType[] = {
        "Node",
        "Query",
        "Command",
        "Command+Par"
    };
    QString typeInfo;
    if(scpiType & SCPI::isNode)
        typeInfo = appendTypeString(typeInfo, scpiNodeType[SCPI::Node]);
    if(scpiType & SCPI::isQuery)
        typeInfo = appendTypeString(typeInfo, scpiNodeType[SCPI::Query]);
    if(scpiType & SCPI::isCmd)
        typeInfo = appendTypeString(typeInfo, scpiNodeType[SCPI::Cmd]);
    if(scpiType & SCPI::isCmdwP)
        typeInfo = appendTypeString(typeInfo, scpiNodeType[SCPI::CmdwP]);
    return typeInfo;
}

QString ScpiNodeStaticFunctions::makeValidXmlTag(QString xmlTag)
{
    xmlTag.replace("*", "");
    xmlTag.replace("+", "PLUS");
    xmlTag.replace("-", "MINUS");
    if(xmlTag.count() > 0 && xmlTag[0].isNumber())
        xmlTag = "N_" + xmlTag;
    return xmlTag;
}

QString ScpiNodeStaticFunctions::appendTypeString(QString typeInfo, const QString &infoAppend)
{
    if(typeInfo.length() > 0)
        typeInfo += ",";
    return typeInfo + infoAppend;
}
