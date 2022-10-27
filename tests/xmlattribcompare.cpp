#include "xmlattribcompare.h"

bool XmlAttribCompare::compare(const QDomElement &elem, QMap<QString, QString> attribKeyValues)
{
    if(bothEmpty(elem, attribKeyValues))
        return true;
    if(attribsEmpty(attribKeyValues) || elemEmpty(elem))
        return false;
    if(elem.attributes().count() != attribKeyValues.count()) {
        return false;
    }
    return compareAll(elem, attribKeyValues);
}

bool XmlAttribCompare::attribsEmpty(QMap<QString, QString> attribKeyValues)
{
    return attribKeyValues.isEmpty();
}

bool XmlAttribCompare::elemEmpty(const QDomElement &elem)
{
    return !elem.hasAttributes();
}

bool XmlAttribCompare::bothEmpty(const QDomElement &elem, QMap<QString, QString> attribKeyValues)
{
    return attribsEmpty(attribKeyValues) && elemEmpty(elem);
}

bool XmlAttribCompare::compareAll(const QDomElement &elem, QMap<QString, QString> attribKeyValues)
{
    auto elemAttribs = elem.attributes();
    for(auto iter=attribKeyValues.cbegin(); iter!=attribKeyValues.cend(); ++iter) {
        QString attrKey = iter.key();
        if(!elemAttribs.contains(attrKey))
            return false;
        if(elem.attribute(attrKey) != iter.value()) {
            return false;
        }
    }
    return true;
}
