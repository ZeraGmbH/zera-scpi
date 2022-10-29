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
    int getElemCount();
    QString getDocType();
    XmlElemIterator begin();
    XmlElemIterator end();
    bool findElem(QStringList elemSearchPath, QDomElement &foundElem);
    bool isEmpty();
private:
    QDomDocument m_doc;
};

#endif // XMLDOCUMENT_H
