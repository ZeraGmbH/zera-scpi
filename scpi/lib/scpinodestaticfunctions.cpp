#include "scpinodestaticfunctions.h"
#include "scpi.h"
#include <QString>
#include <qregularexpression.h>

ScpiNodePtr ScpiNodeStaticFunctions::createNode(const QString &name, ScpiObjectPtr scpiObject)
{
    return std::make_shared<ScpiNode>(name, scpiObject);
}

void ScpiNodeStaticFunctions::addOrReplaceChild(ScpiNodePtr node, ScpiObjectPtr pSCPIObject)
{
    QString fullHeader = pSCPIObject->getName();
    ScpiNodePtr childNode = node->findChildFull(fullHeader);
    if(childNode)
        childNode->setScpiObject(pSCPIObject);
    else
        node->add(createNode(pSCPIObject->getName(), pSCPIObject), node);
}

void ScpiNodeStaticFunctions::delNodeAndEmptyParents(ScpiNodePtr delNode)
{
    while(delNode->parent()) {
        ScpiNodePtr parent = delNode->parent();
        parent->removeChild(delNode);
        if(!parent->isEmpty())
            return;
        delNode = parent;
    }
}

ScpiNodePtr ScpiNodeStaticFunctions::findNode(const ScpiNodePtr parentNode, cParse *parser, const QChar *pInput)
{
    const QString &searchHeader = parser->GetKeyword(&pInput).toUpper();
    ScpiNodePtr childNode = parentNode->findChildFull(searchHeader);
    if(childNode) {
        if(*pInput == ':') // in case input is not parsed completely
            return findNode(childNode, parser, pInput);
        else
            return childNode;
    }
    const QList<ScpiNodePtr> shortChildren = parentNode->findAllChildrenShort(searchHeader);
    if(!shortChildren.isEmpty()) {
        if(*pInput == ':') { // in case input is not parsed completely
            for(ScpiNodePtr shortChild : shortChildren) {
                childNode = findNode(shortChild, parser, pInput);
                if(childNode)
                    return childNode;
            }
        }
        else // yes we mean it
            return shortChildren.first();
    }
    return nullptr;
}

bool ScpiNodeStaticFunctions::isNodeTypeOnly(const ScpiNodePtr node)
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
    xmlTag.remove(QRegularExpression("\\(.*\\)")); // in case of RPCs
    xmlTag.replace("*", "MUL");
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
