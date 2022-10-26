#include "test_xmlcomparer.h"
#include "xmlcomparer.h"
#include <QTest>

QTEST_MAIN(test_xmlcomparer)

void test_xmlcomparer::loadInvalid()
{
    XmlComparer cmp;
    QVERIFY(!cmp.loadXml("foo"));
}

void test_xmlcomparer::loadValid()
{
    QString xml =
        "<!DOCTYPE SCPIModel>"
        "<MODELLIST>"
            "<DEVICE>MT310s2</DEVICE>"
        "</MODELLIST>";
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xml));
}

void test_xmlcomparer::checkRootNode()
{
    QString xml =
        "<!DOCTYPE SCPIModel>"
        "<NODE1></NODE1>";
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xml));
    QCOMPARE(cmp.getNodeCount(), 1);
}

void test_xmlcomparer::checkOneChildNode()
{
    QString xml =
        "<!DOCTYPE SCPIModel>"
        "<NODE1>"
            "<NODE2></NODE2>"
        "</NODE1>";
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xml));
    QCOMPARE(cmp.getNodeCount(), 2);
}

void test_xmlcomparer::checkTwoChildNodes()
{
    QString xml =
        "<!DOCTYPE SCPIModel>"
        "<NODE1>"
            "<NODE2></NODE2>"
            "<NODE3></NODE3>"
        "</NODE1>";
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xml));
    QCOMPARE(cmp.getNodeCount(), 3);

}

void test_xmlcomparer::checkTwoChildNodesNested()
{
    QString xml =
        "<!DOCTYPE SCPIModel>"
        "<NODE1>"
            "<NODE2>"
                "<NODE3></NODE3>"
            "</NODE2>"
        "</NODE1>";
    XmlComparer cmp;
    QVERIFY(cmp.loadXml(xml));
    QCOMPARE(cmp.getNodeCount(), 3);
}

