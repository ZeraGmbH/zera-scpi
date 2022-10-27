#ifndef XMLATTRIBINFO_H
#define XMLATTRIBINFO_H

#include <QDomElement>
#include <QMap>
#include <QString>

class XmlAttribInfo
{
public:
    static bool compareElemAttribs(const QDomElement &elem, QMap<QString, QString> attribKeyValues);
private:
    static bool attribsEmpty(QMap<QString, QString> attribKeyValues);
    static bool elemEmpty(const QDomElement &elem);
    static bool bothEmpty(const QDomElement &elem, QMap<QString, QString> attribKeyValues);
    static bool compareAll(const QDomElement &elem, QMap<QString, QString> attribKeyValues);
};

#endif // XMLATTRIBINFO_H
