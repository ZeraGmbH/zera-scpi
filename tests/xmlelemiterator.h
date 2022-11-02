#ifndef XMLELEMITERATOR_H
#define XMLELEMITERATOR_H

#include <xmlelemiteratortemplate.h>
#include <QDomElement>
#include <QList>

class XmlElemIterator : public XmlElemIteratorTemplate
{
public:
    XmlElemIterator(QDomElement elem);
    QDomElement getNextElem();
private:
    void findIteratedParentsNeighbor();
    void makeChildCurrent(QDomElement child);
    QList<QDomElement> m_iteratedParentList;
};

#endif // XMLELEMITERATOR_H
