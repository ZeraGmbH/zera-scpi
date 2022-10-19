#ifndef SCPIOBJECT_H
#define SCPIOBJECT_H

#include "SCPI_global.h"

class cSCPIObjectPrivate;

class SCPI_EXPORT cSCPIObject {
public:
    cSCPIObject();
    cSCPIObject(const QString &name, quint8 type);
    virtual ~cSCPIObject();
    virtual bool executeSCPI(const QString& sInput, QString& sOutput) = 0;
    const QString getName();
    quint8 getType();
private:
    cSCPIObjectPrivate *d_ptr;
};

#endif // SCPIOBJECT_H
