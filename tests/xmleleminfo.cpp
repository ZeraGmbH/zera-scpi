#include "xmleleminfo.h"

bool XmlElemInfo::loadXml(const QString &xml)
{
    return m_doc.setContent(xml);
}

int XmlElemInfo::getElemCount()
{
    int nodeNums = 0;
    for(auto iter=begin(); iter!=end(); ++iter) {
        nodeNums++;
    }
    return nodeNums;
}

XmlElemIterator XmlElemInfo::begin()
{
    return { m_doc.firstChild().toElement() };
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
    QDomElement elem = m_doc.firstChildElement(childName);
    while(!elem.isNull() && !nodeSearchPath.isEmpty()) {
        childName = nodeSearchPath.takeFirst();
        elem = elem.firstChildElement(childName);
    }
    if(elem.isNull())
        return false;
    foundElem = elem;
    return true;
}
