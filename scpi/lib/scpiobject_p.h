#ifndef SCPIOBJECT_P_H
#define SCPIOBJECT_P_H

#include "scpiobject.h"
#include <QString>

class cSCPIObjectPrivate {
public:
    QString m_sName;
    quint8 m_nType;
    QString m_description;
    cSCPIObject::XmlKeyValueMap m_xmlAtttibuteMap;
};


#endif // SCPIOBJECT_P_H