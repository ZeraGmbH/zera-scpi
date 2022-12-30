#ifndef SCPITESTOBLECTSTUB_H
#define SCPITESTOBLECTSTUB_H

#include "scpiobject.h"

class SCPITestObjectStub : public cSCPIObject
{
public:
    SCPITestObjectStub(QString name, quint8 type) : cSCPIObject(name, type) { }
    bool executeSCPI(const QString& sInput, QString& sOutput) override
    {
        return true;
    }
};

#endif // SCPITESTOBLECTSTUB_H
