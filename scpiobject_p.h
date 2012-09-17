#ifndef SCPIOBJECT_P_H
#define SCPIOBJECT_P_H

#include <QString>

/**
  @brief
   Holds the data for cSCPIObject.
*/
class cSCPIObjectPrivate {
public:
    QString m_sName;
    quint8 m_nType;
};


#endif // SCPIOBJECT_P_H
