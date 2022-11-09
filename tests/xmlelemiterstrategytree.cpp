#include "xmlelemiterstrategytree.h"

QDomElement XmlElemIterStrategyTree::next(QDomElement current)
{
    QDomElement neighbor = current.nextSiblingElement();
    QDomElement child = current.firstChildElement();
    QDomElement next;
    if(!child.isNull()) {
        m_iteratedParentList.append(current);
        next = child;
    }
    else if(!neighbor.isNull())
        next = neighbor;
    else if(!m_iteratedParentList.isEmpty())
        next = findIteratedParentsNeighbor();
    return next;
}

QDomElement XmlElemIterStrategyTree::findIteratedParentsNeighbor()
{
    QDomElement next;
    while(next.isNull() && !m_iteratedParentList.isEmpty()) {
        QDomElement iteratedParent = m_iteratedParentList.takeLast();
        next = iteratedParent.nextSiblingElement();
    }
    return next;
}
