#include "parse.h"

cParse::cParse() :
    delimiter(" :?;"),
    whitespace(" :;")
{
}

const QString &cParse::GetKeyword(const QChar** s)
{
    bool escape = false;
    keyw ="";
    ignoreWhitespace(s);
    for (;;) {
       QChar tc = **s;
       if (tc.isNull())
           break; // we are at end of string
       if (!escape && tc == QChar('\\'))
           escape = true;
       else {
           if (escape)
               escape = false;
           else if (delimiter.contains(tc, Qt::CaseInsensitive))
               break; // if next char is delimiter, we are ready
           keyw += tc;
       }
       (*s)++;
    }
    return keyw; // keyword without delimiter
}

QChar cParse::GetChar(const QChar** s)
{
    ignoreWhitespace(s);
    return **s; // return = 0 or char != whitespace
}

const QString cParse::SetDelimiter(const QString s)
{
    QString r = delimiter;
    delimiter = s;
    return(r); // return old delimiter ....
}

const QString cParse::SetWhiteSpace(const QString s)
{
    QString r = whitespace;
    whitespace = s;
    return(r); // return old whitespace
}

void cParse::ignoreWhitespace(const QChar **s)
{
    for (;;) {
        if ((**s).isNull())
            break;
        if ( !whitespace.contains(**s, Qt::CaseInsensitive) )
            break; // if next char is whitespace
        (*s)++; // pointer to next char
    }
}
