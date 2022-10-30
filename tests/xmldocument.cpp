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

bool XmlDocument::findElem(QStringList tagSearchPath, QDomElement &foundElem)
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

QString XmlDocument::getDocType()
{
    return m_doc.doctype().name();
}
