#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include "xmlelemiterator.h"
#include <QStringList>
#include <QDomDocument>
#include <QDomElement>

class XmlDocument
{
public:
    bool loadXml(const QString &xml, bool fatalOnInvalidXml = false);
    bool isEmpty();
    XmlElemIterator begin();
    XmlElemIterator end();
    bool findElem(QStringList tagSearchPath, QDomElement &foundElem);
    bool addOrFindElem(QStringList tagPath, QDomElement &insertedOrFoundElem);
    QString getDocType();
private:
    QDomDocument m_doc;
};

#endif // XMLDOCUMENT_H
