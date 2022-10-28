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
    QDomNode firstNode = m_doc.firstChild();
    if(!nodeSearchPath.isEmpty()) {
        recurseElemsFind(firstNode, QStringList(), [&](const QDomElement& childElem, QStringList parentPath) -> bool
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

bool XmlElemInfo::recurseElemsFind(QDomNode node, const QStringList &parentPath, const std::function<bool (const QDomElement&, QStringList)>& perElemFindFunction)
{
    bool continueWalk = true;
    if (node.isElement()) {
        QDomElement elem = node.toElement();
        continueWalk = perElemFindFunction(elem, parentPath);
        QStringList childrenparentPath = parentPath + QStringList(elem.tagName());
        for(QDomNode childNode = elem.firstChild(); continueWalk && !childNode.isNull(); childNode = childNode.nextSibling()) {
            continueWalk = recurseElemsFind(childNode, childrenparentPath, perElemFindFunction);
        }
    }
    return continueWalk;
}
