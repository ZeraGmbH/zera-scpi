#ifndef SCPI_P_H
#define SCPI_P_H

#include <QStandardItemModel>
#include <QIODevice>
#include <qstring.h>
#include "scpistring.h"
#include "parse.h"


class cSCPIObject;
class cSCPINode;


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
    void genSCPICmd(const QStringList& parentnodeNames, cSCPIObject* pSCPIObject);
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

    QStandardItemModel m_SCPIModel;

private:
    cSCPINode* createNode(const QString &name, quint8 type, cSCPIObject *scpiObject);
    void delChildItems(QStandardItem* Item);
    void delItemAndParents(QStandardItem* Item);
    void appendSCPIRows(QStandardItem* rootItem, QDomDocument &doc, QDomElement &rootElement, quint32 nlevel);
    QStandardItem *findOrCreateChildParentItem(QStandardItem *parentItem, const QStringList& parentnodeNames);
    bool foundItem(QStandardItem *parentItem, cSCPINode** scpiChildItem, QChar* pInput, bool caseSensitive);


    QString m_interfaceName;
    cParse m_Parser;
};

#endif // SCPI_P_H
