#include "xmlcomparer.h"
#include "xmlattribcompare.h"

XmlComparer::XmlComparer(XmlElemCompareFuncList elemCompareFunctions) :
    m_elemCompareFunctions(elemCompareFunctions)
{
}

XmlElemCompareFuncList XmlComparer::m_standardElemCompareFunctions =
        XmlElemCompareFuncList() <<
[](const QDomElement& elem1, const QDomElement& elem2) -> bool
{
    return XmlAttribCompare::compare(elem1, elem2);
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
            for(const auto &elemCompareFunc : qAsConst(m_elemCompareFunctions)) {
                if(!elemCompareFunc(elem1, elem2))
                    return false;
            }
        }
    }
    return true;
}

bool XmlComparer::isXmlEmptyOrInvalid(XmlElemInfo xmlInfo)
{
    return xmlInfo.begin() == xmlInfo.end();
}