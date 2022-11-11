#include "test_scpiinterface.h"
#include "scpi.h"
#include "scpiobject.h"
#include <QTest>

class SCPITestObject : public cSCPIObject
{
public:
    SCPITestObject(QString name, quint8 type) : cSCPIObject(name, type) { }
    bool executeSCPI(const QString& sInput, QString& sOutput) override
    {
        return true;
    }
};

QTEST_MAIN(test_scpiinterface)

void test_scpiinterface::mostSimpleAddFindAndLearnBehaviour()
{
    cSCPI interface("dev");
    SCPITestObject obj("foo", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "rootitem", &obj);
    // We learned here:
    // * cSCPI cares only for SCPI path
    // * commands (+parameters) or queries are handled by cSCPICommand

    QCOMPARE(interface.getSCPIObject(QString("rootitem:foo")), &obj);
    QCOMPARE(interface.getSCPIObject(QString("root:foo")), &obj);
    QCOMPARE(interface.getSCPIObject(QString("root: foo")), &obj);
    // interpreted as "root" path -> found but no object
    QCOMPARE(interface.getSCPIObject(QString("root : foo")), nullptr);
    QCOMPARE(interface.getSCPIObject(QString("foo:foo")), nullptr);
    QCOMPARE(interface.getSCPIObject(QString("root:foo?")), &obj);
    QCOMPARE(interface.getSCPIObject(QString("root:foo bar")), &obj);
    QCOMPARE(interface.getSCPIObject(QString("root:foo bar;secondCmd")), &obj);
}

void test_scpiinterface::addFindTwoRoot()
{
    cSCPI interface("dev");
    SCPITestObject obj1("foo", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root1", &obj1);
    SCPITestObject obj2("bar", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root2", &obj2);
    QCOMPARE(interface.getSCPIObject(QString("root1:foo")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("root2:bar")), &obj2);
}

void test_scpiinterface::addFindTwoNestedSamePath()
{
    cSCPI interface("dev");
    SCPITestObject obj1("foo", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root" << "child", &obj1);
    SCPITestObject obj2("bar", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root" << "child", &obj2);
    QCOMPARE(interface.getSCPIObject(QString("root:child:foo")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("root:child:bar")), &obj2);
}

void test_scpiinterface::addFindTwoNestedIdentical()
{
    cSCPI interface("dev");
    SCPITestObject obj1("foo", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root" << "child", &obj1);
    SCPITestObject obj2("foo", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root" << "child", &obj2);
    QCOMPARE(interface.getSCPIObject(QString("root:child:foo")), &obj2);
}

void test_scpiinterface::addFindTwoNestedDiffPath()
{
    cSCPI interface("dev");
    SCPITestObject obj1("foo", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root1" << "child1", &obj1);
    SCPITestObject obj2("bar", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root2" << "child2", &obj2);
    QCOMPARE(interface.getSCPIObject(QString("root1:child1:foo")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("root2:child2:bar")), &obj2);
}

void test_scpiinterface::addFindTwoNestedAlmostDiffPath()
{
    cSCPI interface("dev");
    SCPITestObject obj1("foo", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root" << "child1", &obj1);
    SCPITestObject obj2("bar", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root" << "child2", &obj2);
    QCOMPARE(interface.getSCPIObject(QString("root:child1:foo")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("root:child2:bar")), &obj2);
}
