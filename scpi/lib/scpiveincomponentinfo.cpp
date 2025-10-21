#include "scpiveincomponentinfo.h"

ScpiVeinComponentInfo::ScpiVeinComponentInfo(const QString &model,
                                             const QString &cmd,
                                             int cmdTypeMask,
                                             const QString &veinComponentName,
                                             SCPI::eSCPIEntryType entryType) :
    m_sSCPIModel(model),
    m_sSCPICmd(cmd),
    m_sSCPICmdType(QString::number(cmdTypeMask)),
    m_veinComponentName(veinComponentName),
    m_sRefType(entryType==SCPI::isCatalog ? "1" : "0")
{
}

void ScpiVeinComponentInfo::appendSCPIInfo(QJsonArray &jsArr)
{
    QJsonArray jsonSCPIArr;
    jsonSCPIArr.append(m_sSCPIModel);
    jsonSCPIArr.append(m_sSCPICmd);
    jsonSCPIArr.append(m_sSCPICmdType);
    jsonSCPIArr.append(m_veinComponentName);
    jsonSCPIArr.append(m_sRefType);
    if(!m_rpcParams.isNull())
        jsonSCPIArr.append(m_rpcParams);

    jsArr.append(jsonSCPIArr);
}

void ScpiVeinComponentInfo::setRpcParams(QString rpcParams)
{
    m_rpcParams = rpcParams;
}
