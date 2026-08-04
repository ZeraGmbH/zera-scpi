#include "scpinode.h"
#include "scpinodestaticfunctions.h"
#include "scpi.h"

int ScpiNode::m_instanceCount = 0;

ScpiNode::ScpiNode(const QString& scpiHeader, const ScpiObjectPtr &scpiObject) :
    m_scpiObject(scpiObject),
    m_sScpiHeaderFull(scpiHeader.toUpper()),
    m_sScpiHeaderShort(createShortHeader(m_sScpiHeaderFull))
{
    m_instanceCount++;
}

ScpiNode::~ScpiNode()
{
    m_instanceCount--;
}

const ScpiObjectPtr &ScpiNode::getScpiObject() const
{
    return m_scpiObject;
}

void ScpiNode::setScpiObject(const ScpiObjectPtr &pScpiObject)
{
    m_scpiObject = pScpiObject;
}

quint8 ScpiNode::getType() const
{
    if(m_scpiObject)
        return m_scpiObject->getType();
    return SCPI::isNode;
}

const QString &ScpiNode::getFullHeader() const
{
    return m_sScpiHeaderFull;
}

const QString &ScpiNode::getShortHeader() const
{
    return m_sScpiHeaderShort;
}

ScpiNodePtr ScpiNode::findChildShort(const QString &shortHeader) const
{
    auto iter = m_childrenShortNames.constFind(shortHeader);
    if (iter != m_childrenShortNames.constEnd())
        return iter.value().first();
    return nullptr;
}

QList<ScpiNodePtr> ScpiNode::findAllChildrenShort(const QString &shortHeader) const
{
    auto iter = m_childrenShortNames.constFind(shortHeader);
    if (iter != m_childrenShortNames.constEnd())
        return iter.value();
    return QList<ScpiNodePtr>();
}

ScpiNodePtr ScpiNode::findChildFull(const QString &fullHeader) const
{
    auto iter = m_childrenFullNames.constFind(fullHeader);
    if (iter != m_childrenFullNames.constEnd())
        return iter.value();
    return nullptr;
}

const ScpiNodePtr &ScpiNode::parent() const
{
    return m_parent;
}

void ScpiNode::removeChild(const ScpiNodePtr &child)
{
    m_childrenFullNames.remove(child->getFullHeader());

    const QString &shortHeader = child->getShortHeader();
    auto iter = m_childrenShortNames.find(shortHeader);
    if (iter != m_childrenShortNames.end()) {
        iter.value().removeAll(child);
        if (iter.value().isEmpty())
            m_childrenShortNames.remove(shortHeader);
    }

    child->removeAllChildren();
}

void ScpiNode::removeAllChildren()
{
    for (auto iter = m_childrenFullNames.begin(); iter != m_childrenFullNames.end(); ++iter)
        iter.value()->removeAllChildren();
    m_childrenFullNames.clear();
    m_childrenShortNames.clear();
}

bool ScpiNode::isEmpty() const
{
    return m_childrenFullNames.isEmpty();
}

void ScpiNode::add(const ScpiNodePtr &node, const ScpiNodePtr &parentNode)
{
    node->m_parent = parentNode;
    m_childrenFullNames[node->getFullHeader()] = node;
    m_childrenShortNames[node->getShortHeader()].append(node);
}

void ScpiNode::addNodeSpecificAttributes(const ScpiNodePtr &childNode, QDomElement &cmdTag)
{
    ScpiObject::XmlKeyValueMap xmlAtributes;
    if(childNode->getScpiObject())
        xmlAtributes = childNode->getScpiObject()->getXmlAttibuteMap();
    for(auto attIter=xmlAtributes.constBegin(); attIter!=xmlAtributes.constEnd(); ++attIter)
        cmdTag.setAttribute(attIter.key(), attIter.value());
}

QDomElement ScpiNode::createCmdTag(QStringList childNames, QDomDocument &doc, const QString &childName, const ScpiNodePtr childNode)
{
    QDomElement cmdTag = doc.createElement(ScpiNodeStaticFunctions::makeValidXmlTag(childName));
    if(!ScpiNodeStaticFunctions::isNodeTypeOnly(childNode))
        cmdTag.setAttribute("ScpiPath", childNames.join(":"));

    return cmdTag;
}

void ScpiNode::addTypeAttribute(QDomElement &cmdTag, const ScpiNodePtr childNode, const QStringList &parentNames)
{
    QString typeInfo;
    if(parentNames.isEmpty())
        typeInfo = "Model,";
    typeInfo += ScpiNodeStaticFunctions::scpiTypeToString(childNode->getType());
    cmdTag.setAttribute("Type", typeInfo);
}

void ScpiNode::addNodeAndChildrenToXml(const ScpiNodePtr &node, QDomDocument &doc, QDomElement &rootElement, const QStringList &parentNames)
{
    for(auto iter=node->m_childrenFullNames.constBegin(); iter!=node->m_childrenFullNames.constEnd(); iter++) {
        const ScpiNodePtr childNode = iter.value();
        QString childNameFull = childNode->getFullHeader();
        QStringList childNameListFull = parentNames + QStringList(childNameFull);

        QDomElement cmdTag = createCmdTag(childNameListFull, doc, childNameFull, childNode);
        addNodeSpecificAttributes(childNode, cmdTag);
        addTypeAttribute(cmdTag, childNode, parentNames);
        rootElement.appendChild(cmdTag);

        addNodeAndChildrenToXml(childNode, doc, cmdTag, childNameListFull);
    }
}

void ScpiNode::addNodeAndChildrenToNameListFull(const ScpiNodePtr &node, const QStringList &parentNames, QList<QStringList> &scpiPathList)
{
    for(auto iter=node->m_childrenFullNames.constBegin(); iter!=node->m_childrenFullNames.constEnd(); iter++) {
        const ScpiNodePtr childNode = iter.value();
        QString childName = childNode->getFullHeader();
        QStringList childNameList = parentNames + QStringList(childName);
        if(!ScpiNodeStaticFunctions::isNodeTypeOnly(childNode))
            scpiPathList.append(childNameList);
        addNodeAndChildrenToNameListFull(childNode, childNameList, scpiPathList);
    }
}

int ScpiNode::getInstanceCount()
{
    return m_instanceCount;
}

QString ScpiNode::createShortHeader(const QString &scpiHeader)
{
    QString scpiHeaderShort;
    if(scpiHeader.length() < 4)
        scpiHeaderShort = scpiHeader;
    else if(isLastShortAVowel(scpiHeader))
        scpiHeaderShort = scpiHeader.left(3);
    else
        scpiHeaderShort = scpiHeader.left(4);
    return scpiHeaderShort;
}

bool ScpiNode::isLastShortAVowel(const QString &scpiHeader)
{
    return QString("AEIOU").contains(scpiHeader.mid(3, 1));
}
