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
    cSCPICommand& operator = (const QString& str);
    friend QTextStream& operator >> (QTextStream& ts, cSCPICommand& cmd);
    /**
      @b Returns the whole scpicommand.
      */
    const QString getCommand() const;
    /**
      @b Only returns the command part of the scpicommand.
      */
    const QString getCommandStr() const;
    quint32 getParamCount() const;
    QString getParam(qint32 pos) const;
    /**
      @b Returns the whole parameter text after the command without last ;
      */
    QString getParam() const;

    const QStringList &getParamList() const;
    bool isQuery() const;
    bool isQuery(quint8 anzParameter) const;
    bool isCommand() const;
    bool isCommand(quint8 anzParameter) const;

private:
    void setCmdParamList();
    QString m_sCommand;
    QString m_sCommandStr;
    QStringList m_sParamList;
};

#endif // SCPICOMMAND_H
