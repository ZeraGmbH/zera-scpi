#ifndef XMLATTRIBCOMPARE_H
#define XMLATTRIBCOMPARE_H

#include <QDomElement>
#include <QMap>
#include <QString>

class XmlAttribCompare
{
public:
    static bool compare(const QDomElement &elem, QMap<QString, QString> attribKeyValues);
private:
    static bool attribsEmpty(QMap<QString, QString> attribKeyValues);
    static bool elemEmpty(const QDomElement &elem);
    static bool bothEmpty(const QDomElement &elem, QMap<QString, QString> attribKeyValues);
    static bool compareAll(const QDomElement &elem, QMap<QString, QString> attribKeyValues);
};

#endif // XMLATTRIBCOMPARE_H
