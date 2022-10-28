#ifndef XMLELEMITERATOR_H
#define XMLELEMITERATOR_H

#include <QDomElement>
#include <QStringList>

class XmlElemIterator
{
public:
    XmlElemIterator(QDomElement elem);
    QDomElement getElem();
    QStringList getParentPath();

    XmlElemIterator &operator++();
    XmlElemIterator operator++(int);
    bool operator== (const XmlElemIterator &other) const;
    bool operator!= (const XmlElemIterator &other) const;
private:
    QDomElement getNextElem();
    void findParentsNeighbour();
    QDomElement m_elem;
    QList<QDomElement> m_parentList;
};

#endif // XMLELEMITERATOR_H
