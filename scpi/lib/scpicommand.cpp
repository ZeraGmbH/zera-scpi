#include "scpicommand.h"
#include "parse.h"

cSCPICommand::cSCPICommand()
{
}

cSCPICommand::cSCPICommand(const QString& str) :
    m_sCommand(str)
{
    setCmdParamList();
}

cSCPICommand::cSCPICommand(const cSCPICommand &other) :
    m_sCommand(other.getCommand())
{
    setCmdParamList();
}

cSCPICommand& cSCPICommand::operator = (const QString& str)
{
    m_sCommand = str;
    setCmdParamList();
    return *this;
}

QTextStream& operator >> (QTextStream& ts, cSCPICommand& cmd)
{
    cmd.m_sCommand = "";
    while (!ts.atEnd()) // we read the whole stream for 1 command
    {
        QString s;
        ts >> s;
        s += " "; // we append blank character because stream removes them
        cmd.m_sCommand += s;
    }
    cmd.setCmdParamList();
    return ts;
}

const QString cSCPICommand::getCommand() const
{
    return m_sCommand;
}

const QString cSCPICommand::getCommandStr() const
{
    return m_sCommandStr;
}

quint32 cSCPICommand::getParamCount() const
{
    return m_sParamList.count();
}

QString cSCPICommand::getParam(qint32 pos) const
{
    if (pos < m_sParamList.count())
        return m_sParamList[pos];
    else
        return QString();
}

QString cSCPICommand::getParam() const
{
    QString s = m_sCommand;
    QString cmd = m_sCommandStr;
    s = s.right(s.length() - cmd.length());
    return s;
}

const QStringList &cSCPICommand::getParamList() const
{
  return m_sParamList;
}

bool cSCPICommand::isQuery() const
{
    return ( (m_sCommandStr.contains("?")) && (m_sParamList.count() == 0));
}

bool cSCPICommand::isQuery(quint8 anzParameter) const
{
    return ( (m_sCommandStr.contains("?")) && (m_sParamList.count() == anzParameter));
}

bool cSCPICommand::isCommand(quint8 anzParameter) const
{
    return ( (!m_sCommandStr.contains("?")) && (m_sParamList.count() == anzParameter));
}

void cSCPICommand::setCmdParamList()
{
    const QChar* pInput = m_sCommand.data();
    m_sCommandStr = "";

    cParse Parser;
    Parser.SetDelimiter(" :;"); // we leave a ? on the command

    QString keyw;
    do {
        keyw = Parser.GetKeyword(&pInput); // we fetch all keywords from command
        m_sCommandStr += keyw;
        if (*pInput  == ':')
            m_sCommandStr += ":";
    } while (*pInput  == ':');

    // the command is finished

    Parser.SetDelimiter(";"); // we start with fetching parameters
    Parser.SetWhiteSpace(" ");
    m_sParamList.clear();

    do {
        keyw = Parser.GetKeyword(&pInput);
        if (*pInput == ';') {
            m_sParamList.append(keyw);
            pInput++;
        }
    } while (*pInput != 0);
}
