#ifndef XMLELEMITER_H
#define XMLELEMITER_H

#include <QDomElement>
#include <QStringList>
#include <memory>

class XmlElemIterStrategy
{
public:
    virtual QDomElement next(QDomElement current) = 0;
    virtual ~XmlElemIterStrategy() = default;
};

class XmlElemIter
{
public:
    XmlElemIter(std::unique_ptr<XmlElemIterStrategy> &&iterStrategy, QDomElement elem);
    QDomElement getElem();
    QStringList getParentPath();
    bool isEnd();
    void next();
    bool operator== (const XmlElemIter &other) const;
    bool operator!= (const XmlElemIter &other) const;
private:
    std::unique_ptr<XmlElemIterStrategy> m_iterStrategy;
    QDomElement m_elem;
};

#endif // XMLELEMITER_H
