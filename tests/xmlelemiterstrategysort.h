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
    bool collectChildren(QDomElement elem);
    QDomElement nextChild();
    QMap<QString /*tag*/, QList<QDomElement>> m_nextChildren;
    QList<QDomElement> m_potentialNextParents;
    QStringList m_currentParentPath;
};

#endif // XMLELEMITERSTRATEGYSORT_H
