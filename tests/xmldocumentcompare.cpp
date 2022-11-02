#include "xmldocumentcompare.h"
#include "xmlelemcompareattribs.h"
#include "xmlelemcomparetext.h"

XmlDocumentCompare::XmlDocumentCompare(XmlElemCompareFunc elemCompareFunction) :
    m_elemCompareFunc(elemCompareFunction)
{
}

XmlElemCompareFunc XmlDocumentCompare::m_defaultElemCompareFunc =
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
    if(doc1.isEmpty() && doc2.isEmpty())
        return true;
    if(doc1.isEmpty() || doc2.isEmpty())
        return false;
    if(!compareDocTypes(doc1, doc2))
        return false;
    return compareElems(doc1, doc2) && compareElems(doc2, doc1);
}

bool XmlDocumentCompare::compareDocTypes(XmlDocument doc1, XmlDocument doc2)
{
    return doc1.getDocType() == doc2.getDocType();
}

bool XmlDocumentCompare::compareElems(XmlDocument doc1, XmlDocument doc2)
{
    for(auto iter=doc1.begin(); iter!=doc1.end(); ++iter) {
        QDomElement elem1 = iter.getCurrElem();
        QStringList tagpath1 = iter.getParentPath() + QStringList(elem1.tagName());
        QDomElement elem2;
        if(!doc2.findElem(tagpath1, elem2))
            return false;
        if(!m_elemCompareFunc(elem1, elem2))
            return false;
    }
    return true;
}
