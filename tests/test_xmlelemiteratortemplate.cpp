#include "test_xmlelemiteratortemplate.h"
#include "xmlelemiteratortemplate.h"
#include <QTest>

QTEST_MAIN(test_xmlelemiteratortemplate)

class XmlElemIteratorTest : public XmlElemIteratorTemplate
{
public:
    XmlElemIteratorTest(QDomElement elem) :
        XmlElemIteratorTemplate(elem)
    {
    }
protected:
    QDomElement getNextElem() override
    {
       return m_elem;
    }
};

void test_xmlelemiteratortemplate::noDocNoParent()
{
    QDomElement elem;
    XmlElemIteratorTest iter(elem);
    QStringList parentPath = iter.getParentPath();
    QVERIFY(parentPath.empty());
}

void test_xmlelemiteratortemplate::noParentNotInDoc()
{
    QDomDocument doc;
    QDomElement freeNode = doc.createElement("root");
    XmlElemIteratorTest iter(freeNode);
    QCOMPARE(iter.getParentPath(), QStringList());
}

void test_xmlelemiteratortemplate::root()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    XmlElemIteratorTest iter(root);
    QCOMPARE(iter.getParentPath(), QStringList());
}

void test_xmlelemiteratortemplate::childOfRoot()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QDomElement child = doc.createElement("child");
    root.appendChild(child);
    XmlElemIteratorTest iter(child);
    QCOMPARE(iter.getParentPath(), QStringList("root"));
}

void test_xmlelemiteratortemplate::grandChildOfRoot()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QDomElement child = doc.createElement("child");
    root.appendChild(child);
    QDomElement grandChild = doc.createElement("grandChild");
    child.appendChild(grandChild);

    XmlElemIteratorTest iter(grandChild);
    QCOMPARE(iter.getParentPath(), QStringList() << "root" << "child");
}
