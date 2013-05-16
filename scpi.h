/**********************************************************************************/
/**
 * @file scpi.h
 * @brief scpi.h holds all necessary declarations for the implemented scpi interface
 * @author Peter Lohmer p.lohmer@zera.de
***********************************************************************************/


/**
  @mainpage SCPI stands for "standard commands for programmable instruments", originally developed as an extension for IEEE488-2.
  Later SCPI was also defined for serial and network interfaces.

   The behaviour of scpi for servers or modules(devices) is different. scpi devices only answer on queries.
   after execution of commands they only set status information, which can (must, should?) be queried afterwards.
   For our internal client(modules) and server(pcb, dsp) connection i wanted to save additional traffic and decided
   to send an answer also for executed commands. For invalid commands servers send nak, modules only affect the status.
*/

#ifndef SCPI_H
#define SCPI_H

#include <QStringList>
#include <QString>
#include <QList>
#include <QMap>
#include <QMapIterator>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QIODevice>
#include "SCPI_global.h"
#include "scpiobject.h"
#include "scpicommand.h"


namespace SCPI
{
  /**
    @b enum for possible answers
    @see SCPI::scpiAnswer
    */
enum eSCPICmdResult { ack, nak, busy, errcon, erraut, errval, errxml, errmmem,
                      errpath, errexec, errtimo, last };

/**
  @b possible scpi answers
  */
const QString scpiAnswer[errtimo+1] = { "ack",     ///< acknowledge (command recognized and executed)
                                        "nak",     ///< not acknowledge (kommando not recognized)
                                        "busy",    ///< device busy
                                        "errcon",  ///< error connection (command recognized but no connection to device)
                                        "erraut",  ///< error authorization (command recognized but client is not auth. )
                                        "errval",  ///< error value (command recognized but incorrect parameter)
                                        "errxml",  ///< error xml (used xml file was incorrect)
                                        "errmmem", ///< error mass memory
                                        "errpath", ///< error path
                                        "errexec", ///< error execution
                                        "errtimo" ///< error timeout
                                      };
/**
  @b possible scpi types, this one should be used when defining the type
  */
enum eSCPINodeType { isNode = 1, isQuery = 2, isCmd = 4, isCmdwP = 8};
/**
  @b enum for possible scpi nodes
  @see SCPI::scpiNodeType
  */
enum eSCPINode { Node, Query, Cmd, CmdwP };

/**
  @b possible scpi node types as String
  */
const QString scpiNodeType[CmdwP+1] = { "Node",
                                  "Query",
                                  "Command",
                                  "Command+Par" };

}


class cSCPIPrivate; // forward

/**
  @brief
  cSCPI is a class to support servers and modules to generate their SCPI interface.

  To keep cSCPI clean, it only decodes the commands from input and returns a pointer to cSCPIObject, which must then
  decide what to do, whether is it a query or a command and if necessary return an answer.

  It holds all scpi commands in a qstandarditemmodel.

  The commandlist can be exported in xml and imported with xml.
  */
class SCPISHARED_EXPORT cSCPI {
public:
    /**
      @b Initialise the const variables
      @param interfaceName the name of the implementation that holds the scpi interface
      */
    cSCPI(QString interfaceName);
    virtual ~cSCPI();

    /**
      @b Generates 1 scpi command with several parent node names.
      @param[in] parentnodeNames all parent node names.
      @param[in] pSCPIObject the node that holds the object to handle the command.
      */
    void genSCPICmd(const QStringList& parentnodeNames, cSCPIObject* pSCPIObject);

    /**
      @b Generates 1..n scpi commands with 1 parent nodename.
      */
    void genSCPICmd(QMap<QString, QList<cSCPIObject*> > SCPIObjectMap);
    /**
      @b Generates 1 x 1 .. n x m scpi commands with 2 parent nodenames.
      */
    void genSCPICmd(QMap<QString, QMap<QString, QList<cSCPIObject*> > > SCPIObjectMap);
    /**
      @b Generates 1 x 1x 1 .. n x m x l scpi commands with 3 parent nodenames.
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
      @b deletes 1 command from the command list
      */
    void delSCPICmds(cSCPICommand &cmd);
    /**
      @b Decodes commands from input, returns null for invalid commands.
      @param[in] input the received command input
      @param[out] Param position where to get parameters.
      */
    cSCPIObject* getSCPIObject(const QString& input, QString& Param);
    /**
      @b Decodes commands from input, returns null for invalid commands.
      @param[in] input the cSCPICommand to execute
      */
    cSCPIObject* getSCPIObject(cSCPICommand &input);


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

private:
    /**
      @b D'pointer to the private library internal structure

      this is used to hide the internal structure, and thus make the library ABI safe
      */
    cSCPIPrivate *d_ptr;
};

#endif // SCPI_H
