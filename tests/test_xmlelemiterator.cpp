#include "test_xmlelemiterator.h"
#include "xmldocument.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiterator)

void test_xmlelemiterator::noDocNoParent()
{
    QDomElement elem;
    XmlElemIterator iter(elem);
    QStringList parentPath = iter.getParentPath();
    QVERIFY(parentPath.empty());
}

void test_xmlelemiterator::noParentNotInDoc()
{
    QDomDocument doc;
    QDomElement freeNode = doc.createElement("root");
    XmlElemIterator iter(freeNode);
    QCOMPARE(iter.getParentPath(), QStringList());
}

void test_xmlelemiterator::root()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    XmlElemIterator iter(root);
    QCOMPARE(iter.getParentPath(), QStringList());
}

void test_xmlelemiterator::childOfRoot()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QDomElement child = doc.createElement("child");
    root.appendChild(child);
    XmlElemIterator iter(child);
    QCOMPARE(iter.getParentPath(), QStringList("root"));
}

void test_xmlelemiterator::grandChildOfRoot()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QDomElement child = doc.createElement("child");
    root.appendChild(child);
    QDomElement grandChild = doc.createElement("grandChild");
    child.appendChild(grandChild);

    XmlElemIterator iter(grandChild);
    QCOMPARE(iter.getParentPath(), QStringList() << "root" << "child");
}

void test_xmlelemiterator::beginEmpty()
{
    XmlDocument doc;
    XmlElemIterator iter = doc.begin();
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getCurrElem().isNull());
}

QString xmlRootOnly =
    "<root/>";

QString xmlOneChild =
    "<root>"
        "<child1/>"
    "</root>";

void test_xmlelemiterator::beginRoot()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIterator iter = doc.begin();
    QCOMPARE(iter.getParentPath(), QStringList());
    QDomElement rootElem = iter.getCurrElem();
    QCOMPARE(rootElem.tagName(), "root");
}

void test_xmlelemiterator::endEmpty()
{
    XmlDocument doc;
    XmlElemIterator iter = doc.end();
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getCurrElem().isNull());
}

void test_xmlelemiterator::endValid()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlRootOnly, true));
    XmlElemIterator iter = doc.end();
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

void test_xmlelemiterator::iterateOne()
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


void test_xmlelemiterator::iterateTwoNested()
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

void test_xmlelemiterator::iterateTwoParallel()
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

void test_xmlelemiterator::iterateTwoOneGrand()
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

void test_xmlelemiterator::iterateTwoOneGGrand()
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

void test_xmlelemiterator::iterateTwoOneGGGrand()
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

void test_xmlelemiterator::iterateTwoOneGGGrandMulNeighbors()
{
    XmlDocument doc;
    QVERIFY(doc.loadXml(xmlFindUpUpUpMultipleNeighbors, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2" << "child3" << "grandchild1" << "ggrandchild1" << "gggrandchild1";
    QStringList unknownTags = iteratePre(doc, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
}

