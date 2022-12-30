#include "xmlelemcomparetag.h"

bool XmlElemCompareTag::compare(const QDomElement &elem1, const QDomElement &elem2)
{
    return elem1.tagName() == elem2.tagName();
}
