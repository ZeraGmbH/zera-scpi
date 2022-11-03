#ifndef XMLELEMITERATORLIST_H
#define XMLELEMITERATORLIST_H

#include "xmlelemiteratortemplate.h"

class XmlElemIteratorList : public XmlElemIteratorTemplate
{
public:
    XmlElemIteratorList(QDomElement elem);
protected:
    QDomElement getNextElem() override;
};

#endif // XMLELEMITERATORLIST_H
