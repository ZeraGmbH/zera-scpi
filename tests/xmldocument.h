#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include "xmlelemiter.h"
#include "xmlelemiterstrategytree.h"
#include "xmlelemiterstrategylist.h"
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
    XmlElemIter find(QStringList tagSearchPath, std::unique_ptr<XmlElemIterStrategy> &&iterStrategy= std::make_unique<XmlElemIterStrategyList>());

    bool addOrFindElem(QStringList tagPath, QDomElement &insertedOrFoundElem);

    QString getDocType();
private:
    QDomDocument m_doc;
};

#endif // XMLDOCUMENT_H
