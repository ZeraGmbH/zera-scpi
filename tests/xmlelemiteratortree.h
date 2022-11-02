#ifndef XMLELEMITERATORTREE_H
#define XMLELEMITERATORTREE_H

#include <xmlelemiteratortemplate.h>
#include <QDomElement>
#include <QList>

class XmlElemIteratorTree : public XmlElemIteratorTemplate
{
public:
    XmlElemIteratorTree(QDomElement elem);
    QDomElement getNextElem();
private:
    void findIteratedParentsNeighbor();
    void makeChildCurrent(QDomElement child);
    QList<QDomElement> m_iteratedParentList;
};

#endif // XMLELEMITERATORTREE_H
