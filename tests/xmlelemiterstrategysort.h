#ifndef XMLELEMITERSTRATEGYSORT_H
#define XMLELEMITERSTRATEGYSORT_H

#include "xmlelemiter.h"
#include <QMap>
#include <QHash>
#include <QStringList>

class XmlElemIterStrategySort : public XmlElemIterStrategy
{
public:
    QDomElement next(QDomElement current) override;
private:
    QDomElement getNextChild();
    bool collectChildren(QDomElement elem);
    QDomElement createChildrenSetAndGetFirst(QDomElement current);
    QDomElement createGrandChildrenSetAndGetFirst();

    QMap<QString /*tag*/, QList<QDomElement>> m_nextSortedChildren;
    QList<QDomElement> m_handledChildren;
};

#endif // XMLELEMITERSTRATEGYSORT_H
