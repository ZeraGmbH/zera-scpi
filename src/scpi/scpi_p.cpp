#include <QObject>
#include <QModelIndex>
#include <QDomDocument>
#include <QDomDocumentType>
#include <QDomElement>
#include <QList>
#include "scpi_p.h"
#include "scpinode.h"
#include "scpi.h"

cSCPIPrivate::cSCPIPrivate(const QString &interfaceName) :
    m_interfaceName(interfaceName)
{
}

void cSCPIPrivate::insertScpiCmd(const QStringList& parentnodeNames, cSCPIObject *pSCPIObject)
{
    ScpiNode *parentItem = &m_invisibleRootItem;
    ScpiNode *childItem;
    if (parentnodeNames.count() > 0 && parentnodeNames.at(0) != "") {
        parentItem = findOrCreateChildParentItem(parentItem, parentnodeNames);
    }
    bool overwriteExisting = false;
    QString sName = pSCPIObject->getName();
    for (int row = 0; row < parentItem->rowCount(); row++) {
        childItem = parentItem->child(row);
        if (childItem->getName() == sName) {
            overwriteExisting = true;
            break;
        }
    }
    if (overwriteExisting) {
        childItem->setScpiObject(pSCPIObject);
        childItem->setType(SCPI::isNode | pSCPIObject->getType());
    }
    else {
        parentItem->appendRow(createNode(pSCPIObject->getName(),pSCPIObject->getType(), pSCPIObject));
    }
}

void cSCPIPrivate::delItemAndParents(ScpiNode *delItem)
{
    while(delItem->parent()) {
        int row = delItem->row();
        ScpiNode *parent = delItem->parent();
        parent->removeRow(row);
        if(parent->rowCount() >= 1)
            return;
        delItem = parent;
    }
}

void cSCPIPrivate::delSCPICmds(const QString &cmd)
{
    QStringList slNodeNames;
    cParse Parser;
    QChar* pInput = (QChar*) cmd.data();
    do {
        QString keyw = Parser.GetKeyword(&pInput); // we fetch all keywords from command
        slNodeNames.append(keyw); // and put them into the string list
    } while (*pInput  == ':');

    if (slNodeNames.count() > 0 ) {
        ScpiNode *parentItem = &m_invisibleRootItem;
        for(const auto &nodeName: slNodeNames) {
            ScpiNode *childItem = nullptr;
            for (int row = 0; row < parentItem->rowCount(); row++) {
                childItem = parentItem->child(row);
                if (childItem->getName() == nodeName)
                    break;
                else
                    childItem = nullptr;
            }
            parentItem = childItem;
            if (!parentItem)
                break;
        }
        if (parentItem) { // we found the whole keyword list in the model
            delItemAndParents(parentItem);
        }
    }
}

cSCPIObject* cSCPIPrivate::getSCPIObject(const QString& input, bool caseSensitive)
{
    ScpiNode *childItem = nullptr;
    if (foundItem(&m_invisibleRootItem, &childItem, (QChar*) input.data(), caseSensitive)) {
        return childItem->getScpiObject();
    }
    return nullptr;
}

void cSCPIPrivate::appendScpiNodeXmlInfo(ScpiNode *rootItem, QDomDocument& doc,  QDomElement &rootElement, const QStringList parentNames)
{
    for (int row = 0; row < rootItem->rowCount(); row++) {
        ScpiNode *childItem = rootItem->child(row);
        QString childName = childItem->getName();

        QDomElement cmdTag = doc.createElement(makeValidXmlTag(childName));

        QStringList childNames = parentNames;
        childNames.append(childName);
        if(!isNodeTypeOnly(childItem)) {
            cmdTag.setAttribute("ScpiPath", childNames.join(":"));
        }
        cSCPIObject::XmlKeyValueMap xmlAtributes;
        if(childItem->getScpiObject()) {
            xmlAtributes = childItem->getScpiObject()->getXmlAttibuteMap();
        }
        for(auto iter=xmlAtributes.constBegin(); iter!=xmlAtributes.constEnd(); ++iter) {
            cmdTag.setAttribute(iter.key(), iter.value());
        }

        QString typeInfo;
        if (parentNames.isEmpty())
            typeInfo = "Model,";
        typeInfo += scpiTypeToString(childItem->getType());
        cmdTag.setAttribute("Type", typeInfo);

        rootElement.appendChild(cmdTag);
        appendScpiNodeXmlInfo(childItem, doc, cmdTag, childNames);
    }
}

