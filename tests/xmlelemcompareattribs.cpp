#include "xmlelemcompareattribs.h"

bool XmlElemCompareAttribs::compare(const QDomElement &elem1, const QDomElement &elem2)
{
    if(bothEmpty(elem1, elem2))
        return true;
    if(elem1.attributes().count() != elem2.attributes().count())
        return false;
    return compareAll(elem1, elem2);
}

bool XmlElemCompareAttribs::isEmpty(const QDomElement &elem)
{
    return !elem.hasAttributes();
}

bool XmlElemCompareAttribs::bothEmpty(const QDomElement &elem1, const QDomElement &elem2)
{
    return isEmpty(elem1) && isEmpty(elem2);
}

bool XmlElemCompareAttribs::compareAll(const QDomElement &elem1, const QDomElement &elem2)
{
    auto elem1Attribs = elem1.attributes();
    auto elem2Attribs = elem2.attributes();
    for(int i=0; i<elem1Attribs.count(); ++i) {
        QString attrKey = elem1Attribs.item(i).toAttr().name();
        if(!elem2Attribs.contains(attrKey))
            return false;
        if(elem2.attribute(attrKey) != elem1Attribs.item(i).toAttr().value())
            return false;
    }
    return true;
}
