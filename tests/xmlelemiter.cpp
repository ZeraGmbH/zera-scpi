#include "xmlelemiter.h"

XmlElemIter::XmlElemIter(std::unique_ptr<XmlElemIterStrategy> &&iterStrategy, QDomElement elem) :
    m_iterStrategy(std::move(iterStrategy)),
    m_elem(elem)
{

}

QDomElement XmlElemIter::getElem()
{
    return m_elem;
}

QStringList XmlElemIter::getParentPath()
{
    QStringList parentPath;
    QDomNode parent = m_elem.parentNode();
    while(parent.isElement()) {
        parentPath.prepend(parent.toElement().tagName());
        parent = parent.parentNode();
    }
    return parentPath;
}

bool XmlElemIter::isEnd()
{
    return m_elem == QDomElement();
}

void XmlElemIter::next()
{
    m_elem = m_iterStrategy->next(m_elem);
}

bool XmlElemIter::operator==(const XmlElemIter &other) const
{
    return other.m_elem == m_elem;
}

bool XmlElemIter::operator!=(const XmlElemIter &other) const
{
    return other.m_elem != m_elem;
}
