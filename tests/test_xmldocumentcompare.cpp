#include "test_xmldocumentcompare.h"
#include "xmldocumentcompare.h"
#include <QTest>

QTEST_MAIN(test_xmldocumentcompare)

void test_xmldocumentcompare::bothEmpty()
{
    XmlDocumentCompare compare;
    QVERIFY(compare.compareXml("", ""));
}

void test_xmldocumentcompare::bothSameNoAttribs()
{
    QString xmlFindUpUpUp =
        "<root>"
            "<child1>"
                "<grandchild1>"
                    "<ggrandchild1>"
                        "<gggrandchild1/>"
                    "</ggrandchild1>"
                "</grandchild1>"
            "</child1>"
            "<child2/>"
        "</root>";
    XmlDocumentCompare compare;
    QVERIFY(compare.compareXml(xmlFindUpUpUp, xmlFindUpUpUp, true));
}

void test_xmldocumentcompare::bothDifferentNoAttribs()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2/>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2/>"
            "<NODE3/>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::bothSameNoAttribsDiffSequence()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2/>"
            "<NODE3/>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE3/>"
            "<NODE2/>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(compare.compareXml(xml1, xml2, true));
    QVERIFY(compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::firstEmptySecondValid()
{
    QString xml2 =
        "<NODE1>"
            "<NODE3/>"
            "<NODE2/>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(QString(), xml2));
}

void test_xmldocumentcompare::firstValidSecondEmpty()
{
    QString xml1 =
        "<NODE1>"
            "<NODE3/>"
            "<NODE2/>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, QString()));
}

void test_xmldocumentcompare::attibsUnequal()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2 a='1' b='2'/>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2 a='1' b='2'/>"
            "<NODE3 c='3' d='foo' e='5'></NODE3>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::attibsEqual()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2 a='1' b='2'/>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(compare.compareXml(xml1, xml1, true));
}

void test_xmldocumentcompare::attribsDiffSequence()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2 a='1' b='2'/>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
            "<NODE2 a='1' b='2'/>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(compare.compareXml(xml1, xml2, true));
    QVERIFY(compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::textUnequal()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2>bar</NODE2>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::textEqual()
{
    QString xml =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(compare.compareXml(xml, xml, true));
}

void test_xmldocumentcompare::attibsUnequalTextEqual()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
            "<NODE3 c='3' d='foo' e='5'></NODE3>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::attibsEqualTextUnequal()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2>bar</NODE2>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::attibsEqualTextEqual()
{
    QString xml =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
            "<NODE3 c='3' d='4' e='5'></NODE3>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(compare.compareXml(xml, xml, true));
}

void test_xmldocumentcompare::arraySecondMoreElems()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2 a='1' b='2'></NODE2>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2 a='1' b='2'></NODE2>"
            "<NODE2 a='1' b='2'></NODE2>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::arraySwappedSequence()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2 a='1' b='2'></NODE2>"
            "<NODE2 c='1' d='2'></NODE2>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2 c='1' d='2'></NODE2>"
            "<NODE2 a='1' b='2'></NODE2>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::arrayNestedSwappedSequence()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2>"
                "<NODE3 a='1' b='2'></NODE3>"
                "<NODE3 c='1' d='2'></NODE3>"
            "</NODE2>"
            "<NODE2>"
                "<NODE3 a='1' b='2'></NODE3>"
                "<NODE3 c='1' d='2'></NODE3>"
            "</NODE2>"
        "</NODE1>";
    QString xml2 =
        "<NODE1>"
            "<NODE2>"
                "<NODE3 a='1' b='2'></NODE3>"
                "<NODE3 c='1' d='2'></NODE3>"
            "</NODE2>"
            "<NODE2>"
                "<NODE3 c='1' d='2'></NODE3>"
                "<NODE3 a='1' b='2'></NODE3>"
            "</NODE2>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::arrayEqual()
{
    QString xml =
        "<NODE1>"
            "<NODE2>"
                "<NODE3 a='1' b='2'></NODE3>"
                "<NODE3 c='1' d='2'></NODE3>"
            "</NODE2>"
            "<NODE2>"
                "<NODE3 a='1' b='2'></NODE3>"
                "<NODE3 c='1' d='2'></NODE3>"
            "</NODE2>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(compare.compareXml(xml, xml, true));
}


void test_xmldocumentcompare::docTypeUnequal()
{
    QString xml1 =
        "<!DOCTYPE foo>"
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    QString xml2 =
        "<!DOCTYPE bar>"
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}

void test_xmldocumentcompare::docTypeEqual()
{
    QString xml1 =
        "<!DOCTYPE foo>"
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    QString xml2 =
        "<!DOCTYPE foo>"
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    XmlDocumentCompare compare;
    QVERIFY(compare.compareXml(xml1, xml2, true));
}

void test_xmldocumentcompare::sameSizeDifferentElems()
{
    QString xml1 =
        "<NODE1>"
            "<NODE2>foo</NODE2>"
        "</NODE1>";
    QString xml2 =
        "<NODE3>"
            "<NODE4>foo</NODE4>"
        "</NODE3>";
    XmlDocumentCompare compare;
    QVERIFY(!compare.compareXml(xml1, xml2, true));
    QVERIFY(!compare.compareXml(xml2, xml1, true));
}
