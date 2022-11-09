#include "xmlelemiteratorlist.h"

XmlElemIteratorList::XmlElemIteratorList(QDomElement elem) :
    XmlElemIteratorTemplate(elem)
{
}

void XmlElemIteratorList::next()
{
    m_elem = m_elem.nextSiblingElement(m_elem.tagName());
}
