#include "test_xmlelemiteratortree.h"
#include "xmldocument.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiteratortree)


void test_xmlelemiteratortree::beginEmpty()
{
    XmlDocument doc;
    XmlElemIteratorTree iter = doc.begin();
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getCurrElem().isNull());
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
    XmlElemIteratorTree iter = doc.begin();
    QCOMPARE(iter.getParentPath(), QStringList());
    QDomElement rootElem = iter.getCurrElem();
    QCOMPARE(rootElem.tagName(), "root");
}

void test_xmlelemiteratortree::endEmpty()
{
    XmlDocument doc;
    XmlElemIteratorTree iter = doc.end();
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getCurrElem().isNull());
}

void test_xmlelemiteratortree::endValid()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIteratorTree iter = doc.end();
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getCurrElem().isNull());
}

static QStringList iteratePre(XmlDocument doc, QStringList &expectedTags)
{
    QStringList unknownTags;
    for(auto iter=doc.begin(); iter!=doc.end(); ++iter) {
        QDomElement elem = iter.getCurrElem();
        if(!expectedTags.removeOne(elem.tagName())) {
            unknownTags.append(elem.tagName());
        }
    }
    return unknownTags;
}

void test_xmlelemiteratortree::iterateOne()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlOneChild, true));
    QStringList expectedTags = QStringList() << "root" << "child1";
    QStringList unknownTags = iteratePre(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
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
    QStringList unknownTags = iteratePre(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
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
    QStringList unknownTags = iteratePre(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
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
    QStringList unknownTags = iteratePre(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
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
    QStringList unknownTags = iteratePre(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
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
    QStringList unknownTags = iteratePre(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
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
    QStringList unknownTags = iteratePre(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
}

