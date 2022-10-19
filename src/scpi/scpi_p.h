#ifndef SCPI_P_H
#define SCPI_P_H

#include "scpistring.h"
#include "parse.h"
#include "scpiobject.h"
#include "scpinode.h"
#include <QStandardItemModel>
#include <QString>
#include <QDomDocument>

const QString scpimodelDocName = "SCPIModel";
const QString scpimodelrootName = "MODELLIST";
const QString scpimodeldeviceTag =  "DEVICE";
const QString scpimodelsTag = "MODELS";
const QString scpinodeAttributeName = "Type";

class cSCPIPrivate {
public:
    cSCPIPrivate(const QString& interfaceName);
    void insertScpiCmd(const QStringList& parentnodeNames, cSCPIObject* pSCPIObject);
    void delSCPICmds(const QString& cmd);
    cSCPIObject* getSCPIObject(const QString& input, QString& Param, bool caseSensitive = false);
    void exportSCPIModelXML(QString &sxml);

private:
    cSCPINode* createNode(const QString &name, quint8 type, cSCPIObject *scpiObject);
    void delChildItems(QStandardItem* Item);
    void delItemAndParents(QStandardItem* Item);
    void appendScpiNodeXmlInfo(QStandardItem* rootItem, QDomDocument &doc, QDomElement &rootElement, quint32 nlevel);
    static QString scpiTypeToString(quint8 scpiType);
    QStandardItem *findOrCreateChildParentItem(QStandardItem *parentItem, const QStringList& parentnodeNames);
    bool foundItem(QStandardItem *parentItem, cSCPINode** scpiChildItem, QChar* pInput, bool caseSensitive);

    QStandardItemModel m_SCPIModel;
    QString m_interfaceName;
    cParse m_Parser;
};

#endif // SCPI_P_H
