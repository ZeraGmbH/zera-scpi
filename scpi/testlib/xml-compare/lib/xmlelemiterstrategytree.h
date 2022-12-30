#ifndef XMLELEMITERSTRATEGYTREE_H
#define XMLELEMITERSTRATEGYTREE_H

#include "xmlelemiter.h"
#include <QList>

class XmlElemIterStrategyTree : public XmlElemIterStrategy
{
public:
    QDomElement next(QDomElement current) override;
private:
    QDomElement findIteratedParentsNeighbor();
    QList<QDomElement> m_iteratedParentList;
};

#endif // XMLELEMITERSTRATEGYTREE_H
