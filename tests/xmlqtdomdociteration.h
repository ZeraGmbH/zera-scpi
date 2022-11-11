#ifndef XMLQTDOMDOCITERATION_H
#define XMLQTDOMDOCITERATION_H

#include "xmlelemiterstrategytree.h"
#include "xmlelemiterstrategylist.h"
#include <QStringList>
#include <QDomDocument>
#include <QDomElement>

class XmlQtDomDocIteration
{
public:
    bool loadXml(const QString &xml, bool fatalOnInvalidXml = false);
    void setXmlDomDoc(QDomDocument xmlDoc);

    bool isEmpty();
    QString getDocType();

    XmlElemIter root(XmlElemIterStrategyPtr &&iterStrategy = std::make_unique<XmlElemIterStrategyTree>());

    XmlElemIter find(QStringList tagSearchPath, XmlElemIterStrategyPtr &&iterStrategy= std::make_unique<XmlElemIterStrategyList>());
    bool findElem(QStringList tagSearchPath, QDomElement &foundElem);

private:
    QDomDocument m_doc;
};

#endif // XMLQTDOMDOCITERATION_H
