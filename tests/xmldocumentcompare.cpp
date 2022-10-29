#include "xmldocumentcompare.h"
#include "xmlelemcompareattribs.h"
#include "xmlelemcomparetext.h"

XmlDocumentCompare::XmlDocumentCompare(XmlElemCompareFunc elemCompareFunction) :
    m_elemCompareFunc(elemCompareFunction)
{
}

XmlElemCompareFunc XmlDocumentCompare::m_stdElemCompareFunc =
[](const QDomElement& elem1, const QDomElement& elem2) -> bool
{
    return
        XmlElemCompareAttribs::compare(elem1, elem2) &&
        XmlElemCompareText::compare(elem1, elem2);
};

bool XmlDocumentCompare::compareXml(QString xml1, QString xml2, bool fatalOnInvalidXml)
{
    XmlDocument doc1;
    doc1.loadXml(xml1, fatalOnInvalidXml);
    XmlDocument doc2;
    doc2.loadXml(xml2, fatalOnInvalidXml);

    if(isXmlEmptyOrInvalid(doc1) && isXmlEmptyOrInvalid(doc2))
        return true;
    if(doc1.getElemCount() != doc2.getElemCount())
        return false;
    if(!compareDocTypes(doc1, doc2)) {
        return false;
    }

    for(auto iter=doc1.begin(); iter!=doc1.end(); ++iter) {
        QDomElement elem1 = iter.getElem();
        QStringList tagpath1 = iter.getParentPath() + QStringList(elem1.tagName());
        QDomElement elem2;
        if(!doc2.findElem(tagpath1, elem2))
            return false;
        if(!m_elemCompareFunc(elem1, elem2))
            return false;
    }
    return true;
}

bool XmlDocumentCompare::isXmlEmptyOrInvalid(XmlDocument doc)
{
    return doc.begin() == doc.end();
}

bool XmlDocumentCompare::compareDocTypes(XmlDocument doc1, XmlDocument doc2)
{
    return doc1.getDocType() == doc2.getDocType();
}
