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
      @b The full command with all parameters
      */
    QString m_sCommand;
    /**
      @b All parameters of the command
      @note Empty parameter are not omitted
      */
    QStringList m_sParamList;
private:
    /**
      @b Private memberfunction to parse the command and separate the parameters.
      */
    void setParamList();
};

#endif // SCPICOMMAND_P_H
