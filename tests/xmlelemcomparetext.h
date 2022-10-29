#ifndef XMLELEMCOMPARETEXT_H
#define XMLELEMCOMPARETEXT_H

#include <QDomElement>

class XmlElemCompareText
{
public:
    static bool compare(const QDomElement &elem1, const QDomElement &elem2);
};

#endif // XMLELEMCOMPARETEXT_H
