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
    return compareAllElems(doc1, doc2) && compareAllElems(doc2, doc1);
}

bool XmlDocumentCompare::compareDocTypes(XmlDocument doc1, XmlDocument doc2)
{
    return doc1.getDocType() == doc2.getDocType();
}

bool XmlDocumentCompare::compareAllElems(XmlDocument doc1, XmlDocument doc2)
{
    for(auto iter1=doc1.begin(); iter1!=doc1.end(); ++iter1) {
        QDomElement elem1 = iter1.getElem();
        QStringList tagPath1 = iter1.getParentPath() + QStringList(elem1.tagName());
        if(!compareElemsAt(tagPath1, doc1, doc2))
           return false;
    }
    return true;
}

bool XmlDocumentCompare::compareElemsAt(QStringList tagPath, XmlDocument doc1, XmlDocument doc2)
{
    XmlElemIteratorList arrIter1 = doc1.find(tagPath);
    XmlElemIteratorList arrIter2 = doc2.find(tagPath);
    for(;arrIter1!=doc1.end(); ++arrIter1,++arrIter2) {
        if(arrIter2 == doc2.end())
            return false;
        if(!m_elemCompareFunc(arrIter1.getElem(), arrIter2.getElem()))
            return false;
    }
    return true;
}
