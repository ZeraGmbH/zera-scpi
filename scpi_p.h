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
      @param sDevice name of the device that holds the scpi interface
      */
    cSCPIPrivate(QString sDevice);
    virtual ~cSCPIPrivate();

    /**
      @b Generates 1 scpi command with several parent node names.
      @param nodeNames all parent node names.
      @param pSCPIObject the node that holds the object to handle the command.
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
      @b Decodes commands from input, returns null for invalid commands.
      @param input [in] the received command input
      @param Param [out] position where to get parameters.
      */
    cSCPIObject* getSCPIObject(const QString& input, QString& Param);
    /**
      @b Returns the constructed model.
      */
    QStandardItemModel* getSCPIModel();
    /**
      @b Generates a xml representation of the model.
      @param sxml [out] the xml representation as QString
      */
    void exportSCPIModelXML(QString &sxml);
    /**
      @b Generates a model form a xml representation.
      @param ioDevice [in] the xml representation is read from QString.
      */
    bool importSCPIModelXML(QIODevice *ioDevice);
    /**
      @b Holds the model.
      */
    QStandardItemModel m_SCPIModel;

private:
    void genSCPICmd(const QStringList& parentnodeNames, cSCPINode* pSCPINode);
    void appendSCPIRows(QStandardItem* rootItem, QDomDocument &doc, QDomElement &rootElement, quint32 nlevel);
    quint8 getNodeType(const QString& sAttr);
    bool getcommandInfo( QDomNode rootNode, quint32 nlevel );

    /**
      @b Name of the device that holds the scpi interface
      */
    QString m_sDeviceName;
    /**
      @b Holds the parser used for decoding.
      */
    cParse m_Parser;
};

#endif // SCPI_P_H
