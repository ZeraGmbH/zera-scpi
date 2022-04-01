#include "scpicommand_p.h"
#include "parse.h"


cSCPICommandPrivate::cSCPICommandPrivate()
{
}


cSCPICommandPrivate::cSCPICommandPrivate(const QString& str)
{
    m_sCommand = str;
    setCmdParamList();
}


cSCPICommandPrivate::~cSCPICommandPrivate()
{
}


void cSCPICommandPrivate::operator = (const QString& str)
{
    m_sCommand = str;
    setCmdParamList();
}


QTextStream& operator >> (QTextStream& ts, cSCPICommandPrivate& cmd)
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


void cSCPICommandPrivate::setCmdParamList()
{
    cParse Parser;
    QChar* pInput;
    pInput = (QChar*) m_sCommand.data();
    m_sCommandStr = "";
    QString keyw;

    Parser.SetDelimiter(" :;"); // we leave a ? on the command

    do
    {
        keyw = Parser.GetKeyword(&pInput); // we fetch all keywords from command
        m_sCommandStr += keyw;
        if (*pInput  == ':')
            m_sCommandStr += ":";
    } while (*pInput  == ':');

    // the command is finished

    Parser.SetDelimiter(";"); // we start with fetching parameters
    Parser.SetWhiteSpace(" ");
    m_sParamList.clear();

    do
    {
        keyw = Parser.GetKeyword(&pInput);
        if (*pInput == ';')
        {
            m_sParamList.append(keyw);
            pInput++;
        }
    } while (*pInput != 0);

}


bool cSCPICommandPrivate::isQuery() const
{
    return ( (m_sCommandStr.contains("?")) && (m_sParamList.count() == 0));
}


bool cSCPICommandPrivate::isQuery(quint8 anzParameter) const
{
    return ( (m_sCommandStr.contains("?")) && (m_sParamList.count() == anzParameter));
}


bool cSCPICommandPrivate::isCommand(quint8 anzParameter) const
{
    return ( (!m_sCommandStr.contains("?")) && (m_sParamList.count() == anzParameter));
}
