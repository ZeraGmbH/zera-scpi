#include "scpi_p.h"
#include "scpinode.h"
#include "scpi.h"
#include <QDomDocument>
#include <QDomElement>
#include <QList>

cSCPIPrivate::cSCPIPrivate(const QString &interfaceName) :
    m_interfaceName(interfaceName),
    m_invisibleRootNode(QString(), nullptr)
{
}

void cSCPIPrivate::insertScpiCmd(const QStringList& parentNodeNames, cSCPIObject *pSCPIObject)
{
    ScpiNode *parentNode = &m_invisibleRootNode;
    const QStringList parentNamesCleaned = removeEmptyNodes(parentNodeNames);
    if(parentNamesCleaned.count() > 0)
        parentNode = findParentAndCreatePath(parentNamesCleaned);
    if(pSCPIObject)
        ScpiNodeStaticFunctions::addOrReplaceChild(parentNode, pSCPIObject);
}

void cSCPIPrivate::delSCPICmds(const QString &cmd)
{
    QStringList delNodePath;
    cParse parser;
    const QChar* pInput = cmd.data();
    do
        delNodePath.append(parser.GetKeyword(&pInput).toUpper());
    while(*pInput  == ':');
    findAndDeleteNode(delNodePath);
}

cSCPIObject* cSCPIPrivate::getSCPIObject(const QString& input)
{
    ScpiNode *childNode = ScpiNodeStaticFunctions::findNode(&m_invisibleRootNode, &m_Parser, (QChar*) input.data());
    if(childNode)
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

    ScpiNode::addNodeAndChildrenToXml(&m_invisibleRootNode, modelDoc, modelsTag, QStringList());

    sxml = modelDoc.toString();
}

ScpiNode *cSCPIPrivate::findParentAndCreatePath(const QStringList &parentNodePath)
{
    ScpiNode *parentNode = &m_invisibleRootNode;
    for(const QString &parentName : parentNodePath) {
        ScpiNode *child = parentNode->findChildFull(parentName.toUpper());
        if(!child) {
            child = ScpiNodeStaticFunctions::createNode(parentName, nullptr);
            parentNode->add(child);
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
            ScpiNode *childNode = parentNode->findChildFull(nodeName);
            parentNode = childNode;
            if(!parentNode)
                break;
        }
        if(parentNode)
            ScpiNodeStaticFunctions::delNodeAndEmptyParents(parentNode);
    }
}

QStringList cSCPIPrivate::removeEmptyNodes(const QStringList &parentNodeNames)
{
    QStringList parentNodeNamesCleaned = parentNodeNames;
    parentNodeNamesCleaned.removeAll("");
    if(parentNodeNamesCleaned.size() != parentNodeNames.size())
        qWarning("SCPI: Removed empty parent node names in: '%s'", qPrintable(parentNodeNames.join(":")));
    return parentNodeNamesCleaned;
}
