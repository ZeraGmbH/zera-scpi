#include "xmldocumentcompare.h"
#include "xmlelemcompareattribs.h"
#include "xmlelemcomparetext.h"
#include <QHash>

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
    QHash<QStringList, int> tagPathVisitCounts;
    for(auto iter1=doc1.root(); !iter1.isEnd(); iter1.next()) {
        QDomElement elem1 = iter1.getElem();
        QStringList parentPath1 = iter1.getParentPath();
        QStringList tagPath1 = parentPath1 + QStringList(elem1.tagName());
        if(!compareElemsAt(tagPath1, tagPathVisitCounts[parentPath1], doc1, doc2))
           return false;
        tagPathVisitCounts[tagPath1]++;
    }
    return true;
}

bool XmlDocumentCompare::compareElemsAt(QStringList tagPath, int parentVisitCount, XmlDocument doc1, XmlDocument doc2)
{
    XmlElemIter arrIter1 = doc1.find(tagPath);
    XmlElemIter arrIter2 = doc2.find(tagPath);
    if(parentVisitCount > 0) {
        QString tagName = tagPath.last();
        QDomElement elem1 = arrIter1.getElem().parentNode().toElement();
        QDomElement elem2 = arrIter2.getElem().parentNode().toElement();
        while(parentVisitCount > 0) {
            elem1 = elem1.nextSiblingElement(tagName);
            elem2 = elem2.nextSiblingElement(tagName);
            parentVisitCount--;
        }
    }
    for(;!arrIter1.isEnd(); arrIter1.next(), arrIter2.next()) {
        if(arrIter2.isEnd())
            return false;
        if(!m_elemCompareFunc(arrIter1.getElem(), arrIter2.getElem()))
            return false;
    }
    return true;
}
