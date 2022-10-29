#include "xmleleminfo.h"

bool XmlElemInfo::loadXml(const QString &xml, bool fatalOnInvalidXml)
{
    bool loaded = m_doc.setContent(xml);
    if(!loaded && fatalOnInvalidXml) {
        qFatal("Invalid XML\n%s", qPrintable(xml));
    }
    return loaded;
}

int XmlElemInfo::getElemCount()
{
    int nodeNums = 0;
    for(auto iter=begin(); iter!=end(); ++iter)
        nodeNums++;
    return nodeNums;
}

QDomDocumentType XmlElemInfo::getDocType()
{
    return m_doc.doctype();
}

XmlElemIterator XmlElemInfo::begin()
{
    return { m_doc.documentElement() };
}

XmlElemIterator XmlElemInfo::end()
{
    return { QDomElement() };
}

bool XmlElemInfo::findElem(QStringList nodeSearchPath, QDomElement &foundElem)
{
    if(nodeSearchPath.isEmpty())
        return false;
    QString childName = nodeSearchPath.takeFirst();
    foundElem = m_doc.firstChildElement(childName);
    while(!foundElem.isNull() && !nodeSearchPath.isEmpty()) {
        childName = nodeSearchPath.takeFirst();
        foundElem = foundElem.firstChildElement(childName);
    }
    return !foundElem.isNull();
}
