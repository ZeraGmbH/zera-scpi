#include "test_scpisingletonfactory.h"
#include <scpisingletonfactory.h>

QTEST_MAIN(test_scpisingletonfactory)

void test_scpisingletonfactory::askOneOnce()
{
   QVERIFY(ScpiSingletonFactory::getScpiObj());
}

void test_scpisingletonfactory::askOneTwice()
{
    cSCPI* ptr1 = ScpiSingletonFactory::getScpiObj();
    cSCPI* ptr2 = ScpiSingletonFactory::getScpiObj();
    QCOMPARE(ptr1, ptr2);
}
