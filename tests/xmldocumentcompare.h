#ifndef XMLDOCUMENTCOMPARE_H
#define XMLDOCUMENTCOMPARE_H

#include "xmlqtdomdociteration.h"
#include <QString>
#include <QDomElement>
#include <functional>

typedef std::function<bool (const QDomElement, const QDomElement)> XmlElemCompareFunc;

class XmlDocumentCompare
{
public:
    XmlDocumentCompare(XmlElemCompareFunc elemCompareFunction = m_defaultElemCompareFunc);
    bool compareXml(QString xml1, QString xml2, bool fatalOnInvalidXml=false);
private:
    bool compareDocTypes(XmlQtDomDocIteration doc1, XmlQtDomDocIteration doc2);
    bool compareAllElems(XmlQtDomDocIteration doc1, XmlQtDomDocIteration doc2);

    XmlElemCompareFunc m_elemCompareFunc;
    static XmlElemCompareFunc m_defaultElemCompareFunc;
};

#endif // XMLDOCUMENTCOMPARE_H