static const QString scpiNodeType[SCPI::CmdwP+1] =
{
    "Node",
    "Query",
    "Command",
    "Command+Par"
};

QString cSCPIPrivate::scpiTypeToString(quint8 scpiType)
{
    QString typeInfo;
    if (scpiType & SCPI::isNode)
        typeInfo = scpiNodeType[SCPI::Node];

    if (scpiType & SCPI::isQuery) {
        if (typeInfo.length() > 0)
            typeInfo += ",";
        typeInfo += scpiNodeType[SCPI::Query];
    }

    if (scpiType & SCPI::isCmd) {
        if (typeInfo.length() > 0)
            typeInfo += ",";
        typeInfo += scpiNodeType[SCPI::Cmd];
    }

    if (scpiType & SCPI::isCmdwP) {
        if (typeInfo.length() > 0)
            typeInfo += ",";
        typeInfo += scpiNodeType[SCPI::CmdwP];
    }
    return typeInfo;
}

void cSCPIPrivate::exportSCPIModelXML(QString& sxml)
{
    QDomDocument modelDoc("SCPIModel");

    QDomElement rootTag = modelDoc.createElement("MODELLIST");
    modelDoc.appendChild( rootTag );

    QDomElement deviceTag = modelDoc.createElement("DEVICE");
    rootTag.appendChild( deviceTag );
    QDomText t = modelDoc.createTextNode(m_interfaceName);
    deviceTag.appendChild( t );

    QDomElement modelsTag = modelDoc.createElement("MODELS");
    rootTag.appendChild( modelsTag );

    appendScpiNodeXmlInfo(&m_invisibleRootItem, modelDoc, modelsTag, QStringList());

    sxml = modelDoc.toString();
}

ScpiNode *cSCPIPrivate::findOrCreateChildParentItem(ScpiNode *parentItem, const QStringList &parentnodeNames)
{
    for(const QString &nodeName : parentnodeNames) {
        ScpiNode *childItem = nullptr;
        for (int row = 0; row < parentItem->rowCount(); row++) {
            childItem = parentItem->child(row);
            if (childItem->getName() == nodeName)
                break;
            else
                childItem = nullptr;
        }
        if (!childItem) {
            childItem  = createNode(nodeName, SCPI::isNode, nullptr);
            parentItem->appendRow(childItem);
        }
        parentItem = childItem;
    }
    return parentItem;
}

bool cSCPIPrivate::foundItem(ScpiNode *parentItem, ScpiNode **scpiChildItem, QChar *pInput, bool caseSensitive)
{
    bool found = false;
    int nrows = parentItem->rowCount();
    if ( nrows > 0) { // keywords in next command level available
        cSCPIString keyw = (m_Parser.GetKeyword(&pInput)); // we fetch a new keyword
        if (!caseSensitive)
            keyw = keyw.toUpper();
        for (int row = 0; row < nrows; row++) {
            ScpiNode *childItem = parentItem->child(row);
            *scpiChildItem = childItem;
            QString s = (*scpiChildItem)->getName();
            if (!caseSensitive)
                s = s.toUpper(); // (s) is a node name from command tree
            if ((found = (keyw == s))) {
                if (*pInput == ':') { // in case input is not parsed completely
                    ScpiNode* saveNode = *scpiChildItem;
                    QChar* saveInput = pInput;
                    found = found && foundItem(childItem, scpiChildItem, pInput, caseSensitive);
                    if (!found) {
                        *scpiChildItem = saveNode; // if not found we reset the childnode
                        pInput = saveInput; // and input pointer
                    }
                }
            }
            if (found)
                break;
        }
    }
    return found;
}

bool cSCPIPrivate::isNodeTypeOnly(ScpiNode *item)
{
    return item->getType() == SCPI::isNode;
}

QString cSCPIPrivate::makeValidXmlTag(QString xmlTag)
{
    xmlTag.replace("*", "");
    if(xmlTag.count() > 0 && xmlTag[0].isNumber())
        xmlTag = "N_" + xmlTag;
    return xmlTag;
}

ScpiNode *cSCPIPrivate::createNode(const QString &name, quint8 type, cSCPIObject *scpiObject)
{
    return new ScpiNode(name, type, scpiObject);
}

