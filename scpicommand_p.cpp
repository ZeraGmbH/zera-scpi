#include "scpicommand_p.h"
#include "parse.h"


cSCPICommandPrivate::cSCPICommandPrivate()
{
}


cSCPICommandPrivate::cSCPICommandPrivate(const QString& str)
{
    m_sCommand = str;
    setParamList();
}


cSCPICommandPrivate::~cSCPICommandPrivate()
{
}


void cSCPICommandPrivate::operator = (const QString& str)
{
    m_sCommand = str;
    setParamList();
}


QTextStream& operator >> (QTextStream& ts, cSCPICommandPrivate& cmd)
{
    cmd.m_sCommand = "";
    while (!ts.atEnd()) // we read the whole stream for 1 command
    {
        QString s;
        ts >> s;
        s += " ";
        cmd.m_sCommand += s;
    }
    cmd.setParamList();
    return ts;
}


void cSCPICommandPrivate::setParamList()
{
    cParse Parser;
    QChar* pInput;
    pInput = (QChar*) m_sCommand.data();
    QString keyw;
    bool more;

    do
    {
        keyw = Parser.GetKeyword(&pInput); // we fetch all keywords from command
    } while (*pInput  == ':');

    // the command is finished

    Parser.SetDelimiter(";"); // we start with fetching parameters
    m_sParamList.clear();

    do
    {
        keyw = Parser.GetKeyword(&pInput);
        if ((more = (keyw != "")) == true)
            m_sParamList.append(keyw);
    } while (more);
}
