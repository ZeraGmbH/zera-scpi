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

void test_scpiinterface::singleCmd()
{
    cSCPI interface("dev");
    SCPITestObject obj("foo", SCPI::isQuery);
    interface.insertScpiCmd(QStringList() << "root", &obj);
    cSCPIObject* foundObj = interface.getSCPIObject(QString("root:foo?"), "");
    QVERIFY(foundObj);
}
