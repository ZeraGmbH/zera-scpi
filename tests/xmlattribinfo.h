#ifndef XMLATTRIBINFO_H
#define XMLATTRIBINFO_H

#include <QDomElement>
#include <QMap>
#include <QString>

class XmlAttribInfo
{
public:
    static bool compareElemAttribs(const QDomElement &elem, QMap<QString, QString> attribKeyValues);
};

#endif // XMLATTRIBINFO_H
