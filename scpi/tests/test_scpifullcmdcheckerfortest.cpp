#include "test_scpifullcmdcheckerfortest.h"
#include "scpifullcmdcheckerfortest.h"
#include <QTest>

QTEST_MAIN(test_scpifullcmdcheckerfortest)

void test_scpifullcmdcheckerfortest::noMatchOnEmpty()
{
    ScpiFullCmdCheckerForTest checker;
    QCOMPARE(checker.matches("FOO:BAR"), false);
}

void test_scpifullcmdcheckerfortest::checkCmdOnQuery()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand("SENSE:TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:test"), false);
}

void test_scpifullcmdcheckerfortest::matchQueryNoParams()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:test?"), true);
}

void test_scpifullcmdcheckerfortest::testShortParams()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:test?"), true);
}

void test_scpifullcmdcheckerfortest::matchQueryOneParam()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:test? one"), true); // detected as no param !!!
    QCOMPARE(checker.matches("sens:test? one;"), true);
}

void test_scpifullcmdcheckerfortest::matchQueryTwoParam()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:test? one;two"), true); // detected as one param !!!
    QCOMPARE(checker.matches("sens:test? one;two;"), true);
}

void test_scpifullcmdcheckerfortest::noMatchQueryTypo()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:tes?"), false);
}

void test_scpifullcmdcheckerfortest::checkQueryOnCmd()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isCmd);
    QCOMPARE(checker.matches("sens:test?"), false);
}

void test_scpifullcmdcheckerfortest::checkCmdWithParamOnCmdNoParam()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isCmd);
    QCOMPARE(checker.matches("sens:test one;"), false);
    QCOMPARE(checker.matches("sens:test;"), true);
}

void test_scpifullcmdcheckerfortest::matchCmdNoParams()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isCmd);
    QCOMPARE(checker.matches("sens:test"), true);
}

void test_scpifullcmdcheckerfortest::checkCmsWithParamNoParam()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isCmdwP);
    QCOMPARE(checker.matches("sens:test"), false);
    QCOMPARE(checker.matches("sens:test one"), false); // not detected as param!!!
}

void test_scpifullcmdcheckerfortest::matchCmdOneParam()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isCmdwP);
    QCOMPARE(checker.matches("sens:test one;"), true);
}

void test_scpifullcmdcheckerfortest::matchQueryCmdNoParam()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isQuery | SCPI::isCmd);
    QCOMPARE(checker.matches("sens:test"), true);
    QCOMPARE(checker.matches("sens:test one;"), false);
    QCOMPARE(checker.matches("sens:test?"), true);
}

void test_scpifullcmdcheckerfortest::matchQueryCmdParam()
{
    ScpiFullCmdCheckerForTest checker("SENSE:TESTER", SCPI::isQuery | SCPI::isCmdwP);
    QCOMPARE(checker.matches("sens:test one;"), true);
    QCOMPARE(checker.matches("sens:test one;two;"), true);
    QCOMPARE(checker.matches("sens:test"), false);
    QCOMPARE(checker.matches("sens:test?"), true);
}

void test_scpifullcmdcheckerfortest::matchMultipleQueryNoParams()
{
    ScpiFullCmdCheckerForTest checker("SENSE:FOO", SCPI::isQuery);
    checker.addCommand("SENSE:BAR", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:foo?"), true);
    QCOMPARE(checker.matches("sens:bar?"), true);
}

void test_scpifullcmdcheckerfortest::checkOneParamOnQuery()
{
    ScpiFullCmdCheckerForTest checker("SENSE:FOO", SCPI::isQuery, 1);
    QCOMPARE(checker.matches("sens:foo?"), false);
    QCOMPARE(checker.matches("sens:foo? one;"), true);
    QCOMPARE(checker.matches("sens:foo? one;two;"), false);
}

void test_scpifullcmdcheckerfortest::checkTwoParamsOnQuery()
{
    ScpiFullCmdCheckerForTest checker("SENSE:FOO", SCPI::isQuery, 2);
    QCOMPARE(checker.matches("sens:foo?"), false);
    QCOMPARE(checker.matches("sens:foo? one;"), false);
    QCOMPARE(checker.matches("sens:foo? one;two;"), true);
}

void test_scpifullcmdcheckerfortest::checkOneParamOnCmd()
{
    ScpiFullCmdCheckerForTest checker("SENSE:FOO", SCPI::isCmdwP, 1);
    QCOMPARE(checker.matches("sens:foo"), false);
    QCOMPARE(checker.matches("sens:foo;"), false);
    QCOMPARE(checker.matches("sens:foo one;"), true);
    QCOMPARE(checker.matches("sens:foo one;two;"), false);
}

void test_scpifullcmdcheckerfortest::checkTwoParamsOnCmd()
{
    ScpiFullCmdCheckerForTest checker("SENSE:FOO", SCPI::isCmdwP, 2);
    QCOMPARE(checker.matches("sens:foo"), false);
    QCOMPARE(checker.matches("sens:foo;"), false); // detected as empty param!!!
    QCOMPARE(checker.matches("sens:foo one;"), false);
    QCOMPARE(checker.matches("sens:foo one;two;"), true);
}
