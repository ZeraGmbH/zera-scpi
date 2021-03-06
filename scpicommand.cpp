#include "scpicommand.h"
#include "scpicommand_p.h"


cSCPICommand::cSCPICommand()
    :d_ptr(new cSCPICommandPrivate)
{
}


cSCPICommand::cSCPICommand(const QString& str)
    :d_ptr(new cSCPICommandPrivate(str))
{
}

cSCPICommand::cSCPICommand(const cSCPICommand &other)
  :d_ptr(new cSCPICommandPrivate(other.getCommand()))
{
}


cSCPICommand::~cSCPICommand()
{
    delete d_ptr;
}


cSCPICommand& cSCPICommand::operator = (const QString& str)
{
    *d_ptr = str;
    return *this;
}


QTextStream& operator >> (QTextStream& ts, cSCPICommand& cmd)
{
    return ts >> *(cmd.d_ptr);
}


const QString cSCPICommand::getCommand() const
{
    return d_ptr->m_sCommand;
}


const QString cSCPICommand::getCommandStr() const
{
    return d_ptr->m_sCommandStr;
}


quint32 cSCPICommand::getParamCount() const
{
    return d_ptr->m_sParamList.count();
}


QString cSCPICommand::getParam(qint32 pos) const
{
    QString s;
    if (pos < d_ptr->m_sParamList.count())
        return d_ptr->m_sParamList[pos];
    else
        return s;
}


QString cSCPICommand::getParam() const
{
    QString s, cmd;

    s = d_ptr->m_sCommand;
    cmd = d_ptr->m_sCommandStr;

    s = s.right(s.length() - cmd.length());

    return s;
}

const QStringList &cSCPICommand::getParamList() const
{
  return d_ptr->m_sParamList;
}

bool cSCPICommand::isQuery() const
{
    return d_ptr->isQuery();
}


bool cSCPICommand::isQuery(quint8 anzParameter) const
{
    return d_ptr->isQuery(anzParameter);
}


bool cSCPICommand::isCommand(quint8 anzParameter) const
{
    return d_ptr->isCommand(anzParameter);
}
