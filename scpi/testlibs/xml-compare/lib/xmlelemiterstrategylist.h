#ifndef XMLELEMITERSTRATEGYLIST_H
#define XMLELEMITERSTRATEGYLIST_H

#include "xmlelemiter.h"

class XmlElemIterStrategyList : public XmlElemIterStrategy
{
public:
    QDomElement next(QDomElement current) override;
};

#endif // XMLELEMITERSTRATEGYLIST_H
