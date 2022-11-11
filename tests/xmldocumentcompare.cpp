#include "xmldocumentcompare.h"
#include "xmlelemcompareattribs.h"
#include "xmlelemcomparetext.h"
#include "xmlelemcomparetag.h"
#include "xmlelemiterstrategysort.h"
#include <QHash>

XmlDocumentCompare::XmlDocumentCompare(XmlElemCompareFunc elemCompareFunction) :
    m_elemCompareFunc(elemCompareFunction)
{
}

XmlElemCompareFunc XmlDocumentCompare::m_defaultElemCompareFunc =
[](const QDomElement& elem1, const QDomElement& elem2) -> bool
{
    return
        XmlElemCompareTag::compare(elem1, elem2) &&
        XmlElemCompareAttribs::compare(elem1, elem2) &&
        XmlElemCompareText::compare(elem1, elem2);
};

bool XmlDocumentCompare::compareXml(QString xml1, QString xml2, bool fatalOnInvalidXml)
{
    XmlDocument doc1;
    doc1.loadXml(xml1, fatalOnInvalidXml);
    XmlDocument doc2;
    doc2.loadXml(xml2, fatalOnInvalidXml);
    if(doc1.isEmpty() && doc2.isEmpty())
        return true;
    if(doc1.isEmpty() || doc2.isEmpty())
        return false;
    if(!compareDocTypes(doc1, doc2))
        return false;
    return compareAllElems(doc1, doc2);
}

bool XmlDocumentCompare::compareDocTypes(XmlDocument doc1, XmlDocument doc2)
{
    return doc1.getDocType() == doc2.getDocType();
}

bool XmlDocumentCompare::compareAllElems(XmlDocument doc1, XmlDocument doc2)
{
    auto iter1 = doc1.root(std::make_unique<XmlElemIterStrategySort>());
    auto iter2 = doc2.root(std::make_unique<XmlElemIterStrategySort>());
    for(;!iter1.isEnd(); iter1.next(), iter2.next()) {
        if(!m_elemCompareFunc(iter1.getElem(), iter2.getElem()))
            return false;
    }
    if(!iter2.isEnd())
        return false;
    return true;
}
