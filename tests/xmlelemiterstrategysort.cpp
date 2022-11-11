#include "xmlelemiterstrategysort.h"

QDomElement XmlElemIterStrategySort::next(QDomElement current)
{
    if(!m_children.isEmpty())
        return getNextChild();
    else if(!m_handledChildren.isEmpty())
        return addChildrenOfHandledChildrenAndGetFirst();
    return addChildrenAndGetFirst(current);
}

QDomElement XmlElemIterStrategySort::getNextChild()
{
    QString firstTag = m_children.firstKey();
    QDomElement next = m_children[firstTag].takeFirst();
    if(m_children[firstTag].isEmpty())
        m_children.remove(firstTag);
    m_handledChildren.append(next);
    return next;
}

bool XmlElemIterStrategySort::addChildren(QDomElement elem)
{
    for(auto child = elem.firstChildElement(); !child.isNull(); child=child.nextSiblingElement())
        m_children[child.tagName()].append(child);
    return !m_children.isEmpty();
}

QDomElement XmlElemIterStrategySort::addChildrenAndGetFirst(QDomElement elem)
{
    if(addChildren(elem))
        return getNextChild();
    return QDomElement();
}

QDomElement XmlElemIterStrategySort::addChildrenOfHandledChildrenAndGetFirst()
{
    while(!m_handledChildren.isEmpty()) {
        if(addChildren(m_handledChildren.takeFirst()))
            return getNextChild();
    }
    return QDomElement();
}
