#include "parse.h"

cParse::cParse() :
    m_delimiter(" :?;"),
    m_whitespace(" :;")
{
    m_keyw.reserve(256);
}

const QString &cParse::GetKeyword(const QChar** s)
{
    bool escape = false;
    m_keyw.resize(0);
    m_keyw.reserve(256);
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
           else if (m_delimiter.contains(tc, Qt::CaseInsensitive))
               break; // if next char is delimiter, we are ready
           m_keyw.append(tc);
       }
       (*s)++;
    }
    return m_keyw; // keyword without delimiter
}

QChar cParse::GetChar(const QChar** s)
{
    ignoreWhitespace(s);
    return **s; // return = 0 or char != whitespace
}

const QString cParse::SetDelimiter(const QString &delimiter)
{
    QString r = m_delimiter;
    m_delimiter = delimiter;
    return(r); // return old delimiter ....
}

const QString cParse::SetWhiteSpace(const QString &whiteSpace)
{
    QString r = m_whitespace;
    m_whitespace = whiteSpace;
    return(r); // return old whitespace
}

void cParse::ignoreWhitespace(const QChar **s)
{
    for (;;) {
        if ((**s).isNull())
            break;
        if ( !m_whitespace.contains(**s, Qt::CaseInsensitive) )
            break; // if next char is whitespace
        (*s)++; // pointer to next char
    }
}
