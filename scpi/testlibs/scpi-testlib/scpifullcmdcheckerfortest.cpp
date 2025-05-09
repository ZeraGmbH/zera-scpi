#include "scpifullcmdcheckerfortest.h"
#include "scpiparamcheckerdelegate.h"

ScpiFullCmdCheckerForTest::ScpiFullCmdCheckerForTest()
{
}

ScpiFullCmdCheckerForTest::ScpiFullCmdCheckerForTest(QString scpiNodePath, quint8 scpiType, int paramCountExpected)
{
    addCommand(scpiNodePath, scpiType, paramCountExpected);
}

void ScpiFullCmdCheckerForTest::addCommand(QString scpiNodePath, quint8 scpiType, int paramCountExpected)
{
    QStringList path = scpiNodePath.split(":", Qt::SkipEmptyParts);
    if(path.count() > 0) {
        QString nodeName = path.takeLast();
        m_scpiTree.insertScpiCmd(path, std::make_shared<ScpiParamCheckerDelegate>(nodeName, scpiType, paramCountExpected));
    }
}

bool ScpiFullCmdCheckerForTest::matches(QString cmd)
{
    ScpiObjectPtr object = m_scpiTree.getSCPIObject(cmd);
    bool match = object != nullptr;
    if(match) {
        quint8 type = object->getType();
        cSCPICommand scpiCmd = cmd;
        int paramCount = scpiCmd.getParamCount();
        if(scpiCmd.isQuery(paramCount) && (type & SCPI::isQuery) == 0) {
            qWarning("Scpi query not supported!");
            match = false;
        }
        if(scpiCmd.isCommand(paramCount)) {
            if(paramCount == 0 && (type & SCPI::isCmd) == 0) {
                qWarning("Scpi command without params not supported!");
                match = false;
            }
            if(paramCount > 0 && (type & SCPI::isCmdwP) == 0) {
                qWarning("Scpi command with params not supported!");
                match = false;
            }
        }
        if(match) {
            QString scpiOutput;
            match = object->executeSCPI(cmd, scpiOutput);
        }
    }
    else
        qWarning("Scpi command '%s not found!", qPrintable(cmd));
    return match;
}
