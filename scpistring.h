#ifndef CSCPISTRING_H
#define CSCPISTRING_H

#include <QString>

/**
  @brief
  cSCPIString is a class to provide support for scpi conform command abreviations
  */
class cSCPIString : public QString
{

public:
    /**
      @b Constructs a null scpistring. Null scpistrings are also empty.
      */
    cSCPIString();
    /**
      @b Constructs a scpistring from s.
      @param[in] s
      */
    cSCPIString(const QString& s);
    /**
      @b Constructs a scpistring initialized with the 8-bit string s. The given const char pointer is converted to Unicode.
      @param[in] s
      */
    cSCPIString(const char* s);
    /**
      @b Returns true if string s is equal to this string; otherwise returns false.

      The comparison also includes the comparison with the SCPI abreviation.
      */
    bool operator == (const QString& toCompare);  // overloading == operator for decoding scpi abreviations
    /**
      @b Returns true if string s is not equal to this string; otherwise returns false.

      The comparison also includes the comparison with the SCPI abreviation.
      @param[in] s
      */
    bool operator != (const QString& s) { return ( !(*this == s)); } // what ?
    /**
      @b Assigns s to this scpistring and returns a reference to this scpistring.
      @param[in] s
      */
    cSCPIString& operator = (const QString& s);
    /**
      @b Assigns s to this scpistring and returns a reference to this scpistring.
      @param[in] s
      */
    cSCPIString& operator = (const char* s);

private:
    /**
      @b Generates the scpi abreviation of s and returns a reference to it.
      @param[in] s
      */
    QString& m_ScpiAbreviation(const QString& s);
    /**
      @b the SCPI abbreviation example (RES)ource
      */
    QString m_SAbreviation;

};

#endif // CSCPISTRING_H
