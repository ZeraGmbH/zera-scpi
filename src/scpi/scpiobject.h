#ifndef SCPIOBJECT_H
#define SCPIOBJECT_H

#include "SCPI_global.h"

class cSCPIObjectPrivate;

/**
  @brief pure virtual base class to derive classes for command handling objects
  */
class SCPI_EXPORT cSCPIObject {
public:
    cSCPIObject();
    /**
      @b alternative constructor that sets the relevant values
      */
    cSCPIObject(const QString &name, quint8 type);
    virtual ~cSCPIObject();
    /**
      @b The command execution, returns false if invalid.
      @param[in] sInput  the received command.
      @param[out] sOutput generated output, may be empty ... see main description
      */
    virtual bool executeSCPI(const QString& sInput, QString& sOutput) = 0;
    /**
      @b Returns the object's name as const reference.
      */
    const QString getName();
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
    /**
      @b D'pointer to the private library internal structure

      this is used to hide the internal structure, and thus make the library ABI safe
      */
    cSCPIObjectPrivate *d_ptr;
};



#endif // SCPIOBJECT_H
