#include "xmlcomparer.h"

bool XmlComparer::loadXml(const QString &xml)
{
    return m_doc.setContent(xml);
}

int XmlComparer::getNodeCount()
{
    QDomNode firstNode = m_doc.firstChild();
    int nodeNums = 0;
    traverseElements(firstNode, QStringList(), [&](const QDomElement& childElem, QStringList parentPath) -> bool
    {
        Q_UNUSED(childElem)
        Q_UNUSED(parentPath)
        nodeNums++;
        return true;
    });
    return nodeNums;
}

bool XmlComparer::findElem(QStringList nodeSearchPath, QDomElement &foundElem)
{
    bool found = false;
    QDomNode firstNode = m_doc.firstChild();
    if(!nodeSearchPath.isEmpty()) {
        traverseElements(firstNode, QStringList(), [&](const QDomElement& childElem, QStringList parentPath) -> bool
        {
            QStringList childPath = parentPath + QStringList(childElem.tagName());
            found = nodeSearchPath == childPath;
            return !found;
        });
    }
    return found;
}

bool XmlComparer::traverseElements(QDomNode node, const QStringList &parentPath, const std::function<bool (const QDomElement&, QStringList)>& perNodeAction)
{
    bool continueTraverse = true;
    if (node.isElement()) {
        QDomElement elem = node.toElement();
        continueTraverse = perNodeAction(elem, parentPath);
        QStringList childrenparentPath = parentPath + QStringList(elem.tagName());
        for(QDomNode childNode = elem.firstChild(); continueTraverse && !childNode.isNull(); childNode = childNode.nextSibling()) {
            continueTraverse = traverseElements(childNode, childrenparentPath, perNodeAction);
        }
    }
    return continueTraverse;
}
