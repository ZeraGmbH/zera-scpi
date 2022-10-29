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
    XmlDocument doc;
    QVERIFY(!doc.loadXml("foo"));
}

void test_xmldocument::loadValid()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlOneNode, true));
}

void test_xmldocument::countRootNode()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlOneNode, true));
    QCOMPARE(doc.getElemCount(), 1);
}

void test_xmldocument::countOneChildNode()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlOneChildNode, true));
    QCOMPARE(doc.getElemCount(), 2);
}

void test_xmldocument::countTwoChildNodes()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodes, true));
    QCOMPARE(doc.getElemCount(), 3);

}

void test_xmldocument::countTwoChildNodesNested()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodesNested, true));
    QCOMPARE(doc.getElemCount(), 3);
}

void test_xmldocument::countTwoChildNodesAttributes()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodesWithAttributes, true));
    QCOMPARE(doc.getElemCount(), 3);
}

void test_xmldocument::findEmptySearch()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlOneNode, true));
    QDomElement elem;
    QVERIFY(!doc.findElem(QStringList(), elem));
}

void test_xmldocument::findRootNode()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlOneNode, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
}

void test_xmldocument::noFindRootNode()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlOneNode, true));
    QDomElement elem;
    QVERIFY(!doc.findElem(QStringList() << "foo", elem));
}

void test_xmldocument::findOneChildNode()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlOneChildNode, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2", elem));

    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo", elem));
}

void test_xmldocument::findTwoChildNodes()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodes, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE3", elem));

    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE3", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "NODE2"  << "NODE2", elem));
}

void test_xmldocument::findTwoChildNodesNested()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodesNested, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2" << "NODE3", elem));

    QVERIFY(!doc.findElem(QStringList() << "foo", elem));
    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2" << "NODE3", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo" << "NODE3", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "NODE2" << "foo", elem));
}

void test_xmldocument::findTwoChildNodesHasAttributes()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodesWithAttributes, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE3", elem));

    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE3", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "NODE2"  << "NODE2", elem));
}

void test_xmldocument::identFoundElemByAttribCount()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodesWithAttributes, true));

    QDomElement elem;
    doc.findElem(QStringList() << "NODE1", elem);
    QCOMPARE(elem.attributes().count(), 0);

    doc.findElem(QStringList() << "NODE1" << "NODE2", elem);
    QCOMPARE(elem.attributes().count(), 2);

    doc.findElem(QStringList() << "NODE1" << "NODE3", elem);
    QCOMPARE(elem.attributes().count(), 3);
}

