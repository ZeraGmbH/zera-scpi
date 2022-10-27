#include "xmlattribinfo.h"

bool XmlAttribInfo::compareElemAttribs(const QDomElement &elem, QMap<QString, QString> attribKeyValues)
{
    bool found = false;
    if(!attribKeyValues.isEmpty()) {
        QString firstKeyExpected = attribKeyValues.firstKey();
        QString firstValExpected = attribKeyValues[firstKeyExpected];
        QString firstValueFound = elem.attribute(firstKeyExpected);
        found = firstValExpected == firstValueFound;
    }
    else if(!elem.hasAttributes()) {
        found = true;
    }
    return found;
}
