#ifndef XMLCOMPARER_H
#define XMLCOMPARER_H

#include "xmleleminfo.h"
#include <QString>
#include <QDomElement>
#include <QList>
#include <functional>

typedef std::function<bool (const QDomElement, const QDomElement)> XmlElemCompareFunc;
typedef QList<XmlElemCompareFunc> XmlElemCompareFuncList;

class XmlComparer
{
public:
    XmlComparer(XmlElemCompareFuncList elemCompareFunctions = m_standardElemCompareFunctions);
    bool compareXml(QString xml1, QString xml2);
private:
    bool isXmlEmptyOrInvalid(XmlElemInfo xmlInfo);
    XmlElemCompareFuncList m_elemCompareFunctions;
    static XmlElemCompareFuncList m_standardElemCompareFunctions;
};

#endif // XMLCOMPARER_H
