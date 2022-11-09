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

typedef std::unique_ptr<XmlElemIterStrategy> XmlElemIterStrategyPtr;

class XmlElemIter
{
public:
    XmlElemIter(XmlElemIterStrategyPtr &&iterStrategy, QDomElement elem);
    QDomElement getElem();
    QStringList getParentPath();
    bool isEnd();
    void next();
    bool operator== (const XmlElemIter &other) const;
    bool operator!= (const XmlElemIter &other) const;
private:
    XmlElemIterStrategyPtr m_iterStrategy;
    QDomElement m_elem;
};

#endif // XMLELEMITER_H
