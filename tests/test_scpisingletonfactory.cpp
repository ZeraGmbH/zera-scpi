#include "test_scpisingletonfactory.h"
#include <scpisingletonfactory.h>

QTEST_MAIN(test_scpisingletonfactory)

void test_scpisingletonfactory::askOneOnce()
{
   QVERIFY(ScpiSingletonFactory::getScpiObj("foo"));
}

void test_scpisingletonfactory::askOneTwice()
{
    ScpiSingletonFactory::scpiPtr ptr1 = ScpiSingletonFactory::getScpiObj("foo");
    ScpiSingletonFactory::scpiPtr ptr2 = ScpiSingletonFactory::getScpiObj("foo");
    QCOMPARE(ptr1, ptr2);
}

void test_scpisingletonfactory::askTwoOnce()
{
    ScpiSingletonFactory::scpiPtr foo = ScpiSingletonFactory::getScpiObj("foo");
    ScpiSingletonFactory::scpiPtr bar = ScpiSingletonFactory::getScpiObj("bar");
    QVERIFY(foo);
    QVERIFY(bar);
    QVERIFY(foo != bar);
}

void test_scpisingletonfactory::askTwoTwice()
{
    ScpiSingletonFactory::scpiPtr foo1 = ScpiSingletonFactory::getScpiObj("foo");
    ScpiSingletonFactory::scpiPtr foo2 = ScpiSingletonFactory::getScpiObj("foo");
    ScpiSingletonFactory::scpiPtr bar1 = ScpiSingletonFactory::getScpiObj("bar");
    ScpiSingletonFactory::scpiPtr bar2 = ScpiSingletonFactory::getScpiObj("bar");
    QVERIFY(foo1);
    QCOMPARE(foo1, foo2);
    QVERIFY(bar1);
    QCOMPARE(bar1, bar2);
    QVERIFY(foo1 != bar1);
    QVERIFY(foo2 != bar2);
}
