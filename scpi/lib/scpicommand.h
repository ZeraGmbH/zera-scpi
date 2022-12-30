#ifndef SCPICOMMAND_H
#define SCPICOMMAND_H

#include <QString>
#include <QStringList>
#include <QTextStream>
#include "scpi_export.h"

class cSCPICommandPrivate;

/**
  @brief
  cSCPICommand is a class to support parsing of scpi commands.

  An entered command is parsed and parameters behind the command are stored in a qstringlist and can be read.
  */
class SCPI_EXPORT cSCPICommand
{
public:
    cSCPICommand();
    cSCPICommand(const QString& str);
    cSCPICommand(const cSCPICommand &other);
    ~cSCPICommand();
    cSCPICommand& operator = (const QString& str);
    /**
      @b Sets the scpicommand from a QTextStream and returns a reference to QTextStream.
      */
    friend QTextStream& operator >> (QTextStream& ts, cSCPICommand& cmd);
    /**
      @b Returns the whole scpicommand.
      */
    const QString getCommand() const;
    /**
      @b Only returns the command part of the scpicommand.
      */
    const QString getCommandStr() const;
    /**
      @b Returns the number of parameters in the command.
      */
    quint32 getParamCount() const;
    /**
      @b Returns the parameter from pos.
      */
    QString getParam(qint32 pos) const;
    /**
      @b Returns the whole parameter text after the command without last ;
      */
    QString getParam() const;

    const QStringList &getParamList() const;
    /**
      @b Returns true if command is a pure query without additional parameter.
      */
    bool isQuery() const;
    /**
      @b Returns true if command is a query with (anzParameter) parameter.
      */
    bool isQuery(quint8 anzParameter) const;
    /**
      @b Returns true if command is a command with (anzParameter) parameter.
      */
    bool isCommand(quint8 anzParameter) const;

private:
    /**
      @b D'pointer to the private library internal structure

      this is used to hide the internal structure, and thus make the library ABI safe
      */
    cSCPICommandPrivate *d_ptr;
};

#endif // SCPICOMMAND_H
