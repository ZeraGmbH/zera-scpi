#include "xmlelemiterstrategysort.h"

QDomElement XmlElemIterStrategySort::next(QDomElement current)
{
    if(m_nextChildren.isEmpty()) {
        if(m_potentialNextParents.isEmpty()) {
            if(collectChildren(current))
                return nextChild();
        }
        else {
            while(!m_potentialNextParents.isEmpty()) {
                if(collectChildren(m_potentialNextParents.takeFirst()))
                    return nextChild();
            }
        }
    }
    else
        return nextChild();
    return QDomElement();
}

bool XmlElemIterStrategySort::collectChildren(QDomElement elem)
{
    QDomElement firstChild = elem.firstChildElement();
    m_currentParentPath.append(elem.tagName());
    for(auto child = firstChild; !child.isNull(); child=child.nextSiblingElement())
        m_nextChildren[child.tagName()].append(child);
    return !m_nextChildren.isEmpty();
}

QDomElement XmlElemIterStrategySort::nextChild()
{
    QString firstTag = m_nextChildren.firstKey();
    QDomElement next = m_nextChildren[firstTag].takeFirst();
    if(m_nextChildren[firstTag].isEmpty())
        m_nextChildren.remove(firstTag);
    m_potentialNextParents.append(next);
    return next;
}

