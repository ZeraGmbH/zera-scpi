#include "xmlelemiterator.h"

XmlElemIterator::XmlElemIterator(QDomElement elem) :
    XmlElemIteratorTemplate(elem)
{
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
        findIteratedParentsNeighbor();
    else
        m_elem.clear();
    return m_elem;
}

void XmlElemIterator::findIteratedParentsNeighbor()
{
    m_elem.clear();
    while(m_elem.isNull() && !m_iteratedParentList.isEmpty()) {
        QDomElement iteratedParent = m_iteratedParentList.takeLast();
        m_elem = iteratedParent.nextSiblingElement();
    }
}

void XmlElemIterator::makeChildCurrent(QDomElement child)
{
    m_iteratedParentList.append(m_elem);
    m_elem = child;
}
