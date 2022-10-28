#include "xmlcomparer.h"
#include "xmlattribcompare.h"
#include "xmltextcompare.h"

XmlComparer::XmlComparer(XmlElemCompareFunc elemCompareFunction) :
    m_elemCompareFunc(elemCompareFunction)
{
}

XmlElemCompareFunc XmlComparer::m_stdElemCompareFunc =
[](const QDomElement& elem1, const QDomElement& elem2) -> bool
{
    return
        XmlAttribCompare::compare(elem1, elem2) &&
        XmlTextCompare::compare(elem1, elem2);
};

bool XmlComparer::compareXml(QString xml1, QString xml2)
{
    XmlElemInfo elemInfo1;
    elemInfo1.loadXml(xml1);
    XmlElemInfo elemInfo2;
    elemInfo2.loadXml(xml2);

    if(isXmlEmptyOrInvalid(elemInfo1) && isXmlEmptyOrInvalid(elemInfo2))
        return true;
    if(elemInfo1.getElemCount() != elemInfo2.getElemCount())
        return false;

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

bool XmlComparer::isXmlEmptyOrInvalid(XmlElemInfo xmlInfo)
{
    return xmlInfo.begin() == xmlInfo.end();
}
