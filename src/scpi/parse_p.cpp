#include "parse_p.h"

cParsePrivate::cParsePrivate()
{
    delimiter = " :?;";
    whitespace = " :;";
}


cParsePrivate::~cParsePrivate()
{
}


const QString& cParsePrivate::GetKeyword(QChar** s) {
    QChar tc;
    bool escape;

    escape = false;
    keyw ="";
    ignoreWhitespace(s);

    for (;;)
    {
       tc = **s;
       if (tc.isNull()) break; // we are at end of string
       if (!escape && tc == QChar('\\'))
           escape = true;
       else
       {
           if (escape)
               escape = false;
           else
               if ( delimiter.contains(tc,Qt::CaseInsensitive) ) break; // if next char is delimiter, we are ready

           keyw += tc;
       }

       (*s)++;
    }

    /*
    QChar tc=GetChar(s);
    if (!tc.isNull()) { // whitespace skipped and first character found
        for (;;) {
            if ( delimiter.contains(tc,Qt::CaseInsensitive) ) break; // if next char is delimiter, we are ready
            keyw+=tc;
            tc=**s; // get next char
            // if ( delimiter.contains(tc,Qt::CaseInsensitive) ) break; // if next char is delimiter, we are ready
            if (tc.isNull()) break; // we are at end of string
            (*s)++; // pointer to next char
        }
    }
    */

    return keyw; // keyword without delimiter
}


QChar cParsePrivate::GetChar(QChar** s)
{
    ignoreWhitespace(s);
    return **s; // return = 0 or char != whitespace
}


const QString cParsePrivate::SetDelimiter(const QString s) {
    QString r = delimiter;
    delimiter = s;
    return(r); // return old delimiter ....
}


const QString cParsePrivate::SetWhiteSpace(const QString s) {
    QString r = whitespace;
    whitespace = s;
    return(r); // return old whitespace
}


void cParsePrivate::ignoreWhitespace(QChar** s)
{
    for (;;)
    {
        if ((**s).isNull()) break;
        if ( !whitespace.contains(**s,Qt::CaseInsensitive) ) break; // if next char is whitespace
        (*s)++; // pointer to next char
    }
}

