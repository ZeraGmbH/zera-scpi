#ifndef XMLTEXTCOMPARE_H
#define XMLTEXTCOMPARE_H

#include <QDomElement>

class XmlTextCompare
{
public:
    static bool compare(const QDomElement &elem1, const QDomElement &elem2);
};

#endif // XMLTEXTCOMPARE_H
