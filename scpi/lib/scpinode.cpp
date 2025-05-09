#include "scpinode.h"
#include "scpinodestaticfunctions.h"
#include "scpi.h"

int ScpiNode::m_instanceCount = 0;

ScpiNode::ScpiNode(const QString& scpiHeader, ScpiObjectPtr pSCPIObject) :
    m_pScpiObject(pSCPIObject)
{
    adjustScpiHeaders(scpiHeader);
    m_instanceCount++;
}

ScpiNode::~ScpiNode()
{
    m_instanceCount--;
}

ScpiObjectPtr ScpiNode::getScpiObject() const
{
    return m_pScpiObject;
}

void ScpiNode::setScpiObject(ScpiObjectPtr pScpiObject)
{
    m_pScpiObject = pScpiObject;
}

quint8 ScpiNode::getType() const
{
    if(m_pScpiObject)
        return m_pScpiObject->getType();
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
    for(auto iter=m_children.constBegin(); iter!=m_children.constEnd(); iter++) {
        if((*iter)->getShortHeader() == shortHeader) {
            return *iter;
        }
    }
    return nullptr;
}

QList<ScpiNodePtr> ScpiNode::findAllChildrenShort(const QString &shortHeader) const
{
    QList<ScpiNodePtr> found;
    for(auto iter=m_children.constBegin(); iter!=m_children.constEnd(); iter++) {
        if((*iter)->getShortHeader() == shortHeader) {
            found.append(*iter);
        }
    }
    return found;
}

ScpiNodePtr ScpiNode::findChildFull(const QString &fullHeader) const
{
    for(auto iter=m_children.constBegin(); iter!=m_children.constEnd(); iter++) {
        if((*iter)->getFullHeader() == fullHeader) {
            return *iter;
        }
    }
    return nullptr;
}

ScpiNodePtr ScpiNode::parent() const
{
    return m_parent;
}

void ScpiNode::removeChild(ScpiNodePtr child)
{
    removeRow(child->row());
    child->removeAllChildren();
}

void ScpiNode::removeAllChildren()
{
    for (ScpiNodePtr child : m_children)
        child->removeAllChildren();
    m_children.clear();
}

bool ScpiNode::isEmpty() const
{
    return m_children.isEmpty();
}

int ScpiNode::row() const
{
    return m_row;
}

void ScpiNode::add(ScpiNodePtr node, ScpiNodePtr parent)
{
    node->m_parent = parent;
    node->m_row = m_children.count();
    m_children.append(node);
}

void ScpiNode::addNodeSpecificAttributes(const ScpiNodePtr childNode, QDomElement &cmdTag)
{
    ScpiObject::XmlKeyValueMap xmlAtributes;
    if(childNode->getScpiObject())
        xmlAtributes = childNode->getScpiObject()->getXmlAttibuteMap();
    for(auto attIter=xmlAtributes.constBegin(); attIter!=xmlAtributes.constEnd(); ++attIter)
        cmdTag.setAttribute(attIter.key(), attIter.value());
}

QDomElement ScpiNode::createCmdTag(QStringList childNames, QDomDocument &doc, QString childName, const ScpiNodePtr childNode)
{
    QDomElement cmdTag = doc.createElement(ScpiNodeStaticFunctions::makeValidXmlTag(childName));
    if(!ScpiNodeStaticFunctions::isNodeTypeOnly(childNode))
        cmdTag.setAttribute("ScpiPath", childNames.join(":"));

    return cmdTag;
}

void ScpiNode::addTypeAttribute(QDomElement &cmdTag, const ScpiNodePtr childNode, const QStringList parentNames)
{
    QString typeInfo;
    if(parentNames.isEmpty())
        typeInfo = "Model,";
    typeInfo += ScpiNodeStaticFunctions::scpiTypeToString(childNode->getType());
    cmdTag.setAttribute("Type", typeInfo);
}

void ScpiNode::addNodeAndChildrenToXml(const ScpiNodePtr node, QDomDocument &doc, QDomElement &rootElement, const QStringList parentNames)
{
    for(auto iter=node->m_children.constBegin(); iter!=node->m_children.constEnd(); iter++) {
        const ScpiNodePtr childNode = *iter;
        QString childNameFull = childNode->getFullHeader();
        QStringList childNameListFull = parentNames + QStringList(childNameFull);

        QDomElement cmdTag = createCmdTag(childNameListFull, doc, childNameFull, childNode);
        addNodeSpecificAttributes(childNode, cmdTag);
        addTypeAttribute(cmdTag, childNode, parentNames);
        rootElement.appendChild(cmdTag);

        addNodeAndChildrenToXml(childNode, doc, cmdTag, childNameListFull);
    }
}

void ScpiNode::addNodeAndChildrenToNameListFull(const ScpiNodePtr node, const QStringList parentNames, QList<QStringList> &scpiPathList)
{
    for(auto iter=node->m_children.constBegin(); iter!=node->m_children.constEnd(); iter++) {
        const ScpiNodePtr childNode = *iter;
        QString childName = childNode->getFullHeader();
        QStringList childNameList = parentNames + QStringList(childName);
        if(!ScpiNodeStaticFunctions::isNodeTypeOnly(childNode))
            scpiPathList.append(childNameList);
        addNodeAndChildrenToNameListFull(childNode, childNameList, scpiPathList);
    }
}

void ScpiNode::removeRow(int row)
{
    m_children.takeAt(row);
    for(; row<m_children.count(); ++row) {
        m_children.at(row)->m_row = row;
    }
}

int ScpiNode::getInstanceCount()
{
    return m_instanceCount;
}

QString ScpiNode::createShortHeader(QString scpiHeader)
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

void ScpiNode::adjustScpiHeaders(QString scpiHeader)
{
    m_sScpiHeaderFull = scpiHeader.toUpper();
    m_sScpiHeaderShort = createShortHeader(m_sScpiHeaderFull);
}

bool ScpiNode::isLastShortAVowel(QString scpiHeader)
{
    return QString("AEIOU").contains(scpiHeader.mid(3, 1));
}
