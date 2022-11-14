#include "scpi_p.h"
#include "scpistring.h"
#include "scpinode.h"
#include "scpi.h"
#include <QDomDocument>
#include <QDomElement>
#include <QList>

cSCPIPrivate::cSCPIPrivate(const QString &interfaceName) :
    m_interfaceName(interfaceName)
{
}

void cSCPIPrivate::insertScpiCmd(const QStringList& parentNodeNames, cSCPIObject *pSCPIObject)
{
    ScpiNode *parentNode = &m_invisibleRootNode;
    if(parentNodeNames.count() > 0 && parentNodeNames.at(0) != "")
        parentNode = findParentAndCreatePath(parentNodeNames);
    if(pSCPIObject)
        addOrReplaceChild(parentNode, pSCPIObject);
}

void cSCPIPrivate::delSCPICmds(const QString &cmd)
{
    QStringList delNodePath;
    cParse parser;
    QChar* pInput = (QChar*) cmd.data();
    do
        delNodePath.append(parser.GetKeyword(&pInput));
    while(*pInput  == ':');
    findAndDeleteNode(delNodePath);
}

cSCPIObject* cSCPIPrivate::getSCPIObject(const QString& input, bool caseSensitive)
{
    ScpiNode *childNode = nullptr;
    if(foundNode(&m_invisibleRootNode, &childNode, (QChar*) input.data(), caseSensitive))
        return childNode->getScpiObject();
    return nullptr;
}

void cSCPIPrivate::exportSCPIModelXML(QString& sxml)
{
    QDomDocument modelDoc("SCPIModel");

    QDomElement rootTag = modelDoc.createElement("MODELLIST");
    modelDoc.appendChild(rootTag);

    QDomElement deviceTag = modelDoc.createElement("DEVICE");
    rootTag.appendChild( deviceTag );
    QDomText t = modelDoc.createTextNode(m_interfaceName);
    deviceTag.appendChild(t);

    QDomElement modelsTag = modelDoc.createElement("MODELS");
    rootTag.appendChild(modelsTag);

    addNodeAndChildrenToXml(&m_invisibleRootNode, modelDoc, modelsTag, QStringList());

    sxml = modelDoc.toString();
}

void cSCPIPrivate::addOrReplaceChild(ScpiNode *node, cSCPIObject *pSCPIObject)
{
    bool overwriteExisting = false;
    QString sName = pSCPIObject->getName();
    ScpiNode *childNode;
    for(int row=0; row<node->rowCount(); row++) {
        childNode = node->child(row);
        if(childNode->getName() == sName) {
            overwriteExisting = true;
            break;
        }
    }
    if(overwriteExisting) {
        childNode->setScpiObject(pSCPIObject);
        childNode->setType(SCPI::isNode | pSCPIObject->getType());
    }
    else
        node->appendRow(createNode(pSCPIObject->getName(),pSCPIObject->getType(), pSCPIObject));
}

void cSCPIPrivate::addNodeAndChildrenToXml(ScpiNode *node, QDomDocument& doc, QDomElement &rootElement, const QStringList parentNames)
{
    for(int row = 0; row < node->rowCount(); row++) {
        ScpiNode *childNode = node->child(row);
        QString childName = childNode->getName();

        QDomElement cmdTag = doc.createElement(makeValidXmlTag(childName));

        QStringList childNames = parentNames;
        childNames.append(childName);
        if(!isNodeTypeOnly(childNode))
            cmdTag.setAttribute("ScpiPath", childNames.join(":"));
        cSCPIObject::XmlKeyValueMap xmlAtributes;
        if(childNode->getScpiObject())
            xmlAtributes = childNode->getScpiObject()->getXmlAttibuteMap();
        for(auto iter=xmlAtributes.constBegin(); iter!=xmlAtributes.constEnd(); ++iter)
            cmdTag.setAttribute(iter.key(), iter.value());

        QString typeInfo;
        if(parentNames.isEmpty())
            typeInfo = "Model,";
        typeInfo += scpiTypeToString(childNode->getType());
        cmdTag.setAttribute("Type", typeInfo);

        rootElement.appendChild(cmdTag);
        addNodeAndChildrenToXml(childNode, doc, cmdTag, childNames);
    }
}

