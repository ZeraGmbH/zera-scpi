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

QDomElement XmlElemIterator::getNextElem()
{
    QDomElement neighbor = m_elem.nextSiblingElement();
    QDomElement child = m_elem.firstChildElement();
    if(!child.isNull()) {
        m_parentList.append(m_elem);
        m_elem = child;
    }
    else if(!neighbor.isNull()) {
        m_elem = neighbor;
    }
    else if(!m_parentList.isEmpty()) {
        findParentsNeighbour();
    }
    else {
        m_elem = QDomElement();
    }
    return m_elem;
}

void XmlElemIterator::findParentsNeighbour()
{
    m_elem = QDomElement();
    while(m_elem.isNull() && !m_parentList.isEmpty()) {
        QDomElement handledParent = m_parentList.takeLast();
        m_elem = handledParent.nextSiblingElement();
    }
}

bool XmlElemIterator::operator==(const XmlElemIterator &other) const
{
    return other.m_elem == m_elem;
}

bool XmlElemIterator::operator!=(const XmlElemIterator &other) const
{
    return other.m_elem != m_elem;
}


