#include "test_xmlelemiter.h"
#include "xmlelemiter.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiter)

class XmlElemIterStrategyTest : public XmlElemIterStrategy
{
public:
    virtual QDomElement next(QDomElement current) { return QDomElement(); }
};

void test_xmlelemiter::noDocNoParent()
{
    QDomElement elem;
    XmlElemIter iter(std::make_unique<XmlElemIterStrategyTest>(), elem);
    QStringList parentPath = iter.getParentPath();
    QVERIFY(parentPath.empty());
}

void test_xmlelemiter::noParentNotInDoc()
{
    QDomDocument doc;
    QDomElement freeNode = doc.createElement("root");
    XmlElemIter iter(std::make_unique<XmlElemIterStrategyTest>(), freeNode);
    QCOMPARE(iter.getParentPath(), QStringList());
}

void test_xmlelemiter::root()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    XmlElemIter iter(std::make_unique<XmlElemIterStrategyTest>(), root);
    QCOMPARE(iter.getParentPath(), QStringList());
}

void test_xmlelemiter::childOfRoot()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QDomElement child = doc.createElement("child");
    root.appendChild(child);
    XmlElemIter iter(std::make_unique<XmlElemIterStrategyTest>(), child);
    QCOMPARE(iter.getParentPath(), QStringList("root"));
}

void test_xmlelemiter::grandChildOfRoot()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QDomElement child = doc.createElement("child");
    root.appendChild(child);
    QDomElement grandChild = doc.createElement("grandChild");
    child.appendChild(grandChild);

    XmlElemIter iter(std::make_unique<XmlElemIterStrategyTest>(), grandChild);
    QCOMPARE(iter.getParentPath(), QStringList() << "root" << "child");
}
