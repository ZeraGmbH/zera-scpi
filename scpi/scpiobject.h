#ifndef SCPIOBJECT_H
#define SCPIOBJECT_H

#include "SCPI_global.h"
#include <QMap>

class cSCPIObjectPrivate;

class SCPI_EXPORT cSCPIObject {
public:
    cSCPIObject();
    cSCPIObject(const QString &name, quint8 type);
    virtual ~cSCPIObject();
    virtual bool executeSCPI(const QString& sInput, QString& sOutput) = 0;
    const QString getName() const;
    quint8 getType() const;

    void setXmlAttribute(const QString &key, const QString &value);
    typedef QMap<QString, QString> XmlKeyValueMap;
    const XmlKeyValueMap &getXmlAttibuteMap();
private:
    cSCPIObjectPrivate *d_ptr;
};

#endif // SCPIOBJECT_H
