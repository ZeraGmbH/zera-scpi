#include "test_xmlelemiteratorlist.h"
#include "xmlelemiteratorlist.h"
#include "xmldocument.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiteratorlist)

void test_xmlelemiteratorlist::findEmptyDoc()
{
    XmlDocument doc;
    XmlElemIteratorList iter = doc.find(QStringList("foo"));
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getElem().isNull());
}

QString xmlRootOnly =
    "<root/>";

void test_xmlelemiteratorlist::findRoot()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIteratorList iter = doc.find(QStringList("root"));
    QCOMPARE(iter.getParentPath(), QStringList());
    QDomElement rootElem = iter.getElem();
    QCOMPARE(rootElem.tagName(), "root");
}

void test_xmlelemiteratorlist::findRootHasOne()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIteratorList iter = doc.find(QStringList("root"));
    ++iter;
    QVERIFY(iter.getElem().isNull());
}

QString xmlTwoDiffChildren =
    "<root>"
        "<child1/>"
        "<child2/>"
    "</root>";

void test_xmlelemiteratorlist::findOneOnTwoDiffChildren()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoDiffChildren, true));
    XmlElemIteratorList iter = doc.find(QStringList() << "root" << "child1");
    QDomElement childElem = iter.getElem();
    QVERIFY(!childElem.isNull());
    QCOMPARE(childElem.tagName(), "child1");
    ++iter;
    QVERIFY(iter.getElem().isNull());
}

QString xmlTwoEqualChildren =
    "<root>"
        "<child1/>"
        "<child1/>"
    "</root>";


void test_xmlelemiteratorlist::findOneOnTwoEqualChildren()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoEqualChildren, true));
    XmlElemIteratorList iter = doc.find(QStringList() << "root" << "child1");
    QDomElement childElem = iter.getElem();
    QVERIFY(!childElem.isNull());
    QCOMPARE(childElem.tagName(), "child1");
    ++iter;
    QDomElement nextChildElem = iter.getElem();
    QVERIFY(!nextChildElem.isNull());
    QVERIFY(childElem != nextChildElem);
}

void test_xmlelemiteratorlist::nextForNullElem()
{
    QDomElement elem;
    XmlElemIteratorList iter(elem);
    ++iter;
    QVERIFY(iter.getElem().isNull());
}
