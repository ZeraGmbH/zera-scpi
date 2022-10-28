#include "test_xmlelemiterator.h"
#include "xmleleminfo.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiterator)

QString xmlOneChildNode =
    "<root>"
        "<child/>"
    "</root>";

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
    XmlElemInfo elemInfo;
    XmlElemIterator iter = elemInfo.begin();
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getElem().isNull());
}

QString xmlRootOnly =
    "<root/>";

QString xmlOneChild =
    "<root>"
        "<child1/>"
    "</root>";

void test_xmlelemiterator::beginRoot()
{
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlRootOnly, true));
    XmlElemIterator iter = elemInfo.begin();
    QCOMPARE(iter.getParentPath(), QStringList());
    QDomElement rootElem = iter.getElem();
    QCOMPARE(rootElem.tagName(), "root");
}

void test_xmlelemiterator::endEmpty()
{
    XmlElemInfo elemInfo;
    XmlElemIterator iter = elemInfo.end();
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getElem().isNull());
}

void test_xmlelemiterator::endValid()
{
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlRootOnly, true));
    XmlElemIterator iter = elemInfo.end();
    QCOMPARE(iter.getParentPath(), QStringList());
    QVERIFY(iter.getElem().isNull());
}

static QStringList iteratePre(XmlElemInfo elemInfo, QStringList &expectedTags)
{
    QStringList unknownTags;
    for(auto iter=elemInfo.begin(); iter!=elemInfo.end(); ++iter) {
        QDomElement elem = iter.getElem();
        if(!expectedTags.removeOne(elem.tagName())) {
            unknownTags.append(elem.tagName());
        }
    }
    return unknownTags;
}

void test_xmlelemiterator::iterateOnePre()
{
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlOneChild, true));
    QStringList expectedTags = QStringList() << "root" << "child1";
    QStringList unknownTags = iteratePre(elemInfo, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
}

void test_xmlelemiterator::iterateOnePost()
{
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlOneChild, true));
    QStringList expectedTags = QStringList() << "root" << "child1";
    QStringList unknownTags;
    for(auto iter=elemInfo.begin(); iter!=elemInfo.end(); iter++) {
        QDomElement elem = iter.getElem();
        if(!expectedTags.removeOne(elem.tagName())) {
            unknownTags.append(elem.tagName());
        }
    }
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
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlTwoNested, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "grandchild1";
    QStringList unknownTags = iteratePre(elemInfo, expectedTags);
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
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlParallel, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2";
    QStringList unknownTags = iteratePre(elemInfo, expectedTags);
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
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlFindUp, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2" << "grandchild1";
    QStringList unknownTags = iteratePre(elemInfo, expectedTags);
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
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlFindUpUp, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2" << "grandchild1" << "ggrandchild1";
    QStringList unknownTags = iteratePre(elemInfo, expectedTags);
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
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlFindUpUpUp, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2" << "grandchild1" << "ggrandchild1" << "gggrandchild1";
    QStringList unknownTags = iteratePre(elemInfo, expectedTags);
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
    XmlElemInfo elemInfo;
    QVERIFY(elemInfo.loadXml(xmlFindUpUpUpMultipleNeighbors, true));
    QStringList expectedTags = QStringList() << "root" << "child1" << "child2" << "child3" << "grandchild1" << "ggrandchild1" << "gggrandchild1";
    QStringList unknownTags = iteratePre(elemInfo, expectedTags);
    QCOMPARE(expectedTags, QStringList());
    QCOMPARE(unknownTags, QStringList());
}

