#include "xmlelemcomparetext.h"

bool XmlElemCompareText::compare(const QDomElement &elem1, const QDomElement &elem2)
{
    if(elem1.hasAttributes() || elem2.hasAttributes())
        return true;
    return elem1.text() == elem2.text();
}
