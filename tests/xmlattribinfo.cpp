#include "xmlattribinfo.h"

bool XmlAttribInfo::compareElemAttribs(const QDomElement &elem, QMap<QString, QString> attribKeyValues)
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

bool XmlAttribInfo::attribsEmpty(QMap<QString, QString> attribKeyValues)
{
    return attribKeyValues.isEmpty();
}

bool XmlAttribInfo::elemEmpty(const QDomElement &elem)
{
    return !elem.hasAttributes();
}

bool XmlAttribInfo::bothEmpty(const QDomElement &elem, QMap<QString, QString> attribKeyValues)
{
    return attribsEmpty(attribKeyValues) && elemEmpty(elem);
}

bool XmlAttribInfo::compareAll(const QDomElement &elem, QMap<QString, QString> attribKeyValues)
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
