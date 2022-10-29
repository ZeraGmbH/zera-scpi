#ifndef XMLDOCUMENTCOMPARE_H
#define XMLDOCUMENTCOMPARE_H

#include "xmldocument.h"
#include <QString>
#include <QDomElement>
#include <functional>

typedef std::function<bool (const QDomElement, const QDomElement)> XmlElemCompareFunc;

class XmlDocumentCompare
{
public:
    XmlDocumentCompare(XmlElemCompareFunc elemCompareFunction = m_stdElemCompareFunc);
    bool compareXml(QString xml1, QString xml2, bool fatalOnInvalidXml=false);
private:
    bool compareDocTypes(XmlDocument doc1, XmlDocument doc2);
    bool compareElems(XmlDocument doc1, XmlDocument doc2);

    XmlElemCompareFunc m_elemCompareFunc;
    static XmlElemCompareFunc m_stdElemCompareFunc;
};

#endif // XMLDOCUMENTCOMPARE_H
