#ifndef SCPITESTOBLECT_H
#define SCPITESTOBLECT_H

#include "scpiobject.h"

class SCPITestObject : public cSCPIObject
{
public:
    SCPITestObject(QString name, quint8 type) : cSCPIObject(name, type) { }
    bool executeSCPI(const QString& sInput, QString& sOutput) override
    {
        return true;
    }
};

#endif // SCPITESTOBLECT_H
