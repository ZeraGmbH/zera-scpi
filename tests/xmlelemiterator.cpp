#include "xmlelemiterator.h"

XmlElemIterator::XmlElemIterator(QDomElement elem)
{
    m_elem = elem;
}

QDomElement XmlElemIterator::getElem()
{
    return m_elem;
}

QStringList XmlElemIterator::getParentPath()
{
    QStringList parentPath;
    QDomNode parent = m_elem.parentNode();
    while(parent.isElement()) {
        parentPath.prepend(parent.toElement().tagName());
        parent = parent.parentNode();
    }
    return parentPath;
}

XmlElemIterator &XmlElemIterator::operator++()
{
    m_elem = getNextElem();
    return *this;
}

XmlElemIterator XmlElemIterator::operator++(int)
{
    XmlElemIterator r = *this;
    m_elem = getNextElem();
    return r;
}

bool XmlElemIterator::operator==(const XmlElemIterator &other) const
{
    return other.m_elem == m_elem;
}

bool XmlElemIterator::operator!=(const XmlElemIterator &other) const
{
    return other.m_elem != m_elem;
}

QDomElement XmlElemIterator::getNextElem()
{
    QDomElement neighbor = m_elem.nextSiblingElement();
    QDomElement child = m_elem.firstChildElement();
    if(!child.isNull())
        makeChildCurrent(child);
    else if(!neighbor.isNull())
        m_elem = neighbor;
    else if(!m_iteratedParentList.isEmpty())
        findAlreadyIteratedParentsNeighbor();
    else
        m_elem.clear();
    return m_elem;
}

void XmlElemIterator::findAlreadyIteratedParentsNeighbor()
{
    m_elem.clear();
    while(m_elem.isNull() && !m_iteratedParentList.isEmpty()) {
        QDomElement handledParent = m_iteratedParentList.takeLast();
        m_elem = handledParent.nextSiblingElement();
    }
}

void XmlElemIterator::makeChildCurrent(QDomElement child)
{
    m_iteratedParentList.append(m_elem);
    m_elem = child;
}
