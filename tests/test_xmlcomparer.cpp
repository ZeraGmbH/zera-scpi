#include "test_xmlcomparer.h"
#include "xmlcomparer.h"
#include "xmlattribcompare.h"
#include <QTest>

QList<XmlElemCompareFunction> elemAttribCompareList =
        QList<XmlElemCompareFunction>() << [](const QDomElement& elem1, const QDomElement& elem2) -> bool
{
    return XmlAttribCompare::compare(elem1, elem2);
};

void test_xmlcomparer::bothEmpty()
{
    XmlComparer comparer(elemAttribCompareList);
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
    XmlComparer comparer(elemAttribCompareList);
    QVERIFY(comparer.compareXml(xmlFindUpUpUp, xmlFindUpUpUp));
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
    XmlComparer comparer(elemAttribCompareList);
    QVERIFY(!comparer.compareXml(xml1, xml2));
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
    XmlComparer comparer(elemAttribCompareList);
    QVERIFY(comparer.compareXml(xml1, xml2));
}

void test_xmlcomparer::firstEmptySecondValid()
{
    QString xml2 =
        "<NODE1>"
            "<NODE3/>"
            "<NODE2/>"
        "</NODE1>";
    XmlComparer comparer(elemAttribCompareList);
    QVERIFY(!comparer.compareXml(QString(), xml2));
}

void test_xmlcomparer::firstValidSecondEmpty()
{
    QString xml1 =
        "<NODE1>"
            "<NODE3/>"
            "<NODE2/>"
        "</NODE1>";
    XmlComparer comparer(elemAttribCompareList);
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
    XmlComparer comparer(elemAttribCompareList);
    QVERIFY(!comparer.compareXml(xml1, xml2));
}

void test_xmlcomparer::attibsEqual()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2 a='1' b='2'/>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    XmlComparer comparer(elemAttribCompareList);
    QVERIFY(comparer.compareXml(xml1, xml1));
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
    XmlComparer comparer(elemAttribCompareList);
    QVERIFY(comparer.compareXml(xml1, xml2));
}

QTEST_MAIN(test_xmlcomparer)
