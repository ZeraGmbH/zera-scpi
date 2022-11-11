#include "xmlqtdomdociteration.h"
#include "xmlelemiterstrategytree.h"

bool XmlQtDomDocIteration::loadXml(const QString &xml, bool fatalOnInvalidXml)
{
    bool loaded = m_doc.setContent(xml);
    if(!loaded && fatalOnInvalidXml)
        qFatal("Invalid XML\n%s", qPrintable(xml));
    return loaded;
}

void XmlQtDomDocIteration::setXmlDomDoc(QDomDocument xmlDoc)
{
    m_doc = xmlDoc;
}

bool XmlQtDomDocIteration::isEmpty()
{
    return root().getElem() == QDomElement();
}

XmlElemIter XmlQtDomDocIteration::root(XmlElemIterStrategyPtr &&iterStrategy)
{
    return XmlElemIter(std::move(iterStrategy), m_doc.documentElement());
}

bool XmlQtDomDocIteration::findElem(QStringList tagSearchPath, QDomElement &foundElem)
{
    if(tagSearchPath.isEmpty())
        return false;
    QString childName = tagSearchPath.takeFirst();
    foundElem = m_doc.firstChildElement(childName);
    while(!foundElem.isNull() && !tagSearchPath.isEmpty()) {
        childName = tagSearchPath.takeFirst();
        foundElem = foundElem.firstChildElement(childName);
    }
    return !foundElem.isNull();
}

XmlElemIter XmlQtDomDocIteration::find(QStringList tagSearchPath, XmlElemIterStrategyPtr &&iterStrategy)
{
    QDomElement foundElem;
    findElem(tagSearchPath, foundElem);
    return XmlElemIter(std::move(iterStrategy), foundElem);
}

QString XmlQtDomDocIteration::getDocType()
{
    return m_doc.doctype().name();
}
