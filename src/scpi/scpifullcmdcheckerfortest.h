#ifndef SCPIFULLCMDCHECKERFORTEST_H
#define SCPIFULLCMDCHECKERFORTEST_H

#include "scpi.h"
#include "scpiobject.h"

// helper classes to test genereated commands - e.g zera-classes server tasks

class ScpiParamCheckerDelegate : public cSCPIObject
{
public:
    ScpiParamCheckerDelegate(const QString &name, quint8 type, int paramCountExpected);
    bool executeSCPI(const QString& input, QString& output) override;
private:
    int m_paramCountExpected;
};

class ScpiFullCmdCheckerForTest
{
public:
    ScpiFullCmdCheckerForTest();
    ScpiFullCmdCheckerForTest(QString scpiNodePath, quint8 scpiType, int paramCountExpected = -1);
    void addCommand(QString scpiNodePath, quint8 scpiType, int paramCountExpected = -1);
    bool matches(QString cmd);
private:
    cSCPI m_scpiTree;
};

#endif // SCPIFULLCMDCHECKERFORTEST_H
