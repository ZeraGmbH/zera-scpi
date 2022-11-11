#include "test_xmlelemiteratortree.h"
#include "xmldocument.h"
#include "xmlelemiterstrategytree.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiteratortree)


void test_xmlelemiteratortree::beginEmpty()
{
    XmlDocument doc;
    XmlElemIter iter = doc.root();
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getElem().isNull());
}

QString xmlRootOnly =
    "<root/>";

QString xmlOneChild =
    "<root>"
        "<child1/>"
    "</root>";

void test_xmlelemiteratortree::beginRoot()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIter iter = doc.root();
    QCOMPARE(iter.getParentPath(), QStringList());
    QDomElement rootElem = iter.getElem();
    QCOMPARE(rootElem.tagName(), "root");
}

static QStringList iterateCheckExpected(XmlDocument doc, QStringList &expectedTags)
{
    QStringList unexpectedTags;
    for(auto iter=doc.root(); !iter.isEnd(); iter.next()) {
        QDomElement elem = iter.getElem();
        if(!expectedTags.removeOne(elem.tagName())) {
            unexpectedTags.append(elem.tagName());
        }
    }
    return unexpectedTags;
}

void test_xmlelemiteratortree::iterateOne()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlOneChild, true));
    QStringList expectedTags = QStringList() << "root" << "child1";
    QStringList unexpectedTags = iterateCheckExpected(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unexpectedTags, QStringList());
}

QString xmlTwoNested =
    "<root>"
        "<child1>"
            "<grandchild1/>"
        "</child1>"
    "</root>";


void test_xmlelemiteratortree::iterateTwoNested()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlTwoNested, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "grandchild1";
    QStringList unexpectedTags = iterateCheckExpected(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unexpectedTags, QStringList());
}

QString xmlParallel =
    "<root>"
        "<child1/>"
        "<child2/>"
    "</root>";

void test_xmlelemiteratortree::iterateTwoParallel()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlParallel, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2";
    QStringList unexpectedTags = iterateCheckExpected(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unexpectedTags, QStringList());
}

QString xmlFindUp =
    "<root>"
        "<child1>"
            "<grandchild1/>"
        "</child1>"
        "<child2/>"
    "</root>";

void test_xmlelemiteratortree::iterateTwoOneGrand()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlFindUp, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2" << "grandchild1";
    QStringList unexpectedTags = iterateCheckExpected(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unexpectedTags, QStringList());
}

QString xmlFindUpUp =
    "<root>"
        "<child1>"
            "<grandchild1>"
                "<ggrandchild1/>"
            "</grandchild1>"
        "</child1>"
        "<child2/>"
    "</root>";

void test_xmlelemiteratortree::iterateTwoOneGGrand()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlFindUpUp, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2" << "grandchild1" << "ggrandchild1";
    QStringList unexpectedTags = iterateCheckExpected(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unexpectedTags, QStringList());
}

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

void test_xmlelemiteratortree::iterateTwoOneGGGrand()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlFindUpUpUp, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2" << "grandchild1" << "ggrandchild1" << "gggrandchild1";
    QStringList unexpectedTags = iterateCheckExpected(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unexpectedTags, QStringList());
}

QString xmlFindUpUpUpMultipleNeighbors =
    "<root>"
        "<child1>"
            "<grandchild1>"
                "<ggrandchild1>"
                    "<gggrandchild1/>"
                "</ggrandchild1>"
            "</grandchild1>"
        "</child1>"
        "<child2/>"
        "<child3/>"
    "</root>";

void test_xmlelemiteratortree::iterateTwoOneGGGrandMulNeighbors()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlFindUpUpUpMultipleNeighbors, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2" << "child3" << "grandchild1" << "ggrandchild1" << "gggrandchild1";
    QStringList unexpectedTags = iterateCheckExpected(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unexpectedTags, QStringList());
}

void test_xmlelemiteratortree::iterateNestedArray()
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
    XmlDocument doc;
    QVERIFY(doc.loadXml(xml, true));
    QStringList expectedTags = QStringList() << "NODE1" << "NODE2" << "NODE3" << "NODE3" << "NODE2" << "NODE3" << "NODE3";
    QStringList unexpectedTags = iterateCheckExpected(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unexpectedTags, QStringList());
}

