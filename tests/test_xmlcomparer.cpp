#include "test_xmlcomparer.h"
#include "xmlcomparer.h"
#include <QTest>

void test_xmlcomparer::bothEmpty()
{
    XmlComparer comparer;
    QVERIFY(comparer.compareXml("", ""));
}

void test_xmlcomparer::bothSameNoAttribs()
{
    QString xmlFindUpUpUp =
        "<root>"
            "<child1>"
                "<grandchild1>"
                    "<ggrandchild1>"
                        "<gggrandchild1/>"
                    "</ggrandchild1>"
                "</grandchild1>"
            "</child1>"
            "<child2/>"
        "</root>";
    XmlComparer comparer;
    QVERIFY(comparer.compareXml(xmlFindUpUpUp, xmlFindUpUpUp, true));
}

void test_xmlcomparer::bothDifferentNoAttribs()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2/>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2/>"
            "<NODE3/>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(!comparer.compareXml(xml1, xml2, true));
}

void test_xmlcomparer::bothSameNoAttribsDiffSequence()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2/>"
            "<NODE3/>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE3/>"
            "<NODE2/>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(comparer.compareXml(xml1, xml2, true));
}

void test_xmlcomparer::firstEmptySecondValid()
{
    QString xml2 =
        "<NODE1>"
            "<NODE3/>"
            "<NODE2/>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(!comparer.compareXml(QString(), xml2));
}

void test_xmlcomparer::firstValidSecondEmpty()
{
    QString xml1 =
        "<NODE1>"
            "<NODE3/>"
            "<NODE2/>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(!comparer.compareXml(xml1, QString()));
}

void test_xmlcomparer::attibsUnequal()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2 a='1' b='2'/>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2 a='1' b='2'/>"
            "<NODE3 c='3' d='foo' e='5'></NODE3>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(!comparer.compareXml(xml1, xml2, true));
}

void test_xmlcomparer::attibsEqual()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2 a='1' b='2'/>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(comparer.compareXml(xml1, xml1, true));
}

void test_xmlcomparer::attribsDiffSequence()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2 a='1' b='2'/>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
            "<NODE2 a='1' b='2'/>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(comparer.compareXml(xml1, xml2, true));
}

void test_xmlcomparer::textUnequal()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2>bar</NODE2>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(!comparer.compareXml(xml1, xml2, true));
}

void test_xmlcomparer::textEqual()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(comparer.compareXml(xml1, xml1, true));
}

void test_xmlcomparer::attibsUnequalTextEqual()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
            "<NODE3 c='3' d='foo' e='5'></NODE3>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(!comparer.compareXml(xml1, xml2, true));
}

void test_xmlcomparer::attibsEqualTextUnequal()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2>bar</NODE2>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(!comparer.compareXml(xml1, xml2, true));
}

void test_xmlcomparer::attibsEqualTextEqual()
{
    QString xml =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(comparer.compareXml(xml, xml, true));
}


void test_xmlcomparer::docTypeUnequal()
{
    QString xml1 =
        "<!DOCTYPE foo>"
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    QString xml2 =
        "<!DOCTYPE bar>"
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(!comparer.compareXml(xml1, xml2, true));
}

void test_xmlcomparer::docTypeEqual()
{
    QString xml1 =
        "<!DOCTYPE foo>"
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    QString xml2 =
        "<!DOCTYPE foo>"
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    XmlComparer comparer;
    QVERIFY(comparer.compareXml(xml1, xml2, true));
}

QTEST_MAIN(test_xmlcomparer)