ScpiNode *cSCPIPrivate::findParentAndCreatePath(const QStringList &parentNodePath)
{
    ScpiNode *parentNode = &m_invisibleRootNode;
    for(const QString &parentName : parentNodePath) {
        ScpiNode *child = nullptr;
        for(int row = 0; row < parentNode->rowCount(); row++) {
            ScpiNode *node = parentNode->child(row);
            if(node->getName() == parentName) {
                child = node;
                break;
            }
        }
        if(!child) {
            child  = createNode(parentName, SCPI::isNode, nullptr);
            parentNode->appendRow(child);
        }
        parentNode = child;
    }
    return parentNode;
}

void cSCPIPrivate::findAndDeleteNode(const QStringList &nodePath)
{
    if(nodePath.count() > 0 ) {
        ScpiNode *parentNode = &m_invisibleRootNode;
        for(const auto &nodeName: nodePath) {
            ScpiNode *childNode = nullptr;
            for(int row=0; row<parentNode->rowCount(); row++) {
                childNode = parentNode->child(row);
                if(childNode->getName() == nodeName)
                    break;
                else
                    childNode = nullptr;
            }
            parentNode = childNode;
            if(!parentNode)
                break;
        }
        if(parentNode)
            delNodeAndParents(parentNode);
    }
}

bool cSCPIPrivate::foundNode(ScpiNode *parentNode, ScpiNode **scpiChildNode, QChar *pInput, bool caseSensitive)
{
    bool found = false;
    int nrows = parentNode->rowCount();
    if( nrows > 0) { // keywords in next command level available
        cSCPIString keyw = (m_Parser.GetKeyword(&pInput)); // we fetch a new keyword
        if(!caseSensitive)
            keyw = keyw.toUpper();
        for(int row = 0; row < nrows; row++) {
            ScpiNode *childNode = parentNode->child(row);
            *scpiChildNode = childNode;
            QString s = (*scpiChildNode)->getName();
            if(!caseSensitive)
                s = s.toUpper(); // (s) is a node name from command tree
            if((found = (keyw == s))) {
                if(*pInput == ':') { // in case input is not parsed completely
                    ScpiNode* saveNode = *scpiChildNode;
                    QChar* saveInput = pInput;
                    found = found && foundNode(childNode, scpiChildNode, pInput, caseSensitive);
                    if(!found) {
                        *scpiChildNode = saveNode; // ifnot found we reset the childnode
                        pInput = saveInput; // and input pointer
                    }
                }
            }
            if(found)
                break;
        }
    }
    return found;
}

ScpiNode *cSCPIPrivate::createNode(const QString &name, quint8 type, cSCPIObject *scpiObject)
{
    return new ScpiNode(name, type, scpiObject);
}

void cSCPIPrivate::delNodeAndParents(ScpiNode *delNode)
{
    while(delNode->parent()) {
        int row = delNode->row();
        ScpiNode *parent = delNode->parent();
        parent->removeRow(row);
        if(hasStillChildren(parent))
            return;
        delNode = parent;
    }
}

bool cSCPIPrivate::hasStillChildren(ScpiNode *node)
{
    return node->rowCount() >= 1;
}

QString cSCPIPrivate::scpiTypeToString(quint8 scpiType)
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

QString cSCPIPrivate::appendTypeString(QString typeInfo, const QString &infoAppend)
{
    if(typeInfo.length() > 0)
        typeInfo += ",";
    return typeInfo + infoAppend;
}

bool cSCPIPrivate::isNodeTypeOnly(ScpiNode *node)
{
    return node->getType() == SCPI::isNode;
}

QString cSCPIPrivate::makeValidXmlTag(QString xmlTag)
{
    xmlTag.replace("*", "");
    if(xmlTag.count() > 0 && xmlTag[0].isNumber())
        xmlTag = "N_" + xmlTag;
    return xmlTag;
}
