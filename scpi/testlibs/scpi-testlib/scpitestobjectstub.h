#ifndef SCPITESTOBLECTSTUB_H
#define SCPITESTOBLECTSTUB_H

#include "scpiobject.h"

class SCPITestObjectStub : public ScpiObject
{
public:
    SCPITestObjectStub(QString name, quint8 type) : ScpiObject(name, type) { }
    bool executeSCPI(const QString& sInput, QString& sOutput) override
    {
        return true;
    }
};

#endif // SCPITESTOBLECTSTUB_H
