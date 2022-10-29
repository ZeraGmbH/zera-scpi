#include "test_xmldocument.h"
#include "xmldocument.h"
#include <QTest>

QTEST_MAIN(test_xmldocument)

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

void test_xmldocument::loadInvalid()
{
    XmlDocument cmp;
    QVERIFY(!cmp.loadXml("foo"));
}

void test_xmldocument::loadValid()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlOneNode, true));
}

void test_xmldocument::countRootNode()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlOneNode, true));
    QCOMPARE(cmp.getElemCount(), 1);
}

void test_xmldocument::countOneChildNode()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlOneChildNode, true));
    QCOMPARE(cmp.getElemCount(), 2);
}

void test_xmldocument::countTwoChildNodes()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodes, true));
    QCOMPARE(cmp.getElemCount(), 3);

}

void test_xmldocument::countTwoChildNodesNested()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesNested, true));
    QCOMPARE(cmp.getElemCount(), 3);
}

void test_xmldocument::countTwoChildNodesAttributes()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesWithAttributes, true));
    QCOMPARE(cmp.getElemCount(), 3);
}

void test_xmldocument::findEmptySearch()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlOneNode, true));
    QDomElement elem;
    QVERIFY(!cmp.findElem(QStringList(), elem));
}

void test_xmldocument::findRootNode()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlOneNode, true));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
}

void test_xmldocument::noFindRootNode()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlOneNode, true));
    QDomElement elem;
    QVERIFY(!cmp.findElem(QStringList() << "foo", elem));
}

void test_xmldocument::findOneChildNode()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlOneChildNode, true));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE2", elem));

    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "foo", elem));
}

void test_xmldocument::findTwoChildNodes()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodes, true));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE3", elem));

    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE3", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "NODE2"  << "NODE2", elem));
}

void test_xmldocument::findTwoChildNodesNested()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesNested, true));
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

void test_xmldocument::findTwoChildNodesHasAttributes()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesWithAttributes, true));
    QDomElement elem;
    QVERIFY(cmp.findElem(QStringList() << "NODE1", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(cmp.findElem(QStringList() << "NODE1" << "NODE3", elem));

    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!cmp.findElem(QStringList() << "foo" << "NODE3", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!cmp.findElem(QStringList() << "NODE1" << "NODE2"  << "NODE2", elem));
}

void test_xmldocument::identFoundElemByAttribCount()
{
    XmlDocument cmp;
    QVERIFY(cmp.loadXml(xmlTwoChildNodesWithAttributes, true));

    QDomElement elem;
    cmp.findElem(QStringList() << "NODE1", elem);
    QCOMPARE(elem.attributes().count(), 0);

    cmp.findElem(QStringList() << "NODE1" << "NODE2", elem);
    QCOMPARE(elem.attributes().count(), 2);

    cmp.findElem(QStringList() << "NODE1" << "NODE3", elem);
    QCOMPARE(elem.attributes().count(), 3);
}

