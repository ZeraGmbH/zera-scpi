#include "scpinode.h"
#include "scpinodestaticfunctions.h"
#include "scpi.h"

int ScpiNode::m_instanceCount = 0;

ScpiNode::ScpiNode(const QString& scpiHeader, cSCPIObject* pSCPIObject) :
    m_pScpiObject(pSCPIObject)
{
    adjustScpiHeaders(scpiHeader);
    m_instanceCount++;
}

ScpiNode::~ScpiNode()
{
    while(!m_children.isEmpty())
        delete m_children.takeFirst();
    m_instanceCount--;
}

cSCPIObject *ScpiNode::getScpiObject() const
{
    return m_pScpiObject;
}

void ScpiNode::setScpiObject(cSCPIObject *pScpiObject)
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

ScpiNode *ScpiNode::findChildShort(const QString &shortHeader) const
{
    for(auto iter=m_children.constBegin(); iter!=m_children.constEnd(); iter++) {
        if((*iter)->getShortHeader() == shortHeader) {
            return *iter;
        }
    }
    return nullptr;
}

QList<ScpiNode *> ScpiNode::findAllChildrenShort(const QString &shortHeader) const
{
    QList<ScpiNode *> found;
    for(auto iter=m_children.constBegin(); iter!=m_children.constEnd(); iter++) {
        if((*iter)->getShortHeader() == shortHeader) {
            found.append(*iter);
        }
    }
    return found;
}

ScpiNode *ScpiNode::findChildFull(const QString &fullHeader) const
{
    for(auto iter=m_children.constBegin(); iter!=m_children.constEnd(); iter++) {
        if((*iter)->getFullHeader() == fullHeader) {
            return *iter;
        }
    }
    return nullptr;
}

ScpiNode *ScpiNode::parent() const
{
    return m_parent;
}

void ScpiNode::removeChild(ScpiNode *child)
{
    removeRow(child->row());
}

bool ScpiNode::isEmpty() const
{
    return m_children.isEmpty();
}

int ScpiNode::row() const
{
    return m_row;
}

void ScpiNode::add(ScpiNode *node)
{
    node->m_parent = this;
    node->m_row = m_children.count();
    m_children.append(node);
}

void ScpiNode::addNodeSpecificAttributes(const ScpiNode *childNode, QDomElement &cmdTag)
{
    cSCPIObject::XmlKeyValueMap xmlAtributes;
    if(childNode->getScpiObject())
        xmlAtributes = childNode->getScpiObject()->getXmlAttibuteMap();
    for(auto attIter=xmlAtributes.constBegin(); attIter!=xmlAtributes.constEnd(); ++attIter)
        cmdTag.setAttribute(attIter.key(), attIter.value());
}

QDomElement ScpiNode::createCmdTag(QStringList childNames, QDomDocument &doc, QString childName, const ScpiNode *childNode)
{
    QDomElement cmdTag = doc.createElement(ScpiNodeStaticFunctions::makeValidXmlTag(childName));
    if(!ScpiNodeStaticFunctions::isNodeTypeOnly(childNode))
        cmdTag.setAttribute("ScpiPath", childNames.join(":"));

    return cmdTag;
}

void ScpiNode::addTypeAttribute(QDomElement &cmdTag, const ScpiNode *childNode, const QStringList parentNames)
{
    QString typeInfo;
    if(parentNames.isEmpty())
        typeInfo = "Model,";
    typeInfo += ScpiNodeStaticFunctions::scpiTypeToString(childNode->getType());
    cmdTag.setAttribute("Type", typeInfo);
}

void ScpiNode::addNodeAndChildrenToXml(const ScpiNode *node, QDomDocument &doc, QDomElement &rootElement, const QStringList parentNames)
{
    for(auto iter=node->m_children.constBegin(); iter!=node->m_children.constEnd(); iter++) {
        const ScpiNode *childNode = *iter;
        QString childNameFull = childNode->getFullHeader();
        QStringList childNameListFull = parentNames + QStringList(childNameFull);

        QDomElement cmdTag = createCmdTag(childNameListFull, doc, childNameFull, childNode);
        addNodeSpecificAttributes(childNode, cmdTag);
        addTypeAttribute(cmdTag, childNode, parentNames);
        rootElement.appendChild(cmdTag);

        addNodeAndChildrenToXml(childNode, doc, cmdTag, childNameListFull);
    }
}

void ScpiNode::addNodeAndChildrenToNameListFull(const ScpiNode *node, const QStringList parentNames, QList<QStringList> &scpiPathList)
{
    for(auto iter=node->m_children.constBegin(); iter!=node->m_children.constEnd(); iter++) {
        const ScpiNode *childNode = *iter;
        QString childName = childNode->getFullHeader();
        QStringList childNameList = parentNames + QStringList(childName);
        if(!ScpiNodeStaticFunctions::isNodeTypeOnly(childNode))
            scpiPathList.append(childNameList);
        addNodeAndChildrenToNameListFull(childNode, childNameList, scpiPathList);
    }
}

void ScpiNode::removeRow(int row)
{
    delete m_children.takeAt(row);
    for(; row<m_children.count(); ++row) {
        m_children.at(row)->m_row = row;
    }
}

int ScpiNode::getInstanceCount()
{
    return m_instanceCount;
}

void ScpiNode::adjustScpiHeaders(QString scpiHeader)
{
    m_sScpiHeaderFull = scpiHeader.toUpper();
    if(m_sScpiHeaderFull.length() < 4)
        m_sScpiHeaderShort = m_sScpiHeaderFull;
    else if(isLastShortAVowel(m_sScpiHeaderFull))
        m_sScpiHeaderShort = m_sScpiHeaderFull.left(3);
    else {
        m_sScpiHeaderShort = m_sScpiHeaderFull.left(4);
    }
}

bool ScpiNode::isLastShortAVowel(QString scpiHeader)
{
    return QString("AEIOU").contains(scpiHeader.mid(3, 1));
}
