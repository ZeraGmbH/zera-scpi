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
    QVERIFY(iter.getCurrElem().isNull());
}

QString xmlRootOnly =
    "<root/>";

void test_xmlelemiteratorlist::findRoot()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIteratorList iter = doc.find(QStringList("root"));
    QCOMPARE(iter.getParentPath(), QStringList());
    QDomElement rootElem = iter.getCurrElem();
    QCOMPARE(rootElem.tagName(), "root");
}

void test_xmlelemiteratorlist::findRootHasOne()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIteratorList iter = doc.find(QStringList("root"));
    QVERIFY(iter.getNextElem().isNull());
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
    QDomElement childElem = iter.getCurrElem();
    QVERIFY(!childElem.isNull());
    QCOMPARE(childElem.tagName(), "child1");
    QVERIFY(iter.getNextElem().isNull());
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
    QDomElement childElem = iter.getCurrElem();
    QVERIFY(!childElem.isNull());
    QCOMPARE(childElem.tagName(), "child1");
    QDomElement nextChildElem = iter.getNextElem();
    QVERIFY(!nextChildElem.isNull());
    QVERIFY(childElem != nextChildElem);
}

void test_xmlelemiteratorlist::nextForNullElem()
{
    QDomElement elem;
    XmlElemIteratorList iter(elem);
    QVERIFY(iter.getNextElem().isNull());
}
