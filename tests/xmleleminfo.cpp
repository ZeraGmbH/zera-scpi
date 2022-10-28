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
    bool found = false;
    if(!nodeSearchPath.isEmpty()) {
        recurseElemsFind(m_doc.firstChildElement(), QStringList(), [&](const QDomElement& childElem, QStringList parentPath) -> bool
        {   // perElemFindFunction implementation
            QStringList childPath = parentPath + QStringList(childElem.tagName());
            found = nodeSearchPath == childPath;
            if(found) {
                foundElem = childElem;
            }
            return !found;
        });
    }
    return found;
}

bool XmlElemInfo::recurseElemsFind(QDomElement elem, const QStringList &parentPath, const std::function<bool (const QDomElement&, QStringList)>& perElemFindFunction)
{
    bool continueWalk = true;
    continueWalk = perElemFindFunction(elem, parentPath);
    QStringList childrenparentPath = parentPath + QStringList(elem.tagName());
    for(QDomElement childElem = elem.firstChildElement(); continueWalk && !childElem.isNull(); childElem = childElem.nextSiblingElement()) {
        continueWalk = recurseElemsFind(childElem, childrenparentPath, perElemFindFunction);
    }
    return continueWalk;
}
