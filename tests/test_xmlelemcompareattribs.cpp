#include "test_xmlelemcompareattribs.h"
#include "xmlelemcompareattribs.h"
#include <QTest>

QTEST_MAIN(test_xmlelemcompareattribs
)

void test_xmlelemcompareattribs::init()
{
    m_doc = new QDomDocument;
    m_elem1 = new QDomElement;
    m_elem2 = new QDomElement;
    *m_elem1 = m_doc->createElement("elem1");
    *m_elem2 = m_doc->createElement("elem2");
}

void test_xmlelemcompareattribs::cleanup()
{
    delete m_elem2;
    delete m_elem1;
    delete m_doc;
}

void test_xmlelemcompareattribs::compareBothEmpty()
{
    QVERIFY(XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareNoDocRemainsEmptyAlthoughSet()
{
    QDomElement elemNoDoc;
    elemNoDoc.setAttribute("foo", "fooVal");
    QVERIFY(XmlElemCompareAttribs::compare(*m_elem1, elemNoDoc));
}

void test_xmlelemcompareattribs::compareElemEmpty1()
{
    m_elem2->setAttribute("foo", "fooVal");
    QVERIFY(!XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareElemEmpty2()
{
    m_elem1->setAttribute("foo", "fooVal");
    QVERIFY(!XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareOneUnequal()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("foo", "barVal");
    QVERIFY(!XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareOne()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("foo", "fooVal");
    QVERIFY(XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareTwoElem1MissingAttrib()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("bar", "barVal");
    QVERIFY(!XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareTwoElem2MissingAttrib()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "fooVal");
    QVERIFY(!XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareTwoFirstValUnequal()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "");
    m_elem2->setAttribute("bar", "barVal");
    QVERIFY(!XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareTwoSecondValUnequal()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("bar", "");
    QVERIFY(!XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareTwoDifferentAttribNames()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("honk", "barVal");
    QVERIFY(!XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareTwo()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("bar", "barVal");
    QVERIFY(XmlElemCompareAttribs::compare(*m_elem1, *m_elem2));
}

void test_xmlelemcompareattribs::compareSameText()
{
    QDomDocument doc;
    QVERIFY(doc.setContent(QString("<root>text</root>")));
    QDomElement elem = doc.firstChildElement();
    QCOMPARE(elem.text(), "text");
    QVERIFY(XmlElemCompareAttribs::compare(elem, elem));
}

void test_xmlelemcompareattribs::compareDiffText()
{
    QDomDocument doc1;
    QVERIFY(doc1.setContent(QString("<root>text1</root>")));
    QDomElement elem1 = doc1.firstChildElement();
    QCOMPARE(elem1.text(), "text1");
    QDomDocument doc2;
    QVERIFY(doc2.setContent(QString("<root>text2</root>")));
    QDomElement elem2 = doc2.firstChildElement();
    QCOMPARE(elem2.text(), "text2");
    QVERIFY(XmlElemCompareAttribs::compare(elem1, elem2));
}
