#include "test_scpisingletonfactory.h"
#include <scpisingletonfactory.h>

QTEST_MAIN(test_scpisingletonfactory)

void test_scpisingletonfactory::askOneOnce()
{
   QVERIFY(ScpiSingletonFactory::getScpiObj("foo"));
}

void test_scpisingletonfactory::askOneTwice()
{
    cSCPI* ptr1 = ScpiSingletonFactory::getScpiObj("foo");
    cSCPI* ptr2 = ScpiSingletonFactory::getScpiObj("foo");
    QCOMPARE(ptr1, ptr2);
}

void test_scpisingletonfactory::askTwoOnce()
{
    cSCPI* foo = ScpiSingletonFactory::getScpiObj("foo");
    cSCPI* bar = ScpiSingletonFactory::getScpiObj("bar");
    QVERIFY(foo);
    QVERIFY(bar);
    QVERIFY(foo != bar);
}

void test_scpisingletonfactory::askTwoTwice()
{
    cSCPI* foo1 = ScpiSingletonFactory::getScpiObj("foo");
    cSCPI* foo2 = ScpiSingletonFactory::getScpiObj("foo");
    cSCPI* bar1 = ScpiSingletonFactory::getScpiObj("bar");
    cSCPI* bar2 = ScpiSingletonFactory::getScpiObj("bar");
    QVERIFY(foo1);
    QCOMPARE(foo1, foo2);
    QVERIFY(bar1);
    QCOMPARE(bar1, bar2);
    QVERIFY(foo1 != bar1);
    QVERIFY(foo2 != bar2);
}
