#include "xmlelemiterstrategytree.h"

QDomElement XmlElemIterStrategyTree::next(QDomElement current)
{
    QDomElement neighbor = current.nextSiblingElement();
    QDomElement child = current.firstChildElement();
    if(!child.isNull()) {
        m_iteratedParentList.append(current);
        return child;
    }
    else if(!neighbor.isNull())
        return neighbor;
    else if(!m_iteratedParentList.isEmpty())
        return findIteratedParentsNeighbor();
    return QDomElement();
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
