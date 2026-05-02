#ifndef SCPI_P_H
#define SCPI_P_H

#include "parse.h"
#include "scpi.h"
#include "scpinode.h"

class cSCPIPrivate
{
public:
    cSCPIPrivate();
    virtual ~cSCPIPrivate();
    void insertScpiCmd(const QStringList& parentNodeNames, const ScpiObjectPtr &scpiObject);
    void delSCPICmds(const QString& cmd);
    ScpiObjectPtr getSCPIObject(const QString& input);
    void exportSCPIModelXML(QString &sxml, const QMap<QString, QString> &modelListBaseEntry);
    void createFullNonNodeNameList(QList<QStringList> &childNameList);
    ScpiAmbiguityMap checkAmbiguousShortNames(ScpiAmbiguityIgnoreFunction ignoreFunction);
private:
    ScpiNodePtr findParentAndCreatePath(const QStringList& parentNodePath);
    void findAndDeleteNode(const QStringList &nodePath);
    static QStringList removeEmptyNodes(const QStringList& parentNodeNames);
    static QList<QStringList> makeShortNameList(QList<QStringList> childNameListFull);
    static ScpiAmbiguityMap createAllShortLongListMap(QList<QStringList> childNameListFull, QList<QStringList> childNameListShort);
    static ScpiAmbiguityMap filterAmbigous(ScpiAmbiguityMap allShortLong);

    ScpiNodePtr m_invisibleRootNode;
    cParse m_Parser;
};

#endif // SCPI_P_H
