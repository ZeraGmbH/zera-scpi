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
    cSCPIPrivate();
    void insertScpiCmd(const QStringList& parentNodeNames, cSCPIObject* pSCPIObject);
    void delSCPICmds(const QString& cmd);
    cSCPIObject* getSCPIObject(const QString& input);
    void exportSCPIModelXML(QString &sxml, QMap<QString, QString> modelListBaseEntry);
    void createFullNonNodeNameList(QList<QStringList> &childNameList);
    QStringList checkDoubleShortNames();

private:
    ScpiNode *findParentAndCreatePath(const QStringList& parentNodePath);
    void findAndDeleteNode(const QStringList &nodePath);
    QStringList removeEmptyNodes(const QStringList& parentNodeNames);
    static QList<QStringList> makeShort(QList<QStringList> childNameListFull);

    ScpiNode m_invisibleRootNode;
    cParse m_Parser;
};

#endif // SCPI_P_H
