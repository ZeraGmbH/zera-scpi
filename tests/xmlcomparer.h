#ifndef XMLCOMPARER_H
#define XMLCOMPARER_H

#include "xmleleminfo.h"
#include <QString>
#include <QDomElement>
#include <QList>
#include <functional>

typedef std::function<bool (const QDomElement, const QDomElement)> XmlElemCompareFunc;

class XmlComparer
{
public:
    XmlComparer(XmlElemCompareFunc elemCompareFunction = m_stdElemCompareFunc);
    bool compareXml(QString xml1, QString xml2, bool fatalOnInvalidXml=false);
private:
    bool isXmlEmptyOrInvalid(XmlElemInfo xmlInfo);
    XmlElemCompareFunc m_elemCompareFunc;
    static XmlElemCompareFunc m_stdElemCompareFunc;
};

#endif // XMLCOMPARER_H
