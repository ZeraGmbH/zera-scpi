#ifndef SCPIVEINCOMPONENTINFO_H
#define SCPIVEINCOMPONENTINFO_H

#include <QString>
#include <QJsonArray>

namespace SCPI {
enum eSCPIEntryType {
    isComponent,
    isCatalog
};
}

class ScpiVeinComponentInfo
{
public:
    explicit ScpiVeinComponentInfo(QString model,
                                   QString cmd,
                                   int cmdTypeMask, // e.g SCPI::isQuery|SCPI::isCmdwP
                                   QString veinComponentName,
                                   SCPI::eSCPIEntryType entryType);
    void appendSCPIInfo(QJsonArray &jsArr);

private:
    const QString m_sSCPIModel;
    const QString m_sSCPICmd;
    const QString m_sSCPICmdType;
    const QString m_veinComponentName;
    const QString m_sRefType;
};

#endif // SCPIVEINCOMPONENTINFO_H
