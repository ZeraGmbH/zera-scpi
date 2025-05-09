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
    virtual ~cSCPIPrivate();
    void insertScpiCmd(const QStringList& parentNodeNames, ScpiObjectPtr pSCPIObject);
    void delSCPICmds(const QString& cmd);
    ScpiObjectPtr getSCPIObject(const QString& input);
    void exportSCPIModelXML(QString &sxml, QMap<QString, QString> modelListBaseEntry);
    void createFullNonNodeNameList(QList<QStringList> &childNameList);
    ScpiAmbiguityMap checkAmbiguousShortNames(ScpiAmbiguityIgnoreFunction ignoreFunction);
private:
    ScpiNodePtr findParentAndCreatePath(const QStringList& parentNodePath);
    void findAndDeleteNode(const QStringList &nodePath);
    QStringList removeEmptyNodes(const QStringList& parentNodeNames);
    static QList<QStringList> makeShortNameList(QList<QStringList> childNameListFull);
    ScpiAmbiguityMap createAllShortLongListMap(QList<QStringList> childNameListFull, QList<QStringList> childNameListShort);
    ScpiAmbiguityMap filterAmbigous(ScpiAmbiguityMap allShortLong);

    ScpiNodePtr m_invisibleRootNode;
    cParse m_Parser;
};

#endif // SCPI_P_H
