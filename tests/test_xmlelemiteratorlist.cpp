#include "test_xmlelemiteratorlist.h"
#include "xmlqtdomdociteration.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiteratorlist)

void test_xmlelemiteratorlist::findEmptyDoc()
{
    XmlQtDomDocIteration doc;
    XmlElemIter iter = doc.find(QStringList("foo"));
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getElem().isNull());
}

QString xmlRootOnly =
    "<root/>";

void test_xmlelemiteratorlist::findRoot()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIter iter = doc.find(QStringList("root"));
    QCOMPARE(iter.getParentPath(), QStringList());
    QDomElement rootElem = iter.getElem();
    QCOMPARE(rootElem.tagName(), "root");
}

void test_xmlelemiteratorlist::findRootHasOne()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIter iter = doc.find(QStringList("root"));
    iter.next();
    QVERIFY(iter.getElem().isNull());
}

QString xmlTwoDiffChildren =
    "<root>"
        "<child1/>"
        "<child2/>"
    "</root>";

void test_xmlelemiteratorlist::findOneOnTwoDiffChildren()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlTwoDiffChildren, true));
    XmlElemIter iter = doc.find(QStringList() << "root" << "child1");
    QDomElement childElem = iter.getElem();
    QVERIFY(!childElem.isNull());
    QCOMPARE(childElem.tagName(), "child1");
    iter.next();
    QVERIFY(iter.getElem().isNull());
}

QString xmlTwoEqualChildren =
    "<root>"
        "<child1/>"
        "<child1/>"
    "</root>";


void test_xmlelemiteratorlist::findOneOnTwoEqualChildren()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlTwoEqualChildren, true));
    XmlElemIter iter = doc.find(QStringList() << "root" << "child1");
    QDomElement childElem = iter.getElem();
    QVERIFY(!childElem.isNull());
    QCOMPARE(childElem.tagName(), "child1");
    iter.next();
    QDomElement nextChildElem = iter.getElem();
    QVERIFY(!nextChildElem.isNull());
    QVERIFY(childElem != nextChildElem);
}

