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
#include "scpi_export.h"
#include "scpiobject.h"
#include "scpicommand.h"

namespace SCPI
{
enum eSCPICmdResult { ack, nak, busy, erraut, errval, errxml,
                      errpath, errexec,
                      resultcount };

const QString scpiAnswer[resultcount] = { "ack",     ///< acknowledge (command recognized and executed)
                                          "nak",     ///< not acknowledge (kommando not recognized)
                                          "busy",    ///< device busy
                                          "erraut",  ///< error authorization (command recognized but client is not auth. )
                                          "errval",  ///< error value (command recognized but incorrect parameter)
                                          "errxml",  ///< error xml (used xml file was incorrect)
                                          "errpath", ///< error path
                                          "errexec", ///< error execution
                                      };
enum eSCPINodeType { isNode = 1, isQuery = 2, isCmd = 4, isCmdwP = 8, isXMLCmd = 16};
enum eSCPINode { Node, Query, Cmd, CmdwP };
}

class cSCPIPrivate;

typedef QMap<QString /*short name*/, QStringList /*long name list*/> ScpiAmbiguityMap;
typedef std::function<ScpiAmbiguityMap(ScpiAmbiguityMap)> ScpiAmbiguityIgnoreFunction;

class SCPI_EXPORT cSCPI
{
public:
    cSCPI();
    virtual ~cSCPI();

    void insertScpiCmd(const QStringList& parentNodeNames, cSCPIObject* pSCPIObject);

    void delSCPICmds(const QString& cmd);
    void delSCPICmds(cSCPICommand &cmd);

    cSCPIObject* getSCPIObject(const QString& input) const;
    cSCPIObject* getSCPIObject(const cSCPICommand &input) const;

    void exportSCPIModelXML(QString &sxml, QMap<QString, QString> modelListBaseEntry = QMap<QString, QString>());
    void createFullNonNodeNameList(QList<QStringList> &childNameList);

    ScpiAmbiguityMap checkAmbiguousShortNames(ScpiAmbiguityIgnoreFunction ignoreFunction = [](ScpiAmbiguityMap inMap) { return inMap;});
private:
    cSCPIPrivate *d_ptr;
};

#endif // SCPI_H
