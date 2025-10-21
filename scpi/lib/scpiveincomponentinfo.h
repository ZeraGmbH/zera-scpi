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
    explicit ScpiVeinComponentInfo(const QString &model, const QString &cmd,
                                   int cmdTypeMask, // e.g SCPI::isQuery|SCPI::isCmdwP
                                   const QString &veinComponentName,
                                   SCPI::eSCPIEntryType entryType = SCPI::isComponent);
    void appendSCPIInfo(QJsonArray &jsArr);
    void setRpcParams(QString rpcParams);

private:
    const QString m_sSCPIModel;
    const QString m_sSCPICmd;
    const QString m_sSCPICmdType;
    const QString m_veinComponentName;
    const QString m_sRefType;
    QString m_rpcParams;
};

#endif // SCPIVEINCOMPONENTINFO_H
