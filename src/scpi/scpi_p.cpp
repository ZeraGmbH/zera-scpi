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

void cSCPIPrivate::genSCPICmd(const QStringList& parentnodeNames, cSCPIObject *pSCPIObject)
{
    QStandardItem *parentItem = m_SCPIModel.invisibleRootItem();
    QStandardItem *childItem;
    if (parentnodeNames.count() > 0 && parentnodeNames.at(0) != "") {
        parentItem = findOrCreateChildParentItem(parentItem, parentnodeNames);
    }
    bool overwriteExisting = false;
    QString sName = pSCPIObject->getName();
    for (quint32 i = 0; i < parentItem->rowCount(); i++) {
        childItem = parentItem->child(i);
        if (childItem->data(Qt::DisplayRole).toString() == sName) {
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

void cSCPIPrivate::insertNode(const QStringList &parentnodeNames, cSCPINode *pSCPINode)
{
    QStandardItem *parentItem = m_SCPIModel.invisibleRootItem();
    parentItem = findOrCreateChildParentItem(parentItem, parentnodeNames);
    parentItem->appendRow(pSCPINode);
}

void cSCPIPrivate::delChildItems(QStandardItem *Item)
{
    while ( Item->rowCount() > 0 ) // as long as we have child item rows
    {
        delChildItems(Item->child(0)); // we delete all entries behind item recursively
        Item->removeRow(0);
    }
}

void cSCPIPrivate::delItemAndParents(QStandardItem *Item)
{
    QStandardItem *parentItem = Item->parent();

    if (parentItem) // do we have a parent ?
    {
        if (parentItem->rowCount() == 1) // if this item is the ony child of parent
        {
            parentItem->removeRow(0); // we delete it
            delItemAndParents(parentItem); // and test if the parent has to be deleted too
        }
        else
            parentItem->removeRow(Item->row());

    }
    else
    {
        QStandardItemModel *mdl = Item->model();
        mdl->removeRow(Item->row());
    }
}

void cSCPIPrivate::delSCPICmds(const QString &cmd)
{
    cParse Parser;
    QChar* pInput;
    pInput = (QChar*) cmd.data();
    QString keyw;
    QStringList slNodeNames;

    do
    {
        keyw = Parser.GetKeyword(&pInput); // we fetch all keywords from command
        slNodeNames.append(keyw); // and put them into the string list
    } while (*pInput  == ':');

    if (slNodeNames.count() > 0 )
    {
        QStringList::const_iterator it;
        QStandardItem *parentItem;
        QStandardItem *childItem;

        parentItem = m_SCPIModel.invisibleRootItem();

        for (it = slNodeNames.begin(); it != slNodeNames.end(); ++it)
        {

            childItem = 0; // we use childitem = 0 as information that no matching node was found

            quint32 nrows = parentItem->rowCount();

            if (nrows > 0)
                for (quint32 j = 0; j < nrows; j++)
                {
                    childItem = parentItem->child(j);
                    if (childItem->data(Qt::DisplayRole) == *it)
                        break; //
                    else
                        childItem = 0;
                }

            if ( !(parentItem = childItem)) // from here we will continue if possible
                break;

        }

        if (parentItem) // we found the whole keyword list in the model
        {
            delChildItems(parentItem); // so lets delete what's necessary
            delItemAndParents(parentItem);
        }
    }
}

cSCPIObject* cSCPIPrivate::getSCPIObject(const QString& input, QString &Param, bool caseSensitive)
{
    cSCPINode *childItem = 0;
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
    cSCPINode *childItem;
    for (quint32 row = 0; row < rootItem->rowCount(); row++)
    {
        childItem = (cSCPINode*) rootItem->child(row);
        QString nodeName = childItem->data(Qt::DisplayRole).toString();
        QDomElement cmdTag = doc.createElement(nodeName);
        if (nlevel == 0)
            nodeName = "Model,";
        else
            nodeName = "";
        nodeName += childItem->data(Qt::ToolTipRole).toString();
        cmdTag.setAttribute(scpinodeAttributeName, nodeName);
        rootElement.appendChild(cmdTag);
        appendSCPIRows(childItem, doc, cmdTag, ++nlevel);
        --nlevel;
    }
}

void cSCPIPrivate::exportSCPIModelXML(QString& sxml)
{
    QDomDocument modelDoc(scpimodelDocName);

    QDomElement rootTag;
    rootTag = modelDoc.createElement( scpimodelrootName );
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


quint8 cSCPIPrivate::getNodeType(const QString& sAttr)
{
    int t = SCPI::isNode;
    if ( sAttr.contains(SCPI::scpiNodeType[SCPI::Query]) )
        t += SCPI::isQuery;
    if ( sAttr.contains(SCPI::scpiNodeType[SCPI::CmdwP]) )
        t += SCPI::isCmdwP;
    else
    if ( sAttr.contains(SCPI::scpiNodeType[SCPI::Cmd]) )
        t += SCPI::isCmd;

    return t;
}


bool cSCPIPrivate::getcommandInfo( QDomNode rootNode, quint32 nlevel )
{
    static QStringList nodeNames;
    QDomNodeList nl = rootNode.childNodes();

    quint32 n = nl.length();
    if ( n > 0)
    {
        quint32 i;
        for (i=0; i<n ; i++)
        {
            if (nlevel == 0)
                nodeNames.clear(); // for each root cmd we clear the local node name list
            QDomNode node2 = nl.item(i);
            QDomElement e = node2.toElement();
            QString s = e.tagName();
            if (!node2.hasChildNodes())
            {
                quint8 t = getNodeType(e.attribute(scpinodeAttributeName));
                cSCPINode *pSCPINode = createNode (s , t, NULL); // we have no corresponding cSCPIObject -> NULL
                insertNode(nodeNames, pSCPINode);
            }
            else
            {
                nodeNames.append(s); // we add each node name to the list
                getcommandInfo(node2, ++nlevel); // we look for further levels
                --nlevel;
            }
        }
    }

    if (nlevel > 0)
        nodeNames.pop_back();

    return true;
}

QStandardItem *cSCPIPrivate::findOrCreateChildParentItem(QStandardItem *parentItem, const QStringList &parentnodeNames)
{
    for(const QString &nodeName : parentnodeNames) {
        QStandardItem *childItem = nullptr;
        for (quint32 row = 0; row < parentItem->rowCount(); row++) {
            childItem = parentItem->child(row);
            if (childItem->data(Qt::DisplayRole) == nodeName)
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
    quint32 nrows = parentItem->rowCount();
    if ( nrows > 0) { // keywords in next command level available
        cSCPIString keyw = (m_Parser.GetKeyword(&pInput)); // we fetch a new keyword
        if (!caseSensitive)
            keyw = keyw.toUpper();
        for (quint32 i = 0; i < nrows; i++) {
            QStandardItem *childItem = parentItem->child(i);
            *scpiChildItem = (cSCPINode*) childItem;
            QString s = (*scpiChildItem)->data(Qt::DisplayRole).toString();
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

