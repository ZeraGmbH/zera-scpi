#ifndef XMLCOMPARER_H
#define XMLCOMPARER_H

#include <QStringList>
#include <QDomDocument>
#include <QDomNode>
#include <functional>

class XmlElemInfo
{
public:
    bool loadXml(const QString &xml);
    int getElemCount();
    bool findElem(QStringList nodeSearchPath, QDomElement &foundElem);
private:
    bool traverseElements(QDomNode node, const QStringList &parentPath, const std::function<bool (const QDomElement &, QStringList)> &perNodeAction);
    QDomDocument m_doc;
};

#endif // XMLCOMPARER_H
