#ifndef SCPI_P_H
#define SCPI_P_H

#include "parse.h"
#include "scpinode.h"
#include "scpinodestaticfunctions.h"
#include <QStringList>
#include <QDomDocument>

class cSCPIPrivate
{
public:
    cSCPIPrivate(const QString& interfaceName);
    void insertScpiCmd(const QStringList& parentNodeNames, cSCPIObject* pSCPIObject);
    void delSCPICmds(const QString& cmd);
    cSCPIObject* getSCPIObject(const QString& input);
    void exportSCPIModelXML(QString &sxml);
private:
    ScpiNode *findParentAndCreatePath(const QStringList& parentNodePath);
    void findAndDeleteNode(const QStringList &nodePath);
    QStringList removeEmptyNodes(const QStringList& parentNodeNames);

    ScpiNode m_invisibleRootNode;
    QString m_interfaceName;
    cParse m_Parser;
};

#endif // SCPI_P_H
