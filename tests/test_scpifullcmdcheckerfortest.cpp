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
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:test"), false);
}

void test_scpifullcmdcheckerfortest::matchQueryNoParams()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:test?"), true);
}

void test_scpifullcmdcheckerfortest::matchQueryOneParam()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:test? one"), true);
    QCOMPARE(checker.matches("sens:test? one;"), true);
}

void test_scpifullcmdcheckerfortest::matchQueryTwoParam()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:test? one;two"), true);
    QCOMPARE(checker.matches("sens:test? one;two;"), true);
}

void test_scpifullcmdcheckerfortest::noMatchQueryTypo()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isQuery);
    QCOMPARE(checker.matches("sens:tes?"), false);
}

void test_scpifullcmdcheckerfortest::checkQueryOnCmd()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isCmd);
    QCOMPARE(checker.matches("sens:test?"), false);
}

void test_scpifullcmdcheckerfortest::checkCmdWithParamOnCmdNoParam()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isCmd);
    QCOMPARE(checker.matches("sens:test one;"), false);
    QCOMPARE(checker.matches("sens:test;"), false); // detected as empty param!!!
}

void test_scpifullcmdcheckerfortest::matchCmdNoParams()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isCmd);
    QCOMPARE(checker.matches("sens:test"), true);
}

void test_scpifullcmdcheckerfortest::checkCmsWithParamNoParam()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isCmdwP);
    QCOMPARE(checker.matches("sens:test"), false);
    QCOMPARE(checker.matches("sens:test one"), false); // not detected as param!!!
}

void test_scpifullcmdcheckerfortest::matchCmdOneParam()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isCmdwP);
    QCOMPARE(checker.matches("sens:test one;"), true);
}

void test_scpifullcmdcheckerfortest::matchQueryCmdNoParam()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isQuery | SCPI::isCmd);
    QCOMPARE(checker.matches("sens:test"), true);
    QCOMPARE(checker.matches("sens:test one;"), false);
    QCOMPARE(checker.matches("sens:test?"), true);
}

void test_scpifullcmdcheckerfortest::matchQueryCmdParam()
{
    ScpiFullCmdCheckerForTest checker;
    checker.addCommand(QStringList("SENSE"), "TESTER", SCPI::isQuery | SCPI::isCmdwP);
    QCOMPARE(checker.matches("sens:test one;"), true);
    QCOMPARE(checker.matches("sens:test one;two;"), true);
    QCOMPARE(checker.matches("sens:test"), false);
    QCOMPARE(checker.matches("sens:test?"), true);
}

