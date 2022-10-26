#include "xmlcomparer.h"

bool XmlComparer::loadXml(const QString &xml)
{
    return m_doc.setContent(xml);
}

int XmlComparer::getNodeCount()
{
    QDomNode firstNode = m_doc.firstChild();
    int nodeNums = 0;
    traverseElements(firstNode, QStringList(), [&](const QDomNode& childNode, QStringList parentPath) -> bool
    {
        Q_UNUSED(childNode)
        Q_UNUSED(parentPath)
        nodeNums++;
        return true;
    });
    return nodeNums;
}

bool XmlComparer::findNode(QStringList nodeSearchPath, QDomNode &foundNode)
{
    bool found = false;
    QDomNode firstNode = m_doc.firstChild();
    if(!nodeSearchPath.isEmpty()) {
        traverseElements(firstNode, QStringList(), [&](const QDomNode& childNode, QStringList parentPath) -> bool
        {
            QStringList childPath = parentPath;
            childPath.append(childNode.toElement().tagName());
            found = nodeSearchPath == childPath;
            return !found;
        });
    }
    return found;
}

bool XmlComparer::traverseElements(QDomNode node, QStringList parentNames, const std::function<bool (const QDomNode&, QStringList)>& perNodeAction)
{
    bool continueTraverse = node.isElement() && perNodeAction(node, parentNames);
    if (continueTraverse) {
        QDomElement elem = node.toElement();
        QStringList childParentNames = parentNames << elem.tagName();
        for(QDomNode childNode = elem.firstChild(); continueTraverse && !childNode.isNull(); childNode = childNode.nextSibling()) {
            continueTraverse = traverseElements(childNode, childParentNames, perNodeAction);
        }
    }
    return continueTraverse;
}
