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
    XmlElemIterator begin();
    XmlElemIterator end();
    bool findElem(QStringList nodeSearchPath, QDomElement &foundElem);
private:
    QDomDocument m_doc;
};

#endif // XMLELEMINFO_H
