#include "xmlcomparer.h"
#include <QDomNode>

bool XmlComparer::loadXml(const QString &xml)
{
    return m_doc.setContent(xml);
}

int XmlComparer::getNodeCount()
{
    QDomNode firstNode = m_doc.firstChild();
    int nodeNums = 0;
    traverseElements(firstNode, [&](const QDomNode& node)
    {
        nodeNums++;
    });
    return nodeNums;
}

void XmlComparer::traverseElements(QDomNode node, const std::function<void (const QDomNode&)>& perNodeAction)
{
    if (node.isElement()) {
        perNodeAction(node);
        QDomElement elem = node.toElement();
        for(QDomNode childNode = elem.firstChild(); !childNode.isNull(); childNode = childNode.nextSibling()) {
            traverseElements(childNode.toElement(), perNodeAction);
        }
    }
}
