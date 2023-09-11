#include "test_scpishortformcmd.h"
#include "scpinode.h"
#include <QTest>

QTEST_MAIN(test_scpishortformcmd)

void test_scpishortformcmd::init()
{
    QCOMPARE(ScpiNode::getInstanceCount(), 0);
    m_scpiInterface = new cSCPI();
}

void test_scpishortformcmd::cleanup()
{
    delete m_scpiInterface;
    QCOMPARE(ScpiNode::getInstanceCount(), 0);
}

void test_scpishortformcmd::checkNamesForNonNodes()
{
    QList<ScpiNodeInfo> scpiInfos;
    QStringList list1 = QStringList() << "ROOT" << "CHILD1" << "CHILD2" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});

    QStringList list2 = QStringList() << "ROOT" << "CHILD1" << "CHILD2" << "BAR";
    scpiInfos.append({list2, SCPI::isCmd});

    QStringList list3 = QStringList() << "ROOT" << "CHILD1" << "CHILD2" << "BOO";
    scpiInfos.append({list3, SCPI::isCmdwP});
    addScpiObjects(scpiInfos);

    QList<QStringList> scpiList;
    m_scpiInterface->createFullNonNodeNameList(scpiList);
    QCOMPARE(scpiList.count(), 3);
    QVERIFY(scpiList.contains(list1));
    QVERIFY(scpiList.contains(list2));
    QVERIFY(scpiList.contains(list3));
}

void test_scpishortformcmd::checkZeroDoubleShort()
{
    QList<ScpiNodeInfo> scpiInfos;
    QStringList list1 = QStringList() << "ROOT" << "CHILD1" << "CHILD2" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});

    QStringList list2 = QStringList() << "ROOT" << "CHILD1" << "CHILD2" << "BAR";
    scpiInfos.append({list2, SCPI::isCmd});

    QStringList list3 = QStringList() << "ROOT" << "CHILD1" << "CHILD2" << "BOO";
    scpiInfos.append({list3, SCPI::isCmdwP});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 0);
}

void test_scpishortformcmd::checkOneDoubleShort()
{
    QList<ScpiNodeInfo> scpiInfos;
    QStringList list1 = QStringList() << "ROOT" << "CHILD1" << "CHILD1" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});

    QStringList list2 = QStringList() << "ROOT" << "CHILD2" << "CHILD2" << "BAR";
    scpiInfos.append({list2, SCPI::isCmd});

    QStringList list3 = QStringList() << "ROOT" << "CHILD3" << "CHILD3" << "FOO";
    scpiInfos.append({list3, SCPI::isCmdwP});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 1);
    QCOMPARE(errorList[0], "ROOT:CHILD3:CHILD3:FOO / ROOT:CHIL:CHIL:FOO");
}

void test_scpishortformcmd::checkTwoDoubleShort()
{
    QList<ScpiNodeInfo> scpiInfos;
    QStringList list1 = QStringList() << "ROOT" << "CHILD1" << "CHILD1" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});

    QStringList list2 = QStringList() << "ROOT" << "CHILD2" << "CHILD2" << "FOO";
    scpiInfos.append({list2, SCPI::isCmd});

    QStringList list3 = QStringList() << "ROOT" << "CHILD3" << "CHILD3" << "FOO";
    scpiInfos.append({list3, SCPI::isCmdwP});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 2);
    QVERIFY(errorList.contains("ROOT:CHILD2:CHILD2:FOO / ROOT:CHIL:CHIL:FOO"));
    QVERIFY(errorList.contains("ROOT:CHILD3:CHILD3:FOO / ROOT:CHIL:CHIL:FOO"));
}

void test_scpishortformcmd::addScpiObjects(QList<ScpiNodeInfo> scpiNodes)
{
    for(const auto &scpiNode : scpiNodes) {
        QStringList nodePath = scpiNode.nodePath;
        SCPITestObjectStub* tmpScpiObject = new SCPITestObjectStub(nodePath.takeLast(), scpiNode.type);
        m_scpiInterface->insertScpiCmd(nodePath, tmpScpiObject);
    }
}

