#include "scpifullcmdcheckerfortest.h"

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

ScpiFullCmdCheckerForTest::ScpiFullCmdCheckerForTest() :
    m_scpiTree("TestScpi")
{
}

ScpiFullCmdCheckerForTest::ScpiFullCmdCheckerForTest(QString scpiNodePath, quint8 scpiType, int paramCountExpected) :
    m_scpiTree("TestScpi")
{
    addCommand(scpiNodePath, scpiType, paramCountExpected);
}

void ScpiFullCmdCheckerForTest::addCommand(QString scpiNodePath, quint8 scpiType, int paramCountExpected)
{
    QStringList path = scpiNodePath.split(":", Qt::SkipEmptyParts);
    if(path.count() > 0) {
        QString nodeName = path.takeLast();
        m_scpiTree.insertScpiCmd(path, new ScpiParamCheckerDelegate(nodeName, scpiType, paramCountExpected));
    }
}

bool ScpiFullCmdCheckerForTest::matches(QString cmd)
{
    cSCPIObject *object = m_scpiTree.getSCPIObject(cmd);
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
            ScpiParamCheckerDelegate* scpiObject = static_cast<ScpiParamCheckerDelegate*>(object);
            QString scpiOutput;
            match = scpiObject->executeSCPI(cmd, scpiOutput);
        }
    }
    else
        qWarning("Scpi command '%s not found!", qPrintable(cmd));
    return match;
}
