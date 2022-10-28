#include "test_xmleleminfo.h"
#include "xmleleminfo.h"
#include <QTest>

QTEST_MAIN(test_xmleleminfo)

QString xmlOneNode =
    "<NODE1/>";

QString xmlOneChildNode =
    "<NODE1>"
        "<NODE2/>"
    "</NODE1>";

QString xmlTwoChildNodes =
    "<NODE1>"
        "<NODE2/>"
        "<NODE3/>"
    "</NODE1>";

// see https://www.w3schools.com/xml/xml_attributes.asp
QString xmlTwoChildNodesWithAttributes =
    "<NODE1>"
        "<NODE2 a='1' b='2'/>"
        "<NODE3 c='3' d='4' e='5'></NODE3>"
    "</NODE1>";

QString xmlTwoChildNodesNested =
    "<NODE1>"
        "<NODE2>"
            "<NODE3/>"
        "</NODE2>"
    "</NODE1>";

void test_xmleleminfo::loadInvalid()
{
    XmlElemInfo cmp;
    QVERIFY(!cmp.loadXml("foo"));
}

void test_xmleleminfo::loadValid()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
}

void test_xmleleminfo::countRootNode()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QCOMPARE(cmp.getElemCount(), 1);
}

void test_xmleleminfo::countOneChildNode()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlOneChildNode));
    QCOMPARE(cmp.getElemCount(), 2);
}

void test_xmleleminfo::countTwoChildNodes()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodes));
    QCOMPARE(cmp.getElemCount(), 3);

}

void test_xmleleminfo::countTwoChildNodesNested()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesNested));
    QCOMPARE(cmp.getElemCount(), 3);
}

void test_xmleleminfo::countTwoChildNodesAttributes()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesWithAttributes));
    QCOMPARE(cmp.getElemCount(), 3);
}

void test_xmleleminfo::findEmptySearch()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QDomElement elem;
    QVERIFY(!cmp.findElem(QStringList(), elem));
}

void test_xmleleminfo::findRootNode()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
}

void test_xmleleminfo::noFindRootNode()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlOneNode));
    QDomElement elem;
    QVERIFY(!cmp.findElem(QStringList() << "foo", elem));
}

void test_xmleleminfo::findOneChildNode()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlOneChildNode));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE2", elem));

    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "foo", elem));
}

void test_xmleleminfo::findTwoChildNodes()
{
    XmlElemInfo cmp;
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

void test_xmleleminfo::findTwoChildNodesNested()
{
    XmlElemInfo cmp;
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

void test_xmleleminfo::findTwoChildNodesHasAttributes()
{
    XmlElemInfo cmp;
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

void test_xmleleminfo::identFoundElemByAttribCount()
{
    XmlElemInfo cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesWithAttributes));

    QDomElement elem;
    cmp.findElem(QStringList() << "NODE1", elem);
    QCOMPARE(elem.attributes().count(), 0);

    cmp.findElem(QStringList() << "NODE1" << "NODE2", elem);
    QCOMPARE(elem.attributes().count(), 2);

    cmp.findElem(QStringList() << "NODE1" << "NODE3", elem);
    QCOMPARE(elem.attributes().count(), 3);
}

