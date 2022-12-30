#ifndef XMLELEMCOMPARETAG_H
#define XMLELEMCOMPARETAG_H

#include <QDomElement>

class XmlElemCompareTag
{
public:
    static bool compare(const QDomElement &elem1, const QDomElement &elem2);
};

#endif // XMLELEMCOMPARETAG_H
