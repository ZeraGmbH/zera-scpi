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


void cSCPI::genSCPICmd(const QStringList& parentnodeNames, cSCPIObject *pSCPIObject)
{
    d_ptr->genSCPICmd( parentnodeNames, pSCPIObject);
}


void cSCPI::genSCPICmd(QMap<QString, QList<cSCPIObject *> > SCPIObjectMap)
{
    d_ptr->genSCPICmd(SCPIObjectMap);
}


void cSCPI::genSCPICmd(QMap<QString, QMap<QString, QList<cSCPIObject *> > > SCPIObjectMap)
{
    d_ptr->genSCPICmd(SCPIObjectMap);
}


void cSCPI::genSCPICmd(QMap<QString, QMap<QString, QMap<QString, QList<cSCPIObject*> > > >SCPIObjectMap)
{
    d_ptr->genSCPICmd(SCPIObjectMap);
}


void cSCPI::genSCPICmd(QMap<QString, QMap<QString, QMap<QString, QMap<QString, QList<cSCPIObject*> > > > > SCPIObjectMap)
{
    d_ptr->genSCPICmd(SCPIObjectMap);
}


void cSCPI::genSCPICmd(QMap<QString, QMap<QString, QMap<QString, QMap<QString, QMap<QString, QList<cSCPIObject*> > > > > > SCPIObjectMap)
{
    d_ptr->genSCPICmd(SCPIObjectMap);
}


void cSCPI::clearSCPICmdList()
{
    d_ptr->clearSCPICmdList();
}


void cSCPI::delSCPICmds(const QString &cmd)
{
    d_ptr->delSCPICmds(cmd);
}


void cSCPI::delSCPICmds(cSCPICommand &cmd)
{
   delSCPICmds(cmd.getCommand());
}


cSCPIObject* cSCPI::getSCPIObject(const QString &input, QString &Param)
{
    return d_ptr->getSCPIObject( input, Param);
}


cSCPIObject* cSCPI::getSCPIObject(cSCPICommand &input)
{
    QString dummy;
    return this->getSCPIObject(input.getCommand(), dummy);
}


QStandardItemModel* cSCPI::getSCPIModel()
{
    return &(d_ptr->m_SCPIModel);
}


void cSCPI::exportSCPIModelXML(QString &sxml)
{
    d_ptr->exportSCPIModelXML(sxml);
}


bool cSCPI::importSCPIModelXML(QIODevice *ioDevice)
{
    return d_ptr->importSCPIModelXML(ioDevice);
}
