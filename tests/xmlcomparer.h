#ifndef XMLCOMPARER_H
#define XMLCOMPARER_H

#include <QStringList>
#include <QDomDocument>
#include <QDomNode>
#include <functional>

class XmlComparer
{
public:
    bool loadXml(const QString &xml);
    int getNodeCount();
    bool findNode(QStringList nodeSearchPath, QDomNode &foundNode);
private:
    bool traverseElements(QDomNode node, QStringList parentNames, const std::function<bool (const QDomNode &, QStringList)> &perNodeAction);
    QDomDocument m_doc;


};

#endif // XMLCOMPARER_H
