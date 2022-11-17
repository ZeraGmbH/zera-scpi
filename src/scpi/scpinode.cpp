#include "scpinode.h"
#include "scpinodestaticfunctions.h"
#include "scpi.h"

int ScpiNode::m_instanceCount = 0;

ScpiNode::ScpiNode()
{
    m_instanceCount++;
}

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

ScpiNode *ScpiNode::findChildShort(QString shortHeader) const
{
    for(int row=0; row<rowCount(); ++row)
        if(child(row)->getShortHeader() == shortHeader)
            return child(row);
    return nullptr;
}

ScpiNode *ScpiNode::findChildFull(QString fullHeader) const
{
    for(int row=0; row<rowCount(); ++row)
        if(child(row)->getFullHeader() == fullHeader)
            return child(row);
    return nullptr;
}

ScpiNode *ScpiNode::child(int row) const
{
    return m_children[row];
}

ScpiNode *ScpiNode::parent() const
{
    return m_parent;
}

void ScpiNode::removeChild(ScpiNode *child)
{
    removeRow(child->row());
}

bool ScpiNode::isEmpty()
{
    return rowCount() == 0;
}

int ScpiNode::rowCount() const
{
    return m_children.count();
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

void ScpiNode::addNodeAndChildrenToXml(ScpiNode *node, QDomDocument &doc, QDomElement &rootElement, const QStringList parentNames)
{
    for(int row = 0; row < node->rowCount(); row++) {
        ScpiNode *childNode = node->child(row);
        QString childName = childNode->getFullHeader();

        QDomElement cmdTag = doc.createElement(ScpiNodeStaticFunctions::makeValidXmlTag(childName));

        QStringList childNames = parentNames;
        childNames.append(childName);
        if(!ScpiNodeStaticFunctions::isNodeTypeOnly(childNode))
            cmdTag.setAttribute("ScpiPath", childNames.join(":"));
        cSCPIObject::XmlKeyValueMap xmlAtributes;
        if(childNode->getScpiObject())
            xmlAtributes = childNode->getScpiObject()->getXmlAttibuteMap();
        for(auto iter=xmlAtributes.constBegin(); iter!=xmlAtributes.constEnd(); ++iter)
            cmdTag.setAttribute(iter.key(), iter.value());

        QString typeInfo;
        if(parentNames.isEmpty())
            typeInfo = "Model,";
        typeInfo += ScpiNodeStaticFunctions::scpiTypeToString(childNode->getType());
        cmdTag.setAttribute("Type", typeInfo);

        rootElement.appendChild(cmdTag);
        addNodeAndChildrenToXml(childNode, doc, cmdTag, childNames);
    }
}

void ScpiNode::removeRow(int row)
{
    delete m_children.takeAt(row);
    for(int row=0; row<m_children.count(); ++row) {
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
    else if(isLastShortAVowel())
        m_sScpiHeaderShort = m_sScpiHeaderFull.left(3);
    else {
        m_sScpiHeaderShort = m_sScpiHeaderFull.left(4);
    }
}

bool ScpiNode::isLastShortAVowel()
{
    return QString("AEIOU").contains(m_sScpiHeaderFull.mid(3, 1));
}

bool ScpiNode::foundNode(ScpiNode *parentNode, ScpiNode **scpiChildNode, cParse *parser, QChar *pInput)
{
    bool found = false;
    QString searchHeader = parser->GetKeyword(&pInput).toUpper();
    for(int row = 0; row < parentNode->rowCount(); row++) {
        ScpiNode *childNode = parentNode->child(row);
        *scpiChildNode = childNode;
        if(childNode->getShortHeader() == searchHeader ||
                childNode->getFullHeader() == searchHeader) {
            found = true;
            if(*pInput == ':') { // in case input is not parsed completely
                ScpiNode* saveNode = *scpiChildNode;
                QChar* saveInput = pInput;
                found = found && foundNode(childNode, scpiChildNode, parser, pInput);
                if(!found) {
                    *scpiChildNode = saveNode; // ifnot found we reset the childnode
                    pInput = saveInput; // and input pointer
                }
            }
        }
        if(found)
            break;
    }
    return found;
}

