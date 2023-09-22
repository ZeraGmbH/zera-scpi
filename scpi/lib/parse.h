#ifndef PARSE_H
#define PARSE_H

#include "scpi_export.h"
#include <QString>

/**
  @brief
  cParse offers an parser for analysing command input lines, especially for SCPI commands.

  It searches for whole words using free selectable delimiters and whitespaces.
  Default whitespace characters are " ", ":" .
  */
class SCPI_EXPORT cParse {
public:
    cParse();
    const QString& GetKeyword(const QChar **s);
    QChar GetChar(const QChar **s); // read next character string

    const QString SetDelimiter(const QString s);
    const QString SetWhiteSpace(const QString s);
private:
    void ignoreWhitespace(const QChar **s);
    QString keyw;
    QString delimiter;
    QString whitespace;
};

#endif
