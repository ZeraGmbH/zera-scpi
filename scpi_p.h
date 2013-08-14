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


/**
  @brief
  The implemention for cSCPI.
  */
class cSCPIPrivate {
public:
    /**
      @b Initialise the const variables
      @param interfaceName the name of the implementation that holds the scpi interface
      */
    cSCPIPrivate(const QString& interfaceName);
    virtual ~cSCPIPrivate();

    /**
      @b Generates 1 scpi command with several parent node names.
      @param[in] parentnodeNames all parent node names.
      @param[in] pSCPIObject the node that holds the object to handle the command.
      */
    void genSCPICmd(const QStringList& parentnodeNames, cSCPIObject* pSCPIObject);
    /**
      @b Generates 1..n scpi commands with 1 parent nodename.
      */
    void genSCPICmd(QMap<QString, QList<cSCPIObject *> > SCPIObjectMap);
    /**
      @b Generates 1 x 1 .. n x m scpi commands with 2 parent nodenames.
      */
    void genSCPICmd(QMap<QString, QMap<QString, QList<cSCPIObject *> > > SCPIObjectMap);
    /**
      @b Generates 1 x 1 x 1 .. n x m x l scpi commands with 3 parent nodenames.
      */
    void genSCPICmd(QMap<QString, QMap<QString, QMap<QString, QList<cSCPIObject*> > > > SCPIObjectMap);
    /**
      @b Generates 1 x 1 x 1 x 1 .. n x m x l x k scpi commands with 4 parent nodenames.
      */
    void genSCPICmd(QMap<QString, QMap<QString, QMap<QString, QMap<QString, QList<cSCPIObject*> > > > > SCPIObjectMap);
    /**
      @b Generates 1 x 1 x 1 x 1 x 1 .. n x m x l x k x j scpi commands with 5 parent nodenames.
      */
    void genSCPICmd(QMap<QString, QMap<QString, QMap<QString, QMap<QString, QMap<QString, QList<cSCPIObject*> > > > > > SCPIObjectMap);
    /**
      @b Clears the whole command list.
      */
    void clearSCPICmdList();
    /**
      @b deletes command(s) from the command list
      */
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
    /**
      @b Generates a model form a xml representation.
      @param[in] ioDevice the xml representation is read from QString.
      */
    bool importSCPIModelXML(QIODevice *ioDevice);
    /**
      @b Holds the model.
      */
    QStandardItemModel m_SCPIModel;

private:
    /**
      @b Generates 1 scpi command with several parent node names.
      @param[in] parentnodeNames all parent node names.
      @param[in] pSCPINode the node that holds the object to handle the command.
      */
    void genSCPICmd(const QStringList& parentnodeNames, cSCPINode* pSCPINode);

    /**
      @b deletes all child items
      */
    void delChildItems(QStandardItem* Item);
    /**
      @b deletes itself and all parent items if necessary
      */
    void delItemAndParents(QStandardItem* Item);
    /**
      @b used in the XML parser
      */
    void appendSCPIRows(QStandardItem* rootItem, QDomDocument &doc, QDomElement &rootElement, quint32 nlevel);
    /**
      @b Returns the readable node type
      */
    quint8 getNodeType(const QString& sAttr);
    /**
      @b used in the XML parser
      */
    bool getcommandInfo( QDomNode rootNode, quint32 nlevel );

    /**
      @b Name of the device that holds the scpi interface
      */
    QString m_interfaceName;
    /**
      @b Holds the parser used for decoding.
      */
    cParse m_Parser;
};

#endif // SCPI_P_H
