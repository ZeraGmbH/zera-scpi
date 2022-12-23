#include "scpifullcmdcheckerfortest.h"

ScpiDummyEchoObject::ScpiDummyEchoObject(const QString &name, quint8 type) :
    cSCPIObject(name, type)
{
}

bool ScpiDummyEchoObject::executeSCPI(const QString &sInput, QString &sOutput)
{
    sOutput = sInput;
    return true;
}

ScpiFullCmdCheckerForTest::ScpiFullCmdCheckerForTest() :
    m_scpiTree("TestScpi")
{
}

ScpiFullCmdCheckerForTest::ScpiFullCmdCheckerForTest(QString scpiNodePath, quint8 scpiType) :
    m_scpiTree("TestScpi")
{
    addCommand(scpiNodePath, scpiType);
}

void ScpiFullCmdCheckerForTest::addCommand(QString scpiNodePath, quint8 scpiType)
{
    QStringList path = scpiNodePath.split(":", Qt::SkipEmptyParts);
    if(path.count() > 0) {
        QString nodeName = path.takeLast();
        m_scpiTree.insertScpiCmd(path, new ScpiDummyEchoObject(nodeName, scpiType));
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
        if(scpiCmd.isQuery(paramCount) && (type & SCPI::isQuery) == 0)
            match = false;
        if(scpiCmd.isCommand(paramCount)) {
            if(paramCount == 0 && (type & SCPI::isCmd) == 0)
                match = false;
            if(paramCount > 0 && (type & SCPI::isCmdwP) == 0)
                match = false;
        }
    }
    return match;
}
