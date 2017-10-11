#ifndef SCPICOMMAND_P_H
#define SCPICOMMAND_P_H

#include <QString>
#include <QStringList>
#include <QTextStream>


/**
  @brief
  The implemention for cSCPICommand.
  */
class cSCPICommandPrivate
{
public:
    /**
      @b Generates an empty scpicommand.
      */
    cSCPICommandPrivate();
    /**
      @b Generates a scpicommand from str.
      */
    cSCPICommandPrivate(const QString& str);
    ~cSCPICommandPrivate();
    /**
      @b Sets the scpicommand with str.
      */
    void operator = (const QString& str);
    /**
      @b Sets the scpicommand from a QTextStream.
      */
    friend QTextStream& operator >> (QTextStream& ts, cSCPICommandPrivate& cmd);
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

    /**
      @b The full command with all parameters
      */
    QString m_sCommand;
    /**
      @b Only the command part itself
      */
    QString m_sCommandStr;
    /**
      @b All parameters of the command
      @note Empty parameter are not omitted
      */
    QStringList m_sParamList;
private:
    /**
      @b Private memberfunction to parse the command and separate the parameters.
      */
    void setCmdParamList();
};

#endif // SCPICOMMAND_P_H
