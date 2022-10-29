#ifndef XMLELEMCOMPAREATTRIBS_H
#define XMLELEMCOMPAREATTRIBS_H

#include <QDomElement>

class XmlElemCompareAttribs
{
public:
    static bool compare(const QDomElement &elem1, const QDomElement &elem2);
private:
    static bool elemEmpty(const QDomElement &elem);
    static bool bothEmpty(const QDomElement &elem1, const QDomElement &elem2);
    static bool compareAll(const QDomElement &elem1, const QDomElement &elem2);
};

#endif // XMLELEMCOMPAREATTRIBS_H
