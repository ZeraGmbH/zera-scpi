#include "test_xmlelemiteratorsort.h"
#include "xmldocument.h"
#include "xmlelemiterstrategysort.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiteratorsort)

void test_xmlelemiteratorsort::noArrayOneNested()
{
    QString xml =
        "<root>"
            "<child1>"
                "<cchild1/>"
            "</child1>"
        "</root>";
    XmlDocument doc;
    doc.loadXml(xml, true);
    XmlElemIter iter = doc.root(std::make_unique<XmlElemIterStrategySort>());
    QCOMPARE(iter.getElem().tagName(), "root");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "child1");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "cchild1");
    iter.next(); QVERIFY(iter.getElem().isNull());
}

void test_xmlelemiteratorsort::noArraySortTwo()
{
    QString xml =
        "<root>"
            "<child2/>"
            "<child1/>"
        "</root>";
    XmlDocument doc;
    doc.loadXml(xml, true);
    XmlElemIter iter = doc.root(std::make_unique<XmlElemIterStrategySort>());
    QCOMPARE(iter.getElem().tagName(), "root");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "child1");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "child2");
    iter.next(); QVERIFY(iter.getElem().isNull());
}

void test_xmlelemiteratorsort::noArraySortTwoNested()
{
    QString xml =
        "<root>"
            "<child2>"
                "<cchild2/>"
            "</child2>"
            "<child1>"
                "<cchild1/>"
            "</child1>"
        "</root>";
    XmlDocument doc;
    doc.loadXml(xml, true);
    XmlElemIter iter = doc.root(std::make_unique<XmlElemIterStrategySort>());
    QCOMPARE(iter.getElem().tagName(), "root");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "child1");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "child2");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "cchild1");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "cchild2");
    iter.next(); QVERIFY(iter.getElem().isNull());
}

void test_xmlelemiteratorsort::noArraySortTwoNestedTwo()
{
    QString xml =
        "<root>"
            "<child2>"
                "<cchild4/>"
                "<cchild3/>"
            "</child2>"
            "<child1>"
                "<cchild1/>"
                "<cchild2/>"
            "</child1>"
        "</root>";
    XmlDocument doc;
    doc.loadXml(xml, true);
    XmlElemIter iter = doc.root(std::make_unique<XmlElemIterStrategySort>());
    QCOMPARE(iter.getElem().tagName(), "root");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "child1");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "child2");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "cchild1");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "cchild2");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "cchild3");
    iter.next(); QCOMPARE(iter.getElem().tagName(), "cchild4");
    iter.next(); QVERIFY(iter.getElem().isNull());
}
