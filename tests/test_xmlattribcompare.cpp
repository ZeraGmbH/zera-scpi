#include "test_xmlattribcompare.h"
#include "xmlattribcompare.h"
#include <QTest>

QTEST_MAIN(test_xmlattribcompare)

void test_xmlattribcompare::compareBothEmpty()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    QMap<QString, QString> attribKeyValues;
    QVERIFY(XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareNoDoc()
{
    QDomElement elem;
    elem.setAttribute("foo", "fooVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    QVERIFY(!XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareElemEmpty()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    QVERIFY(!XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareAttribsEmpty()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    QMap<QString, QString> attribKeyValues;
    QVERIFY(!XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareOneUnequal()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooBar";
    QVERIFY(!XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareOne()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    QVERIFY(XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareTwoElemMissing()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    attribKeyValues["bar"] = "barVal";
    QVERIFY(!XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareTwoAttribMissing()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    elem.setAttribute("bar", "barVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    QVERIFY(!XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareTwoFirstValUnequal()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    elem.setAttribute("bar", "barVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "";
    attribKeyValues["bar"] = "barVal";
    QVERIFY(!XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareTwoSecondValUnequal()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    elem.setAttribute("bar", "barVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    attribKeyValues["bar"] = "";
    QVERIFY(!XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareTwoDifferentAttribNames()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    elem.setAttribute("bar", "barVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    attribKeyValues["honk"] = "barVal";
    QVERIFY(!XmlAttribCompare::compare(elem, attribKeyValues));
}

void test_xmlattribcompare::compareTwo()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    elem.setAttribute("bar", "barVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    attribKeyValues["bar"] = "barVal";
    QVERIFY(XmlAttribCompare::compare(elem, attribKeyValues));
}

