#ifndef XMLELEMITERATORTEMPLATE_H
#define XMLELEMITERATORTEMPLATE_H

#include <QDomElement>
#include <QStringList>
#include <memory>

class XmlElemIteratorTemplate
{
public:
    XmlElemIteratorTemplate(QDomElement elem);
    QDomElement getElem();
    QStringList getParentPath();
    virtual void next() = 0;

    bool operator== (const XmlElemIteratorTemplate &other) const;
    bool operator!= (const XmlElemIteratorTemplate &other) const;
protected:
    QDomElement m_elem;
};

typedef std::shared_ptr<XmlElemIteratorTemplate> XmlElemIterator;

#endif // XMLELEMITERATORTEMPLATE_H
