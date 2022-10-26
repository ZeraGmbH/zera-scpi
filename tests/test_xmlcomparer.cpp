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

QString xmlTwoChildNodesWithAttributes =
    "<!DOCTYPE SCPIModel>"
    "<NODE1>"
        "<NODE2>a=1 b=2</NODE2>"
        "<NODE3>c=\"3\" d=\"4\"</NODE3>"
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

void test_xmlcomparer::checkRootNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QCOMPARE(cmp.getNodeCount(), 1);
}

void test_xmlcomparer::checkOneChildNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneChildNode));
    QCOMPARE(cmp.getNodeCount(), 2);
}

void test_xmlcomparer::checkTwoChildNodes()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodes));
    QCOMPARE(cmp.getNodeCount(), 3);

}

void test_xmlcomparer::checkTwoChildNodesNested()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesNested));
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


