#ifndef SCPIOBJECT_H
#define SCPIOBJECT_H

#include "SCPI_global.h"
#include "scpiobject_p.h"

/**
  @brief pure virtual base class to derive classes for command handling objects
  */
class SCPISHARED_EXPORT cSCPIObject {
public:
    cSCPIObject();
    cSCPIObject(QString& name, quint8 type);
    /**
      @b The command execution, returns false if invalid.
      @param[in] sInput  the received command.
      @param[out] sOutput generated output, may be empty ... see main description
      */
    virtual bool executeSCPI(const QString& sInput, QString& sOutput) = 0;
    /**
      @b Returns the object's name as const reference.
      */
    const QString& getName();
    /**
      @b Sets the object's name to name.
      @param[in] name
      */
    void setName(const QString& name);
    /**
      @b Return the object's type.
      */
    quint8 getType();
    /**
      @b Sets the object's type to type.
      */
    void setType(const quint8 type);

private:
    cSCPIObjectPrivate *d_ptr;
};



#endif // SCPIOBJECT_H
