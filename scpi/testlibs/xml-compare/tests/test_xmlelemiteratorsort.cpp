#include "test_xmlelemiteratorsort.h"
#include "xmldocument.h"
#include "xmlelemiterstrategysort.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiteratorsort)

void test_xmlelemiteratorsort::noFreakOutOnEmpty()
{
    XmlDocument doc;
    XmlElemIter iter = doc.root(std::make_unique<XmlElemIterStrategySort>());
    iter.next(); QVERIFY(iter.getElem().isNull());
}

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

void test_xmlelemiteratorsort::arraySortTwoNestedTwo()
{
    QString xml =
        "<NODE1 i='1'>"
            "<NODE2 i='2'>"
                "<NODE3 i='4'></NODE3>"
                "<NODE3 i='5'></NODE3>"
            "</NODE2>"
            "<NODE2 i='3'>"
                "<NODE3 i='6'></NODE3>"
                "<NODE3 i='7'></NODE3>"
            "</NODE2>"
        "</NODE1>";
    XmlDocument doc;
    doc.loadXml(xml, true);
    XmlElemIter iter = doc.root(std::make_unique<XmlElemIterStrategySort>());
    QCOMPARE(iter.getElem().attribute("i"), "1");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "2");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "3");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "4");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "5");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "6");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "7");
    iter.next(); QVERIFY(iter.getElem().isNull());
}

void test_xmlelemiteratorsort::arraySortTwoNestedTwoMixNonArray()
{
    QString xml =
        "<root i='1'>"
            "<NODE1 i='2'></NODE1>"
            "<NODE2 i='3'>"
                "<NODE3 i='5'></NODE3>"
                "<NODE3 i='6'></NODE3>"
            "</NODE2>"
            "<NODE2 i='4'>"
                "<NODE1 i='7'></NODE1>"
                "<NODE3 i='8'></NODE3>"
                "<NODE3 i='9'></NODE3>"
            "</NODE2>"
        "</root>";
    XmlDocument doc;
    doc.loadXml(xml, true);
    XmlElemIter iter = doc.root(std::make_unique<XmlElemIterStrategySort>());
    QCOMPARE(iter.getElem().attribute("i"), "1");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "2");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "3");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "4");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "5");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "6");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "7");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "8");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "9");
    iter.next(); QVERIFY(iter.getElem().isNull());
}

void test_xmlelemiteratorsort::arraySortTwoNestedTwoMixNonArraySort()
{
    QString xml =
        "<root i='1'>"
            "<NODE2 i='3'>"
                "<NODE3 i='5'></NODE3>"
                "<NODE3 i='6'></NODE3>"
            "</NODE2>"
            "<NODE2 i='4'>"
                "<NODE3 i='8'></NODE3>"
                "<NODE3 i='9'></NODE3>"
                "<NODE1 i='7'></NODE1>"
            "</NODE2>"
            "<NODE1 i='2'></NODE1>"
        "</root>";
    XmlDocument doc;
    doc.loadXml(xml, true);
    XmlElemIter iter = doc.root(std::make_unique<XmlElemIterStrategySort>());
    QCOMPARE(iter.getElem().attribute("i"), "1");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "2");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "3");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "4");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "5");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "6");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "7");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "8");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "9");
    iter.next(); QVERIFY(iter.getElem().isNull());
}

void test_xmlelemiteratorsort::arraySortTwoNestedTwoMixNonArrayObscure()
{
    QString xml =
        "<root i='1'>"
            "<NODE2 i='3'>"
                "<NODE3 i='5'></NODE3>"
                "<NODE3 i='6'></NODE3>"
            "</NODE2>"
            "<NODE1 i='2'></NODE1>"
            "<NODE2 i='4'>"
                "<NODE3 i='8'></NODE3>"
                "<NODE1 i='7'></NODE1>"
                "<NODE3 i='9'></NODE3>"
            "</NODE2>"
        "</root>";
    XmlDocument doc;
    doc.loadXml(xml, true);
    XmlElemIter iter = doc.root(std::make_unique<XmlElemIterStrategySort>());
    QCOMPARE(iter.getElem().attribute("i"), "1");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "2");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "3");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "4");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "5");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "6");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "7");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "8");
    iter.next(); QCOMPARE(iter.getElem().attribute("i"), "9");
    iter.next(); QVERIFY(iter.getElem().isNull());
}
