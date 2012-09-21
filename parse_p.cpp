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
    keyw ="";
    QChar tc=GetChar(s);
    if (!tc.isNull()) { // whitespace skipped and first character found
        for (;;) {
            keyw+=tc;
            tc=**s; // get next char
            if ( delimiter.contains(tc,Qt::CaseInsensitive) ) break; // if next char is delimiter, we are ready
            if (tc.isNull()) break; // we are at end of string
            (*s)++; // pointer to next char
        }
    };
    return(keyw); // keyword without delimiter
}


QChar cParsePrivate::GetChar(QChar** s)	{
    QChar tc=0;
    if (!(**s).isNull()) { // test string end
        do {
            tc = **s;
            (*s)++;
        }
        while ( (!tc.isNull()) && (whitespace.contains(tc,Qt::CaseInsensitive)) ); // ignore whitespace chars
    }
    return(tc); // return = 0 or char != whitespace
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

