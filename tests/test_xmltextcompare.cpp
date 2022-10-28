#include "test_xmltextcompare.h"
#include "xmltextcompare.h"
#include <QTest>

QTEST_MAIN(test_xmltextcompare)

void test_xmltextcompare::bothEmpty()
{
    QDomDocument doc;
    QVERIFY(doc.setContent(QString("<root></root>")));
    QDomElement elem = doc.firstChildElement();
    QCOMPARE(elem.text(), "");
    QVERIFY(XmlTextCompare::compare(elem, elem));
}

void test_xmltextcompare::oneEmpty()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root></root>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root>foo</root>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "foo");
    QVERIFY(!XmlTextCompare::compare(elem1, elem2));
}

void test_xmltextcompare::equal()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root>foo</root>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "foo");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root>foo</root>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "foo");
    QVERIFY(XmlTextCompare::compare(elem1, elem2));
}

void test_xmltextcompare::unequal()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root>foo</root>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "foo");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root>bar</root>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "bar");
    QVERIFY(!XmlTextCompare::compare(elem1, elem2));
}

void test_xmltextcompare::firstHasAttributes()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root a='1' b='2'/>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root>foo</root>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "foo");
    QVERIFY(XmlTextCompare::compare(elem1, elem2));
}

void test_xmltextcompare::secondHasAttibutes()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root>foo</root>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "foo");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root a='1' b='2'/>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "");
    QVERIFY(XmlTextCompare::compare(elem1, elem2));
}
