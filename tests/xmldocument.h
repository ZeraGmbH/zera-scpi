#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include "xmlelemiterator.h"
#include <QStringList>
#include <QDomDocument>
#include <QDomDocumentType>
#include <QDomElement>

class XmlDocument
{
public:
    bool loadXml(const QString &xml, bool fatalOnInvalidXml = false);
    int getElemCount();
    QDomDocumentType getDocType();
    XmlElemIterator begin();
    XmlElemIterator end();
    bool findElem(QStringList elemSearchPath, QDomElement &foundElem);
private:
    QDomDocument m_doc;
};

#endif // XMLDOCUMENT_H
