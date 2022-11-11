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
    bool addChildren(QDomElement elem);
    QDomElement addChildrenAndGetFirst(QDomElement elem);
    QDomElement addChildrenOfHandledChildrenAndGetFirst();

    QMap<QString /*tag*/, QList<QDomElement>> m_children;
    QList<QDomElement> m_handledChildren;
};

#endif // XMLELEMITERSTRATEGYSORT_H
