#ifndef XMLELEMITERATORTEMPLATE_H
#define XMLELEMITERATORTEMPLATE_H

#include <QDomElement>
#include <QStringList>

class XmlElemIteratorTemplate
{
public:
    XmlElemIteratorTemplate(QDomElement elem);
    QDomElement getElem();
    QStringList getParentPath();

    XmlElemIteratorTemplate &operator++();
    bool operator== (const XmlElemIteratorTemplate &other) const;
    bool operator!= (const XmlElemIteratorTemplate &other) const;
protected:
    virtual QDomElement getNextElem() = 0;
    QDomElement m_elem;
};

#endif // XMLELEMITERATORTEMPLATE_H
