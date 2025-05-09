#include "scpi_p.h"
#include "scpinode.h"
#include "scpi.h"
#include "scpinodestaticfunctions.h"
#include <QDomDocument>
#include <QDomElement>
#include <QList>
#include <QSet>

cSCPIPrivate::cSCPIPrivate() :
    m_invisibleRootNode(std::make_shared<ScpiNode>(QString(), nullptr))
{
}

cSCPIPrivate::~cSCPIPrivate()
{
    m_invisibleRootNode->removeAllChildren();
}

void cSCPIPrivate::insertScpiCmd(const QStringList& parentNodeNames, ScpiObjectPtr pSCPIObject)
{
    ScpiNodePtr parentNode = m_invisibleRootNode;
    const QStringList parentNamesCleaned = removeEmptyNodes(parentNodeNames);
    int nodesRemoved = parentNodeNames.size() - parentNamesCleaned.size();
    if(nodesRemoved)
        qWarning("SCPI: Removed %i empty parent node names in: '%s:%s'",
                 nodesRemoved,
                 qPrintable(parentNodeNames.join(":")),
                 qPrintable(pSCPIObject->getName()));
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

ScpiObjectPtr cSCPIPrivate::getSCPIObject(const QString& input)
{
    ScpiNodePtr childNode = ScpiNodeStaticFunctions::findNode(m_invisibleRootNode, &m_Parser, (QChar*) input.data());
    if(childNode)
        return childNode->getScpiObject();
    return nullptr;
}

void cSCPIPrivate::exportSCPIModelXML(QString& sxml, QMap<QString, QString> modelListBaseEntry)
{
    QDomDocument modelDoc("SCPIModel");

    QDomElement rootTag = modelDoc.createElement("MODELLIST");
    modelDoc.appendChild(rootTag);

    for(auto iter=modelListBaseEntry.constBegin(); iter!=modelListBaseEntry.constEnd(); iter++) {
        QDomElement elem = modelDoc.createElement(iter.key());
        rootTag.appendChild(elem);
        QDomText domText = modelDoc.createTextNode(iter.value());
        elem.appendChild(domText);
    }

    QDomElement modelsTag = modelDoc.createElement("MODELS");
    rootTag.appendChild(modelsTag);

    ScpiNode::addNodeAndChildrenToXml(m_invisibleRootNode, modelDoc, modelsTag, QStringList());

    sxml = modelDoc.toString();
}

void cSCPIPrivate::createFullNonNodeNameList(QList<QStringList> &childNameList)
{
    ScpiNode::addNodeAndChildrenToNameListFull(m_invisibleRootNode, QStringList(), childNameList);
}

ScpiAmbiguityMap cSCPIPrivate::checkAmbiguousShortNames(ScpiAmbiguityIgnoreFunction ignoreFunction)
{
    QList<QStringList> childNameListFull;
    createFullNonNodeNameList(childNameListFull);
    QList<QStringList> childNameListShort = makeShortNameList(childNameListFull);
    Q_ASSERT(childNameListFull.size() == childNameListShort.size());

    return ignoreFunction(filterAmbigous(createAllShortLongListMap(childNameListFull, childNameListShort)));
}

ScpiNodePtr cSCPIPrivate::findParentAndCreatePath(const QStringList &parentNodePath)
{
    ScpiNodePtr parentNode = m_invisibleRootNode;
    for(const QString &parentName : parentNodePath) {
        ScpiNodePtr child = parentNode->findChildFull(parentName.toUpper());
        if(!child) {
            child = ScpiNodeStaticFunctions::createNode(parentName, nullptr);
            parentNode->add(child, parentNode);
        }
        parentNode = child;
    }
    return parentNode;
}

void cSCPIPrivate::findAndDeleteNode(const QStringList &nodePath)
{
    if(nodePath.count() > 0 ) {
        ScpiNodePtr parentNode = m_invisibleRootNode;
        for(const auto &nodeName: nodePath) {
            ScpiNodePtr childNode = parentNode->findChildFull(nodeName);
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
    return parentNodeNamesCleaned;
}

QList<QStringList> cSCPIPrivate::makeShortNameList(QList<QStringList> childNameListFull)
{
    QList<QStringList> childNameListShort;
    for(const auto &fullNameList : qAsConst(childNameListFull)) {
        QStringList namesShort;
        for(const auto &fullName : fullNameList)
            namesShort.append(ScpiNode::createShortHeader(fullName));
        childNameListShort.append(namesShort);
    }
    return childNameListShort;
}

ScpiAmbiguityMap cSCPIPrivate::createAllShortLongListMap(QList<QStringList> childNameListFull, QList<QStringList> childNameListShort)
{
    ScpiAmbiguityMap allShortLong;
    for(int i = 0; i<childNameListShort.count(); i++)
        allShortLong[childNameListShort[i].join(":")].append(childNameListFull[i].join(":"));
    return allShortLong;
}

ScpiAmbiguityMap cSCPIPrivate::filterAmbigous(ScpiAmbiguityMap allShortLong)
{
    ScpiAmbiguityMap ambigouosShortMap;
    for(auto iter=allShortLong.constBegin(); iter!=allShortLong.constEnd(); ++iter)
        if(iter.value().count() > 1)
            ambigouosShortMap[iter.key()] = iter.value();
    return ambigouosShortMap;
}
