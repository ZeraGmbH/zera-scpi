#ifndef SCPI_P_H
#define SCPI_P_H

#include "parse.h"
#include "scpi.h"
#include "scpinode.h"
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
    ScpiAmbiguityMap checkAmbiguousShortNames();
private:
    ScpiNode *findParentAndCreatePath(const QStringList& parentNodePath);
    void findAndDeleteNode(const QStringList &nodePath);
    QStringList removeEmptyNodes(const QStringList& parentNodeNames);
    static QList<QStringList> makeShortNameList(QList<QStringList> childNameListFull);
    ScpiAmbiguityMap createAllShortLongListMap(QList<QStringList> childNameListFull, QList<QStringList> childNameListShort);
    ScpiAmbiguityMap filterAmbigous(ScpiAmbiguityMap allShortLong);

    ScpiNode m_invisibleRootNode;
    cParse m_Parser;
};

#endif // SCPI_P_H
