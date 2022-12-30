#ifndef SCPIFULLCMDCHECKERFORTEST_H
#define SCPIFULLCMDCHECKERFORTEST_H

#include "scpi.h"

// helper class to test generated commands - used at e.g zera-classes / tests on server tasks

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
