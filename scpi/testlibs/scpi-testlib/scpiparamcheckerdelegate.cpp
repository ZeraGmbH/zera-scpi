#include "scpiparamcheckerdelegate.h"
#include "scpicommand.h"

ScpiParamCheckerDelegate::ScpiParamCheckerDelegate(const QString &name, quint8 type, int paramCountExpected) :
    cSCPIObject(name, type),
    m_paramCountExpected(paramCountExpected)
{
}

bool ScpiParamCheckerDelegate::executeSCPI(const QString &input, QString &output)
{
    cSCPICommand cmd = input;
    bool ok = m_paramCountExpected < 0 || m_paramCountExpected == cmd.getParamCount();
    if(!ok)
        qWarning("Invalid parameter count: Expected %i / received %i",
                 m_paramCountExpected, cmd.getParamCount());
    return ok;
}
