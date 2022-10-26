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
    QDomNode node;
    QVERIFY(!cmp.findNode(QStringList(), node));
}

void test_xmlcomparer::findRootNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QDomNode node;
    QVERIFY(cmp.findNode(QStringList() << "NODE1", node));
}

void test_xmlcomparer::noFindRootNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QDomNode node;
    QVERIFY(!cmp.findNode(QStringList() << "foo", node));
}

void test_xmlcomparer::findOneChildNode()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlOneChildNode));
    QDomNode node;
    QVERIFY(cmp.findNode(QStringList() << "NODE1", node));
    QVERIFY(cmp.findNode(QStringList() << "NODE1" << "NODE2", node));
    QVERIFY(!cmp.findNode(QStringList() << "foo" << "NODE2", node));
    QVERIFY(!cmp.findNode(QStringList() << "NODE1" << "foo", node));
}

void test_xmlcomparer::findTwoChildNodes()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodes));
    QDomNode node;
    QVERIFY(cmp.findNode(QStringList() << "NODE1", node));
    QVERIFY(cmp.findNode(QStringList() << "NODE1" << "NODE2", node));
    QVERIFY(cmp.findNode(QStringList() << "NODE1" << "NODE3", node));
    QVERIFY(!cmp.findNode(QStringList() << "foo" << "NODE2", node));
    QVERIFY(!cmp.findNode(QStringList() << "foo" << "NODE3", node));
    QVERIFY(!cmp.findNode(QStringList() << "NODE1" << "foo", node));
    QVERIFY(!cmp.findNode(QStringList() << "NODE1" << "NODE2"  << "NODE2", node));
}

void test_xmlcomparer::findTwoChildNodesNested()
{
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesNested));
    QDomNode node;
    QVERIFY(cmp.findNode(QStringList() << "NODE1", node));
    QVERIFY(cmp.findNode(QStringList() << "NODE1" << "NODE2", node));
    QVERIFY(cmp.findNode(QStringList() << "NODE1" << "NODE2" << "NODE3", node));

    QVERIFY(!cmp.findNode(QStringList() << "foo", node));
    QVERIFY(!cmp.findNode(QStringList() << "foo" << "NODE2", node));
    QVERIFY(!cmp.findNode(QStringList() << "NODE1" << "foo", node));
    QVERIFY(!cmp.findNode(QStringList() << "foo" << "NODE2" << "NODE3", node));
    QVERIFY(!cmp.findNode(QStringList() << "NODE1" << "foo" << "NODE3", node));
    QVERIFY(!cmp.findNode(QStringList() << "NODE1" << "NODE2" << "foo", node));
}

