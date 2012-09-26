#ifndef SCPIOBJECT_P_H
#define SCPIOBJECT_P_H

#include <QString>

/**
  @brief
  Holds the data for cSCPIObject.
  */
class cSCPIObjectPrivate {
public:
    /**
      @b name of the object
      */
    QString m_sName;
    /**
      @b type of the object
      @see SCPI::eSCPINodeType
      */
    quint8 m_nType;
};


#endif // SCPIOBJECT_P_H
