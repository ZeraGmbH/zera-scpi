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

void cSCPI::insertScpiCmd(const QStringList& parentNodeNames, cSCPIObject *pSCPIObject)
{
    d_ptr->insertScpiCmd(parentNodeNames, pSCPIObject);
}

void cSCPI::delSCPICmds(const QString &cmd)
{
    d_ptr->delSCPICmds(cmd);
}

void cSCPI::delSCPICmds(cSCPICommand &cmd)
{
    delSCPICmds(cmd.getCommand());
}

cSCPIObject* cSCPI::getSCPIObject(const QString &input) const
{
    return d_ptr->getSCPIObject(input);
}

cSCPIObject* cSCPI::getSCPIObject(const cSCPICommand &input) const
{
    return getSCPIObject(input.getCommand());
}

void cSCPI::exportSCPIModelXML(QString &sxml)
{
    d_ptr->exportSCPIModelXML(sxml);
}
