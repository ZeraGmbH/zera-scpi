#include "xmlelemiterstrategylist.h"

QDomElement XmlElemIterStrategyList::next(QDomElement current)
{
    return current.nextSiblingElement(current.tagName());
}
