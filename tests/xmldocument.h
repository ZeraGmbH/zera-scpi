#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include "xmlelemiteratortree.h"
#include "xmlelemiteratorlist.h"
#include <QStringList>
#include <QDomDocument>
#include <QDomElement>

class XmlDocument
{
public:
    bool loadXml(const QString &xml, bool fatalOnInvalidXml = false);
    bool isEmpty();
    XmlElemIteratorTree begin();
    XmlElemIteratorTree end();
    bool findElem(QStringList tagSearchPath, QDomElement &foundElem);
    XmlElemIteratorList find(QStringList tagSearchPath);
    bool addOrFindElem(QStringList tagPath, QDomElement &insertedOrFoundElem);
    QString getDocType();
private:
    QDomDocument m_doc;
};

#endif // XMLDOCUMENT_H
