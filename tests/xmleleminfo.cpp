#include "xmleleminfo.h"
#include <QDomDocument>

bool XmlElemInfo::loadXml(const QString &xml)
{
    return m_doc.setContent(xml);
}

int XmlElemInfo::getElemCount()
{
    QDomNode firstNode = m_doc.firstChild();
    int nodeNums = 0;
    recursiveElemWalk(firstNode, QStringList(), [&](const QDomElement& childElem, QStringList parentPath) -> bool
    {   // perElemFunction
        Q_UNUSED(childElem)
        Q_UNUSED(parentPath)
        nodeNums++;
        return true;
    });
    return nodeNums;
}

bool XmlElemInfo::findElem(QStringList nodeSearchPath, QDomElement &foundElem)
{
    bool found = false;
    QDomNode firstNode = m_doc.firstChild();
    if(!nodeSearchPath.isEmpty()) {
        recursiveElemWalk(firstNode, QStringList(), [&](const QDomElement& childElem, QStringList parentPath) -> bool
        {   // perElemFunction
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

bool XmlElemInfo::recursiveElemWalk(QDomNode node, const QStringList &parentPath, const std::function<bool (const QDomElement&, QStringList)>& perElemFunction)
{
    bool continueWalk = true;
    if (node.isElement()) {
        QDomElement elem = node.toElement();
        continueWalk = perElemFunction(elem, parentPath);
        QStringList childrenparentPath = parentPath + QStringList(elem.tagName());
        for(QDomNode childNode = elem.firstChild(); continueWalk && !childNode.isNull(); childNode = childNode.nextSibling()) {
            continueWalk = recursiveElemWalk(childNode, childrenparentPath, perElemFunction);
        }
    }
    return continueWalk;
}
