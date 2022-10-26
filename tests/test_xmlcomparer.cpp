#include "test_xmlcomparer.h"
#include "xmlcomparer.h"
#include <QTest>

QTEST_MAIN(test_xmlcomparer)

QString xmlOneNode =
    "<!DOCTYPE SCPIModel>"
        "<NODE1/>";

QString xmlOneChildNode =
    "<!DOCTYPE SCPIModel>"
    "<NODE1>"
        "<NODE2/>"
    "</NODE1>";

QString xmlTwoChildNodes =
    "<!DOCTYPE SCPIModel>"
    "<NODE1>"
        "<NODE2/>"
        "<NODE3/>"
    "</NODE1>";

// see https://www.w3schools.com/xml/xml_attributes.asp
QString xmlTwoChildNodesWithAttributes =
    "<!DOCTYPE SCPIModel>"
    "<NODE1>"
        "<NODE2 a='1' b='2'></NODE2>"
        "<NODE3 c='3' d='4' e='5'></NODE3>"
    "</NODE1>";

QString xmlTwoChildNodesNested =
    "<!DOCTYPE SCPIModel>"
    "<NODE1>"
        "<NODE2>"
            "<NODE3/>"
        "</NODE2>"
    "</NODE1>";

void test_xmlcomparer::loadInvalid()
{
    XmlComparer cmp;
    QVERIFY(!cmp.loadXml("foo"));
}

void test_xmlcomparer::loadValid()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
}

void test_xmlcomparer::countRootNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QCOMPARE(cmp.getNodeCount(), 1);
}

void test_xmlcomparer::countOneChildNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneChildNode));
    QCOMPARE(cmp.getNodeCount(), 2);
}

void test_xmlcomparer::countTwoChildNodes()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodes));
    QCOMPARE(cmp.getNodeCount(), 3);

}

void test_xmlcomparer::countTwoChildNodesNested()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesNested));
    QCOMPARE(cmp.getNodeCount(), 3);
}

void test_xmlcomparer::countTwoChildNodesAttributes()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesWithAttributes));
    QCOMPARE(cmp.getNodeCount(), 3);
}

void test_xmlcomparer::findEmptySearch()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QDomElement elem;
    QVERIFY(!cmp.findElem(QStringList(), elem));
}

void test_xmlcomparer::findRootNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
}

void test_xmlcomparer::noFindRootNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QDomElement elem;
    QVERIFY(!cmp.findElem(QStringList() << "foo", elem));
}

void test_xmlcomparer::findOneChildNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneChildNode));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "foo", elem));
}

void test_xmlcomparer::findTwoChildNodes()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodes));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE3", elem));
    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE3", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "NODE2"  << "NODE2", elem));
}

void test_xmlcomparer::findTwoChildNodesNested()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesNested));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE2" << "NODE3", elem));

    QVERIFY(!cmp.findElem(QStringList() << "foo", elem));
    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE2" << "NODE3", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "foo" << "NODE3", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "NODE2" << "foo", elem));
}

void test_xmlcomparer::findTwoChildNodesHasAttributes()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesWithAttributes));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE3", elem));
    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE3", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "NODE2"  << "NODE2", elem));
}

void test_xmlcomparer::identFoundElemByAttribCount()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesWithAttributes));

    QDomElement elem;
    cmp.findElem(QStringList() << "NODE1", elem);
    QCOMPARE(elem.attributes().count(), 0);

    cmp.findElem(QStringList() << "NODE1" << "NODE2", elem);
    QCOMPARE(elem.attributes().count(), 2);

    cmp.findElem(QStringList() << "NODE1" << "NODE3", elem);
    QCOMPARE(elem.attributes().count(), 3);
}

