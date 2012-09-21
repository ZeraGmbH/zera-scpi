// implementation für keyword parser

#include "parse.h"
#include "parse_p.h"


cParse::cParse()
    :d_ptr(new cParsePrivate())
{
}


cParse::~cParse()
{
    delete d_ptr;
}


const QString &cParse::GetKeyword(QChar** s)
{
    return d_ptr->GetKeyword(s);
}


QChar cParse::GetChar(QChar** s)
{
    return d_ptr->GetChar(s);
}


const QString cParse::SetDelimiter(const QString s)
{
    return d_ptr->SetDelimiter(s);
}


const QString cParse::SetWhiteSpace(const QString s)
{
    return d_ptr->SetWhiteSpace(s);
}
