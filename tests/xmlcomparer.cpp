#include "xmlcomparer.h"
#include "xmlelemcompareattribs.h"
#include "xmlelemcomparetext.h"

XmlComparer::XmlComparer(XmlElemCompareFunc elemCompareFunction) :
    m_elemCompareFunc(elemCompareFunction)
{
}

XmlElemCompareFunc XmlComparer::m_stdElemCompareFunc =
[](const QDomElement& elem1, const QDomElement& elem2) -> bool
{
    return
        XmlElemCompareAttribs::compare(elem1, elem2) &&
        XmlElemCompareText::compare(elem1, elem2);
};

bool XmlComparer::compareXml(QString xml1, QString xml2, bool fatalOnInvalidXml)
{
    XmlDocument elemInfo1;
    elemInfo1.loadXml(xml1, fatalOnInvalidXml);
    XmlDocument elemInfo2;
    elemInfo2.loadXml(xml2, fatalOnInvalidXml);

    if(isXmlEmptyOrInvalid(elemInfo1) && isXmlEmptyOrInvalid(elemInfo2))
        return true;
    if(elemInfo1.getElemCount() != elemInfo2.getElemCount())
        return false;
    if(!compareDocTypes(elemInfo1, elemInfo2)) {
        return false;
    }

    for(auto iter=elemInfo1.begin(); iter!=elemInfo1.end(); ++iter) {
        QDomElement elem1 = iter.getElem();
        QStringList tagpath1 = iter.getParentPath() + QStringList(elem1.tagName());
        QDomElement elem2;
        if(elemInfo2.findElem(tagpath1, elem2)) {
            if(!m_elemCompareFunc(elem1, elem2))
                return false;
        }
    }
    return true;
}

bool XmlComparer::isXmlEmptyOrInvalid(XmlDocument elemInfo)
{
    return elemInfo.begin() == elemInfo.end();
}

bool XmlComparer::compareDocTypes(XmlDocument elemInfo1, XmlDocument elemInfo2)
{
    return elemInfo1.getDocType().name() == elemInfo2.getDocType().name();
}
