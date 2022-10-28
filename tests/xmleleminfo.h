#ifndef XMLELEMINFO_H
#define XMLELEMINFO_H

#include "xmlelemiterator.h"
#include <QStringList>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <functional>

class XmlElemInfo
{
public:
    bool loadXml(const QString &xml);
    int getElemCount();
    bool findElem(QStringList nodeSearchPath, QDomElement &foundElem);
    XmlElemIterator begin();
    XmlElemIterator end();
private:
    void recurseElemsForCount(QDomNode node, const std::function<void ()> &countElemFunction);
    bool recurseElemsFind(QDomNode node, const QStringList &parentPath, const std::function<bool (const QDomElement &, QStringList)> &perElemFindFunction);
    QDomDocument m_doc;
};

#endif // XMLELEMINFO_H
