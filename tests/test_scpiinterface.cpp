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

void test_scpiinterface::addRoot()
{
    cSCPI interface("dev");
    SCPITestObject obj("foo", SCPI::isQuery);
    interface.insertScpiCmd(QStringList(), &obj);
    QCOMPARE(interface.getSCPIObject(QString("foo")), &obj);
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

void test_scpiinterface::addFindTwoIdenticalSecondOverwrites()
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

void test_scpiinterface::addFindCaseInsensitive1()
{
    cSCPI interface("dev");
    SCPITestObject obj1("xxxx", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root" << "child", &obj1);
    SCPITestObject obj2("XXXX", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "ROOT" << "CHILD", &obj2);
    QCOMPARE(interface.getSCPIObject(QString("root:child:xxxx")), &obj2);
    QCOMPARE(interface.getSCPIObject(QString("ROOT:CHILD:XXXX")), &obj2);
}

void test_scpiinterface::addFindCaseInsensitive2()
{
    cSCPI interface("dev");
    SCPITestObject obj1("XXXX", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "ROOT" << "CHILD", &obj1);
    SCPITestObject obj2("xxxx", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root" << "child", &obj2);
    QCOMPARE(interface.getSCPIObject(QString("root:child:xxxx")), &obj2);
    QCOMPARE(interface.getSCPIObject(QString("ROOT:CHILD:XXXX")), &obj2);
}

void test_scpiinterface::addFindExactShortLong()
{
    cSCPI interface("dev");
    SCPITestObject obj1("xxxxxx", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "bbbbbb", &obj1);
    QCOMPARE(interface.getSCPIObject(QString("bbbb:xxxx")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("bbbbbb:xxxxxx")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("bbbbb:xxxx")), nullptr);
    QCOMPARE(interface.getSCPIObject(QString("bbbb:xxxxx")), nullptr);
}

void test_scpiinterface::addFindExactShortLongVowelA()
{
    cSCPI interface("dev");
    SCPITestObject obj1("x", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "aaaaaa", &obj1);
    QCOMPARE(interface.getSCPIObject(QString("aaa:x")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("aaaaaa:x")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("aaaa:x")), nullptr);
    QCOMPARE(interface.getSCPIObject(QString("aaaaa:x")), nullptr);
}

void test_scpiinterface::addFindExactShortLongVowelE()
{
    cSCPI interface("dev");
    SCPITestObject obj1("x", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "eeeeee", &obj1);
    QCOMPARE(interface.getSCPIObject(QString("eee:x")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("eeeeee:x")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("eeee:x")), nullptr);
    QCOMPARE(interface.getSCPIObject(QString("eeeee:x")), nullptr);
}

void test_scpiinterface::addFindExactShortLongVowelI()
{
    cSCPI interface("dev");
    SCPITestObject obj1("x", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "iiiiii", &obj1);
    QCOMPARE(interface.getSCPIObject(QString("iii:x")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("iiiiii:x")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("iiii:x")), nullptr);
    QCOMPARE(interface.getSCPIObject(QString("iiiii:x")), nullptr);
}

void test_scpiinterface::addFindExactShortLongVowelO()
{
    cSCPI interface("dev");
    SCPITestObject obj1("x", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "uuuuuu", &obj1);
    QCOMPARE(interface.getSCPIObject(QString("uuu:x")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("uuuuuu:x")), &obj1);
    QCOMPARE(interface.getSCPIObject(QString("uuuu:x")), nullptr);
    QCOMPARE(interface.getSCPIObject(QString("uuuuu:x")), nullptr);
}

void test_scpiinterface::addFindExactShortLongVowelU()
{

}
