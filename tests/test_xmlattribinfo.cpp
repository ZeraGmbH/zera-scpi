#include "test_xmlattribinfo.h"
#include "xmlattribinfo.h"
#include <QTest>

QTEST_MAIN(test_xmlattribinfo)

void test_xmlattribinfo::compareBothEmpty()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    QMap<QString, QString> attribKeyValues;
    QVERIFY(XmlAttribInfo::compareElemAttribs(elem, attribKeyValues));
}

void test_xmlattribinfo::compareElemEmpty()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    QVERIFY(!XmlAttribInfo::compareElemAttribs(elem, attribKeyValues));
}

void test_xmlattribinfo::compareAttribsEmpty()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    QMap<QString, QString> attribKeyValues;
    QVERIFY(!XmlAttribInfo::compareElemAttribs(elem, attribKeyValues));
}

void test_xmlattribinfo::compareOneUnequal()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooBar";
    QVERIFY(!XmlAttribInfo::compareElemAttribs(elem, attribKeyValues));
}

void test_xmlattribinfo::compareOne()
{
    QDomDocument doc;
    QDomElement elem = doc.createElement("foo");
    elem.setAttribute("foo", "fooVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    QVERIFY(XmlAttribInfo::compareElemAttribs(elem, attribKeyValues));
}

void test_xmlattribinfo::compareNoDoc()
{
    QDomElement elem;
    elem.setAttribute("foo", "fooVal");
    QMap<QString, QString> attribKeyValues;
    attribKeyValues["foo"] = "fooVal";
    QVERIFY(!XmlAttribInfo::compareElemAttribs(elem, attribKeyValues));
}


