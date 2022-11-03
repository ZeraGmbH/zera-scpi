#include "xmlelemiteratorlist.h"

XmlElemIteratorList::XmlElemIteratorList(QDomElement elem) :
    XmlElemIteratorTemplate(elem)
{
}

QDomElement XmlElemIteratorList::getNextElem()
{
    return m_elem.nextSiblingElement(m_elem.tagName());
}
