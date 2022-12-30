#include "test_xmlelemcomparetext.h"
#include "xmlelemcomparetext.h"
#include <QTest>

QTEST_MAIN(test_xmlelemcomparetext)

void test_xmlelemcomparetext::bothEmpty()
{
    QDomDocument doc;
    QVERIFY(doc.setContent(QString("<root></root>")));
    QDomElement elem = doc.firstChildElement();
    QCOMPARE(elem.text(), "");
    QVERIFY(XmlElemCompareText::compare(elem, elem));
}

void test_xmlelemcomparetext::oneEmpty()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root></root>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root>foo</root>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "foo");
    QVERIFY(!XmlElemCompareText::compare(elem1, elem2));
}

void test_xmlelemcomparetext::equal()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root>foo</root>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "foo");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root>foo</root>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "foo");
    QVERIFY(XmlElemCompareText::compare(elem1, elem2));
}

void test_xmlelemcomparetext::unequal()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root>foo</root>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "foo");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root>bar</root>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "bar");
    QVERIFY(!XmlElemCompareText::compare(elem1, elem2));
}

void test_xmlelemcomparetext::firstHasAttributes()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root a='1' b='2'/>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root>foo</root>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "foo");
    QVERIFY(XmlElemCompareText::compare(elem1, elem2));
}

void test_xmlelemcomparetext::secondHasAttibutes()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root>foo</root>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "foo");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root a='1' b='2'/>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "");
    QVERIFY(XmlElemCompareText::compare(elem1, elem2));
}
