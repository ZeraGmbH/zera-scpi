#include "xmldocument.h"

bool XmlDocument::loadXml(const QString &xml, bool fatalOnInvalidXml)
{
    bool loaded = m_doc.setContent(xml);
    if(!loaded && fatalOnInvalidXml) {
        qFatal("Invalid XML\n%s", qPrintable(xml));
    }
    return loaded;
}

bool XmlDocument::isEmpty()
{
    return begin() == end();
}

XmlElemIterator XmlDocument::begin()
{
    return { m_doc.documentElement() };
}

XmlElemIterator XmlDocument::end()
{
    return { QDomElement() };
}

bool XmlDocument::findElem(QStringList elemSearchPath, QDomElement &foundElem)
{
    if(elemSearchPath.isEmpty())
        return false;
    QString childName = elemSearchPath.takeFirst();
    foundElem = m_doc.firstChildElement(childName);
    while(!foundElem.isNull() && !elemSearchPath.isEmpty()) {
        childName = elemSearchPath.takeFirst();
        foundElem = foundElem.firstChildElement(childName);
    }
    return !foundElem.isNull();
}

int XmlDocument::getElemCount()
{
    int nodeNums = 0;
    for(auto iter=begin(); iter!=end(); ++iter)
        nodeNums++;
    return nodeNums;
}

QString XmlDocument::getDocType()
{
    return m_doc.doctype().name();
}
