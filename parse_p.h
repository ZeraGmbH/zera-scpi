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
    const QString& GetKeyword(QChar** s); // read the next keyword from string
    QChar GetChar(QChar** s); // read next character string
    const QString SetDelimiter(const QString s);
    const QString SetWhiteSpace(const QString s);
private:
    QString keyw;
    QString delimiter;
    QString whitespace;
};

#endif // PARSE_P_H
