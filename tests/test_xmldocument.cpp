#include "test_xmldocument.h"
#include "xmlqtdomdociteration.h"
#include <QTest>

QTEST_MAIN(test_xmldocument)

QString xmlOneNode =
    "<NODE1/>";

QString xmlOneChildNode =
    "<NODE1>"
        "<NODE2/>"
    "</NODE1>";

QString xmlTwoChildNodes =
    "<NODE1>"
        "<NODE2/>"
        "<NODE3/>"
    "</NODE1>";

// see https://www.w3schools.com/xml/xml_attributes.asp
QString xmlTwoChildNodesWithAttributes =
    "<NODE1>"
        "<NODE2 a='1' b='2'/>"
        "<NODE3 c='3' d='4' e='5'></NODE3>"
    "</NODE1>";

QString xmlTwoChildNodesNested =
    "<NODE1>"
        "<NODE2>"
            "<NODE3/>"
        "</NODE2>"
    "</NODE1>";

void test_xmldocument::loadInvalid()
{
    XmlQtDomDocIteration doc;
    QVERIFY(!doc.loadXml("foo"));
}

void test_xmldocument::loadValid()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlOneNode, true));
}

void test_xmldocument::setDoc()
{
    QDomDocument domDoc;
    domDoc.setContent(xmlOneNode);
    XmlQtDomDocIteration doc;
    doc.setXmlDomDoc(domDoc);
    QDomElement elem;
    QVERIFY(!doc.findElem(QStringList() << "foo", elem));
}

void test_xmldocument::findEmptySearch()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlOneNode, true));
    QDomElement elem;
    QVERIFY(!doc.findElem(QStringList(), elem));
}

void test_xmldocument::findRootNode()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlOneNode, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
}

void test_xmldocument::noFindRootNode()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlOneNode, true));
    QDomElement elem;
    QVERIFY(!doc.findElem(QStringList() << "foo", elem));
}

void test_xmldocument::findOneChildNode()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlOneChildNode, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2", elem));

    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo", elem));
}

void test_xmldocument::findTwoChildNodes()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodes, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE3", elem));

    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE3", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "NODE2"  << "NODE2", elem));
}

void test_xmldocument::findTwoChildNodesNested()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodesNested, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2" << "NODE3", elem));

    QVERIFY(!doc.findElem(QStringList() << "foo", elem));
    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2" << "NODE3", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo" << "NODE3", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "NODE2" << "foo", elem));
}

void test_xmldocument::findTwoChildNodesHasAttributes()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodesWithAttributes, true));
    QDomElement elem;
    QVERIFY(doc.findElem(QStringList() << "NODE1", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE2", elem));
    QVERIFY(doc.findElem(QStringList() << "NODE1" << "NODE3", elem));

    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE2", elem));
    QVERIFY(!doc.findElem(QStringList() << "foo" << "NODE3", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "foo", elem));
    QVERIFY(!doc.findElem(QStringList() << "NODE1" << "NODE2"  << "NODE2", elem));
}

void test_xmldocument::identFoundElemByAttribCount()
{
    XmlQtDomDocIteration doc;
    QVERIFY(doc.loadXml(xmlTwoChildNodesWithAttributes, true));

    QDomElement elem;
    doc.findElem(QStringList() << "NODE1", elem);
    QCOMPARE(elem.attributes().count(), 0);

    doc.findElem(QStringList() << "NODE1" << "NODE2", elem);
    QCOMPARE(elem.attributes().count(), 2);

    doc.findElem(QStringList() << "NODE1" << "NODE3", elem);
    QCOMPARE(elem.attributes().count(), 3);
}

static int countElems(XmlQtDomDocIteration doc)
{
    int elemCount = 0;
    for(auto iter=doc.root(); !iter.isEnd(); iter.next())
        elemCount++;
    return elemCount;
}

/*void test_xmldocument::addRootOnEmpty()
{
    XmlQtDomDocIteration doc;
    QDomElement insertedElem;
    QStringList rootPath("root");
    QVERIFY(doc.addOrFindElem(rootPath, insertedElem));
    QVERIFY(!insertedElem.isNull());
    QDomElement insertedElemCheck;
    QVERIFY(doc.findElem(rootPath, insertedElemCheck));
    QCOMPARE(countElems(doc), 1);
}

void test_xmldocument::addExistingRoot()
{
    XmlQtDomDocIteration doc;
    QDomElement insertedElem;
    QStringList rootPath("root");
    QVERIFY(doc.addOrFindElem(rootPath, insertedElem));
    QDomElement insertedElemSecond;
    QVERIFY(!doc.addOrFindElem(rootPath, insertedElemSecond));
    QCOMPARE(insertedElem.tagName(), insertedElemSecond.tagName());
    QCOMPARE(countElems(doc), 1);
}

void test_xmldocument::addInvalidRoot()
{
    XmlQtDomDocIteration doc;
    QDomElement insertedElem;
    // Note: "0001" is accepted and using
    // QDomImplementation::setInvalidDataPolicy has unforseeable
    // side effects
    QStringList rootPath("");
    QVERIFY(!doc.addOrFindElem(rootPath, insertedElem));
    QVERIFY(insertedElem.isNull());
    QVERIFY(doc.isEmpty());
}

void test_xmldocument::addInvalidNested1()
{
    XmlQtDomDocIteration doc;
    QDomElement insertedElem;
    QStringList rootPath = QStringList() << "" << "foo";
    QVERIFY(!doc.addOrFindElem(rootPath, insertedElem));
    QVERIFY(insertedElem.isNull());
    QVERIFY(doc.isEmpty());
}

void test_xmldocument::addInvalidNested2()
{
    XmlQtDomDocIteration doc;
    QDomElement insertedElem;
    QStringList rootPath = QStringList() << "foo"  << "";
    QVERIFY(!doc.addOrFindElem(rootPath, insertedElem));
    QVERIFY(insertedElem.isNull());
    QVERIFY(doc.isEmpty());
}

void test_xmldocument::addSecondRoot()
{
    XmlQtDomDocIteration doc;
    QDomElement insertedElem;
    QVERIFY(doc.addOrFindElem(QStringList("foo"), insertedElem));
    QVERIFY(!doc.addOrFindElem(QStringList("bar"), insertedElem));
    QVERIFY(insertedElem.isNull());
    QCOMPARE(countElems(doc), 1);
}

void test_xmldocument::addNested1OnEmpty()
{
    XmlQtDomDocIteration doc;
    QDomElement insertedElem;
    QStringList nestedPath = QStringList() << "foo" << "bar";
    QVERIFY(doc.addOrFindElem(nestedPath, insertedElem));
    QVERIFY(!insertedElem.isNull());
    QDomElement insertedElemCheck;
    QVERIFY(doc.findElem(nestedPath, insertedElemCheck));
    QCOMPARE(insertedElemCheck.tagName(), "bar");
    QCOMPARE(countElems(doc), 2);
}

void test_xmldocument::addNested2OnEmpty()
{
    XmlQtDomDocIteration doc;
    QDomElement insertedElem;
    QStringList nestedPath = QStringList() << "foo" << "bar" << "baz";
    QVERIFY(doc.addOrFindElem(nestedPath, insertedElem));
    QVERIFY(!insertedElem.isNull());
    QDomElement insertedElemCheck;
    QVERIFY(doc.findElem(nestedPath, insertedElemCheck));
    QCOMPARE(insertedElemCheck.tagName(), "baz");
    QCOMPARE(countElems(doc), 3);
}*/

