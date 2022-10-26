#ifndef XMLCOMPARER_H
#define XMLCOMPARER_H

#include <QString>
#include <QDomDocument>
#include <functional>

class XmlComparer
{
public:
    bool loadXml(const QString &xml);
    int getNodeCount();
private:
    void traverseElements(QDomNode node, const std::function<void (const QDomNode&)>& perNodeAction);
    QDomDocument m_doc;


};

#endif // XMLCOMPARER_H
