#ifndef SCPIFULLCMDCHECKERFORTEST_H
#define SCPIFULLCMDCHECKERFORTEST_H

#include "scpi.h"
#include "scpiobject.h"

// helper classes to test genereated commands - e.g zera-classes server tasks

class ScpiDummyEchoObject : public cSCPIObject
{
public:
    ScpiDummyEchoObject(const QString &name, quint8 type);
    bool executeSCPI(const QString& sInput, QString& sOutput) override;
};

class ScpiFullCmdCheckerForTest
{
public:
    ScpiFullCmdCheckerForTest();
    void addCommand(QStringList parentNodeNames, QString nodeName,
                    quint8 scpiType);
    void addCommand(QString scpiNodePath,
                    quint8 scpiType);
    bool matches(QString cmd);
private:
    cSCPI m_scpiTree;
};

#endif // SCPIFULLCMDCHECKERFORTEST_H
