#ifndef XMLATTRIBCOMPARE_H
#define XMLATTRIBCOMPARE_H

#include <QDomElement>
#include <QMap>
#include <QString>

class XmlAttribCompare
{
public:
    static bool compare(const QDomElement &elem1, const QDomElement &elem2);
private:
    static bool elemEmpty(const QDomElement &elem);
    static bool bothEmpty(const QDomElement &elem1, const QDomElement &elem2);
    static bool compareAll(const QDomElement &elem1, const QDomElement &elem2);
};

#endif // XMLATTRIBCOMPARE_H
