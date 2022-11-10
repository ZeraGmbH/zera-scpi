#include "xmlelemiterstrategysort.h"

QDomElement XmlElemIterStrategySort::next(QDomElement current)
{
    if(!m_nextSortedChildren.isEmpty())
        return getNextChild();
    else if(!m_handledChildren.isEmpty())
        return createGrandChildrenSetAndGetFirst();
    return createChildrenSetAndGetFirst(current);
}

QDomElement XmlElemIterStrategySort::getNextChild()
{
    QString firstTag = m_nextSortedChildren.firstKey();
    QDomElement next = m_nextSortedChildren[firstTag].takeFirst();
    if(m_nextSortedChildren[firstTag].isEmpty())
        m_nextSortedChildren.remove(firstTag);
    m_handledChildren.append(next);
    return next;
}

bool XmlElemIterStrategySort::collectChildren(QDomElement elem)
{
    for(auto child = elem.firstChildElement(); !child.isNull(); child=child.nextSiblingElement())
        m_nextSortedChildren[child.tagName()].append(child);
    return !m_nextSortedChildren.isEmpty();
}

QDomElement XmlElemIterStrategySort::createChildrenSetAndGetFirst(QDomElement current)
{
    if(collectChildren(current))
        return getNextChild();
    return QDomElement();
}

QDomElement XmlElemIterStrategySort::createGrandChildrenSetAndGetFirst()
{
    while(!m_handledChildren.isEmpty()) {
        if(collectChildren(m_handledChildren.takeFirst()))
            return getNextChild();
    }
    return QDomElement();
}
