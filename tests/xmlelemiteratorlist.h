#ifndef XMLELEMITERATORLIST_H
#define XMLELEMITERATORLIST_H

#include "xmlelemiteratortemplate.h"

class XmlElemIteratorList : public XmlElemIteratorTemplate
{
public:
    XmlElemIteratorList(QDomElement elem);
protected:
    void next() override;
};

#endif // XMLELEMITERATORLIST_H
