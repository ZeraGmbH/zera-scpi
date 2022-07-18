#include "scpistring.h"

cSCPIString::cSCPIString()
    :QString()
{
}


cSCPIString::cSCPIString(const QString& s)
    :QString(s)
{
}


cSCPIString::cSCPIString(const char* s)
    :QString(s)
{
}


bool cSCPIString::operator == (const QString& toCompare)
{
    QString keyw = *this; // we create qstring from cSCPIString

    QString toCompareAbreviation = m_ScpiAbreviation(toCompare);

    bool b = ( (keyw == toCompare) || (keyw == toCompareAbreviation) );
    return (b);
}


cSCPIString& cSCPIString::operator = (const QString& s) {
    *this=(cSCPIString) s;
    return (*this);
}


cSCPIString& cSCPIString::operator = (const char* s) {
    *this=(cSCPIString) QString(s);
    return (*this);
}


QString& cSCPIString::m_ScpiAbreviation (const QString& s) {
    if (s.length() <= 4) return (QString&) s; // if command <= 4 characters -> short command = long command
    m_SAbreviation = s.left(4); // first: short command is first 4 characters
    if ( QString("AEIOU").contains(m_SAbreviation.right(1),Qt::CaseInsensitive) ) m_SAbreviation = m_SAbreviation.left(3);
    // if the 4th character is vocal -> short command is 3 characters
    return(m_SAbreviation);
}
