// header datei für keyword parser

#ifndef PARSE_H
#define PARSE_H

#include <qstring.h>
#include "SCPI_global.h"


class cParsePrivate;


/**
  @brief
  cParse offers an parser for analysing command input lines, especially for SCPI commands.

  It searches for whole words using free selectable delimiters and whitespaces.
  Default whitespace characters are " ", ":" .
  */
class SCPISHARED_EXPORT cParse { // searches for whole words using free selectable delimiters and whitespaces
public:
    /**
      @b contruct a default configured parser.
      */
    cParse();
    ~cParse();
    /**
      @b Searches for the next keyword starting from s.
      @param[in] s the string to search from
      */
    const QString& GetKeyword(QChar** s);
    /**
      @b Returns the next character starting from s ignoring whitespace.
      @param[in] s the characters to read from
      */
    QChar GetChar(QChar** s); // read next character string
    /**
      @b Sets the new delimiter characters from s for parsing and returns the old one.
      */
    const QString SetDelimiter(const QString s);
    /**
      @b Sets the new whitespace characters from s for parsing and returns the old one.
      */
    const QString SetWhiteSpace(const QString s);
private:
    cParsePrivate *d_ptr;
};

#endif
