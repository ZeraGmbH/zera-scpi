#include <QObject>
#include <QModelIndex>
#include <QDomDocument>
#include <QDomDocumentType>
#include <QDomElement>
#include <QList>
#include "scpi_p.h"
#include "scpinode.h"
#include "scpi.h"

cSCPIPrivate::cSCPIPrivate(const QString &interfaceName)
    :m_interfaceName(interfaceName)
{
    m_SCPIModel.clear();
}

cSCPIPrivate::~cSCPIPrivate()
{
}

void cSCPIPrivate::insertScpiCmd(const QStringList& parentnodeNames, cSCPIObject *pSCPIObject)
{
    QStandardItem *parentItem = m_SCPIModel.invisibleRootItem();
    cSCPINode *childItem;
    if (parentnodeNames.count() > 0 && parentnodeNames.at(0) != "") {
        parentItem = findOrCreateChildParentItem(parentItem, parentnodeNames);
    }
    bool overwriteExisting = false;
    QString sName = pSCPIObject->getName();
    for (int row = 0; row < parentItem->rowCount(); row++) {
        childItem = static_cast<cSCPINode*>(parentItem->child(row));
        if (childItem->getName() == sName) {
            overwriteExisting = true;
            break;
        }
    }
    if (overwriteExisting) {
        cSCPINode *c = (cSCPINode*) childItem;
        c->m_pSCPIObject = pSCPIObject;
        c->setType(SCPI::isNode | pSCPIObject->getType());
    }
    else {
        parentItem->appendRow(createNode(pSCPIObject->getName(),pSCPIObject->getType(), pSCPIObject));
    }
}

void cSCPIPrivate::delChildItems(QStandardItem *Item)
{
    while ( Item->rowCount() > 0 ) { // as long as we have child item rows
        delChildItems(Item->child(0)); // we delete all entries behind item recursively
        Item->removeRow(0);
    }
}

void cSCPIPrivate::delItemAndParents(QStandardItem *Item)
{
    QStandardItem *parentItem = Item->parent();
    if (parentItem) { // do we have a parent ?
        if (parentItem->rowCount() == 1) { // if this item is the ony child of parent
            parentItem->removeRow(0); // we delete it
            delItemAndParents(parentItem); // and test if the parent has to be deleted too
        }
        else
            parentItem->removeRow(Item->row());
    }
    else {
        QStandardItemModel *mdl = Item->model();
        mdl->removeRow(Item->row());
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
        QStandardItem *parentItem = m_SCPIModel.invisibleRootItem();
        for(const auto &nodeName: slNodeNames) {
            cSCPINode *childItem = nullptr;
            for (int row = 0; row < parentItem->rowCount(); row++) {
                childItem = static_cast<cSCPINode*>(parentItem->child(row));
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
            delChildItems(parentItem); // so lets delete what's necessary
            delItemAndParents(parentItem);
        }
    }
}

cSCPIObject* cSCPIPrivate::getSCPIObject(const QString& input, QString &Param, bool caseSensitive)
{
    cSCPINode *childItem = nullptr;
    QChar* pInput;
    if (foundItem(m_SCPIModel.invisibleRootItem(), &childItem, pInput = (QChar*) input.data(), caseSensitive)) {
        Param = QString(pInput);
        return childItem->m_pSCPIObject;
    }
    else
        return nullptr;
}

QStandardItemModel* cSCPIPrivate::getSCPIModel()
{
    return &m_SCPIModel;
}

void cSCPIPrivate::appendSCPIRows(QStandardItem *rootItem, QDomDocument& doc,  QDomElement &rootElement, quint32 nlevel)
{
    for (int row = 0; row < rootItem->rowCount(); row++) {
        cSCPINode *childItem = static_cast<cSCPINode*>(rootItem->child(row));
        QString nodeName = childItem->getName();
        QDomElement cmdTag = doc.createElement(nodeName);
        if (nlevel == 0)
            nodeName = "Model,";
        else
            nodeName = "";
        nodeName += childItem->getTypeInfo();
        cmdTag.setAttribute(scpinodeAttributeName, nodeName);
        rootElement.appendChild(cmdTag);
        appendSCPIRows(childItem, doc, cmdTag, ++nlevel);
        --nlevel;
    }
}

void cSCPIPrivate::exportSCPIModelXML(QString& sxml)
{
    QDomDocument modelDoc(scpimodelDocName);

    QDomElement rootTag = modelDoc.createElement( scpimodelrootName );
    modelDoc.appendChild( rootTag );

    QDomElement deviceTag = modelDoc.createElement( scpimodeldeviceTag );
    rootTag.appendChild( deviceTag );
    QDomText t = modelDoc.createTextNode(m_interfaceName);
    deviceTag.appendChild( t );

    QDomElement modelsTag = modelDoc.createElement(scpimodelsTag);
    rootTag.appendChild( modelsTag );

    appendSCPIRows(m_SCPIModel.invisibleRootItem(), modelDoc, modelsTag, 0);

    sxml = modelDoc.toString();
}

QStandardItem *cSCPIPrivate::findOrCreateChildParentItem(QStandardItem *parentItem, const QStringList &parentnodeNames)
{
    for(const QString &nodeName : parentnodeNames) {
        cSCPINode *childItem = nullptr;
        for (int row = 0; row < parentItem->rowCount(); row++) {
            childItem = static_cast<cSCPINode*>(parentItem->child(row));
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

bool cSCPIPrivate::foundItem(QStandardItem *parentItem, cSCPINode **scpiChildItem, QChar *pInput, bool caseSensitive)
{
    bool found = false;
    int nrows = parentItem->rowCount();
    if ( nrows > 0) { // keywords in next command level available
        cSCPIString keyw = (m_Parser.GetKeyword(&pInput)); // we fetch a new keyword
        if (!caseSensitive)
            keyw = keyw.toUpper();
        for (int row = 0; row < nrows; row++) {
            cSCPINode *childItem = static_cast<cSCPINode*>(parentItem->child(row));
            *scpiChildItem = (cSCPINode*) childItem;
            QString s = (*scpiChildItem)->getName();
            if (!caseSensitive)
                s = s.toUpper(); // (s) is a node name from command tree
            if ((found = (keyw == s))) {
                if (*pInput == ':') { // in case input is not parsed completely
                    cSCPINode* saveNode = *scpiChildItem;
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

cSCPINode *cSCPIPrivate::createNode(const QString &name, quint8 type, cSCPIObject *scpiObject)
{
    return new cSCPINode(name, type, scpiObject);
}

