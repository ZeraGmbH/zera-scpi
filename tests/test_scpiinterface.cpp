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
    QCOMPARE(interface.getSCPIObject(QString("root:foo bar;baz")), &obj);
}
