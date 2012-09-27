#ifndef PARSE_P_H
#define PARSE_P_H

#include <QString>

/**
  @brief
   The implemention for cParse.
*/
class cParsePrivate
{
public:
    cParsePrivate();
    virtual ~cParsePrivate();
    /**
      @b read the next keyword from string
      */
    const QString& GetKeyword(QChar** s);
    /**
      @b read next character string
      */
    QChar GetChar(QChar** s);
    /**
      @b Sets the new delimiter characters from s for parsing and returns the old one.
      */
    const QString SetDelimiter(const QString s);
    /**
      @b Sets the new whitespace characters from s for parsing and returns the old one.
      */
    const QString SetWhiteSpace(const QString s);
private:
    /**
      @b the keyword without delimiter
      */
    QString keyw;
    /**
      @b the delimiter for the parser
      */
    QString delimiter;
    /**
      @b the whitespace for the parser
      */
    QString whitespace;
    void ignoreWhitespace(QChar **s);
};

#endif // PARSE_P_H
