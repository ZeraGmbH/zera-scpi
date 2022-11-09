#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include "xmlelemiteratortemplate.h"
#include "xmlelemiter.h"
#include "xmlelemiterstrategytree.h"
#include <QStringList>
#include <QDomDocument>
#include <QDomElement>
#include <memory>

class XmlDocument
{
public:
    bool loadXml(const QString &xml, bool fatalOnInvalidXml = false);
    bool isEmpty();
    XmlElemIter begin(std::unique_ptr<XmlElemIterStrategy> &&iterStrategy = std::make_unique<XmlElemIterStrategyTree>());

    bool findElem(QStringList tagSearchPath, QDomElement &foundElem);
    XmlElemIterator find(QStringList tagSearchPath);
    XmlElemIter find(QStringList tagSearchPath, std::unique_ptr<XmlElemIterStrategy> &&iterStrategy);

    bool addOrFindElem(QStringList tagPath, QDomElement &insertedOrFoundElem);

    QString getDocType();
private:
    QDomDocument m_doc;
};

#endif // XMLDOCUMENT_H
