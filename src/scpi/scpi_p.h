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
    virtual ~cSCPIPrivate();
    /**
      @b Generates 1 scpi command with several parent node names.
      @param[in] parentnodeNames all parent node names.
      @param[in] pSCPIObject the node that holds the object to handle the command.
      */
    void insertScpiCmd(const QStringList& parentnodeNames, cSCPIObject* pSCPIObject);
    void delSCPICmds(const QString& cmd);
    /**
      @b Decodes commands from input, returns null for invalid commands.
      @param[in] input the received command input
      @param[out] Param position where to get parameters.
      */
    cSCPIObject* getSCPIObject(const QString& input, QString& Param, bool caseSensitive = false);
    /**
      @b Returns the constructed model.
      */
    QStandardItemModel* getSCPIModel();
    /**
      @b Generates a xml representation of the model.
      @param[out] sxml the xml representation as QString
      */
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
