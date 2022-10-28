#include "test_xmlattribcompare.h"
#include "xmlattribcompare.h"
#include <QTest>

QTEST_MAIN(test_xmlattribcompare)

void test_xmlattribcompare::init()
{
    m_doc = new QDomDocument;
    m_elem1 = new QDomElement;
    m_elem2 = new QDomElement;
    *m_elem1 = m_doc->createElement("elem1");
    *m_elem2 = m_doc->createElement("elem2");
}

void test_xmlattribcompare::cleanup()
{
    delete m_elem2;
    delete m_elem1;
    delete m_doc;
}

void test_xmlattribcompare::compareBothEmpty()
{
    QVERIFY(XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareNoDocRemainsEmptyAlthoughSet()
{
    QDomElement elemNoDoc;
    elemNoDoc.setAttribute("foo", "fooVal");
    QVERIFY(XmlAttribCompare::compare(*m_elem1, elemNoDoc));
}

void test_xmlattribcompare::compareElemEmpty1()
{
    m_elem2->setAttribute("foo", "fooVal");
    QVERIFY(!XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareElemEmpty2()
{
    m_elem1->setAttribute("foo", "fooVal");
    QVERIFY(!XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareOneUnequal()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("foo", "barVal");
    QVERIFY(!XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareOne()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("foo", "fooVal");
    QVERIFY(XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareTwoElem1MissingAttrib()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("bar", "barVal");
    QVERIFY(!XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareTwoElem2MissingAttrib()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "fooVal");
    QVERIFY(!XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareTwoFirstValUnequal()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "");
    m_elem2->setAttribute("bar", "barVal");
    QVERIFY(!XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareTwoSecondValUnequal()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("bar", "");
    QVERIFY(!XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareTwoDifferentAttribNames()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("honk", "barVal");
    QVERIFY(!XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

void test_xmlattribcompare::compareTwo()
{
    m_elem1->setAttribute("foo", "fooVal");
    m_elem1->setAttribute("bar", "barVal");
    m_elem2->setAttribute("foo", "fooVal");
    m_elem2->setAttribute("bar", "barVal");
    QVERIFY(XmlAttribCompare::compare(*m_elem1, *m_elem2));
}

