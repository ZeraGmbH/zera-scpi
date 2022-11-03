#include "xmlelemiteratortemplate.h"

XmlElemIteratorTemplate::XmlElemIteratorTemplate(QDomElement elem) :
    m_elem(elem)
{

}

QDomElement XmlElemIteratorTemplate::getElem()
{
    return m_elem;
}

QStringList XmlElemIteratorTemplate::getParentPath()
{
    QStringList parentPath;
    QDomNode parent = m_elem.parentNode();
    while(parent.isElement()) {
        parentPath.prepend(parent.toElement().tagName());
        parent = parent.parentNode();
    }
    return parentPath;
}

XmlElemIteratorTemplate &XmlElemIteratorTemplate::operator++()
{
    m_elem = getNextElem();
    return *this;
}

bool XmlElemIteratorTemplate::operator==(const XmlElemIteratorTemplate &other) const
{
    return other.m_elem == m_elem;
}

bool XmlElemIteratorTemplate::operator!=(const XmlElemIteratorTemplate &other) const
{
    return other.m_elem != m_elem;
}
