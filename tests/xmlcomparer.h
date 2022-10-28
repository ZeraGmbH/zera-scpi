#ifndef XMLCOMPARER_H
#define XMLCOMPARER_H

#include "xmleleminfo.h"
#include <QString>
#include <QDomElement>
#include <QList>
#include <functional>

typedef std::function<bool (const QDomElement, const QDomElement)> XmlElemCompareFunction;

class XmlComparer
{
public:
    XmlComparer(QList<XmlElemCompareFunction> elemCompareFunctions);
    bool compareXml(QString xml1, QString xml2);
private:
    bool isXmlEmptyOrInvalid(XmlElemInfo xmlInfo);
    QList<XmlElemCompareFunction> m_elemCompareFunctions;
};

#endif // XMLCOMPARER_H
