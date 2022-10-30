#include <QObject>
#include <QModelIndex>
#include <QDomDocument>
#include <QDomDocumentType>
#include <QDomElement>
#include "scpi.h"
#include "scpi_p.h"

cSCPI::cSCPI(QString interfaceName)
    :d_ptr(new cSCPIPrivate(interfaceName))
{
}

cSCPI::~cSCPI()
{
    delete d_ptr;
}

void cSCPI::insertScpiCmd(const QStringList& parentnodeNames, cSCPIObject *pSCPIObject)
{
    d_ptr->insertScpiCmd( parentnodeNames, pSCPIObject);
}

void cSCPI::delSCPICmds(const QString &cmd)
{
    d_ptr->delSCPICmds(cmd);
}

void cSCPI::delSCPICmds(cSCPICommand &cmd)
{
   delSCPICmds(cmd.getCommand());
}

cSCPIObject* cSCPI::getSCPIObject(const QString &input, bool caseSensitive) const
{
    return d_ptr->getSCPIObject(input, caseSensitive);
}

cSCPIObject* cSCPI::getSCPIObject(const cSCPICommand &input, bool caseSensitive) const
{
    return this->getSCPIObject(input.getCommand(), caseSensitive);
}

void cSCPI::exportSCPIModelXML(QString &sxml)
{
    d_ptr->exportSCPIModelXML(sxml);
}
