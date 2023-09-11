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
    QStringList list1 = QStringList() << "CHILD1" << "CHILD2" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});

    QStringList list2 = QStringList() << "CHILD1" << "CHILD2" << "BAR";
    scpiInfos.append({list2, SCPI::isCmd});

    QStringList list3 = QStringList() << "CHILD1" << "CHILD2" << "BOO";
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

    QStringList list1 = QStringList() << "CHILD1" << "CHILD2" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});
    QStringList list2 = QStringList() << "CHILD1" << "CHILD2" << "BAR";
    scpiInfos.append({list2, SCPI::isCmd});
    QStringList list3 = QStringList() << "CHILD1" << "CHILD2" << "BOO";
    scpiInfos.append({list3, SCPI::isCmdwP});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 0);

    cSCPIObject *scpiObject1 = m_scpiInterface->getSCPIObject("chil:chil:foo");
    QCOMPARE(scpiObject1->getType(), SCPI::isQuery);
    cSCPIObject *scpiObject2 = m_scpiInterface->getSCPIObject("chil:chil:bar");
    QCOMPARE(scpiObject2->getType(), SCPI::isCmd);
    cSCPIObject *scpiObject3 = m_scpiInterface->getSCPIObject("chil:chil:boo");
    QCOMPARE(scpiObject3->getType(), SCPI::isCmdwP);
}

void test_scpishortformcmd::checkOneDoubleShort()
{
    QList<ScpiNodeInfo> scpiInfos;

    QStringList list1 = QStringList() << "CHILD1" << "CHILD1" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});
    QStringList list2 = QStringList() << "CHILD2" << "CHILD2" << "BAR";
    scpiInfos.append({list2, SCPI::isCmd});
    QStringList list3 = QStringList() << "CHILD3" << "CHILD3" << "FOO";
    scpiInfos.append({list3, SCPI::isCmdwP});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 1);
    QCOMPARE(errorList[0], "CHILD3:CHILD3:FOO / CHIL:CHIL:FOO");
}

void test_scpishortformcmd::checkTwoDoubleShort()
{
    QList<ScpiNodeInfo> scpiInfos;

    QStringList list1 = QStringList() << "CHILD1" << "CHILD1" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});
    QStringList list2 = QStringList() << "CHILD2" << "CHILD2" << "FOO";
    scpiInfos.append({list2, SCPI::isCmd});
    QStringList list3 = QStringList() << "CHILD3" << "CHILD3" << "FOO";
    scpiInfos.append({list3, SCPI::isCmdwP});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 2);
    QVERIFY(errorList.contains("CHILD2:CHILD2:FOO / CHIL:CHIL:FOO"));
    QVERIFY(errorList.contains("CHILD3:CHILD3:FOO / CHIL:CHIL:FOO"));
}

void test_scpishortformcmd::checkNodeOnOtherPath1()
{
    QList<ScpiNodeInfo> scpiInfos;

    QStringList list1 = QStringList() << "CHILD1" << "CHILD1" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});
    QStringList list2 = QStringList() << "CHILD1" << "FOO";
    scpiInfos.append({list2, SCPI::isCmd});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 0);

    cSCPIObject *scpiObject1 = m_scpiInterface->getSCPIObject("chil:chil:foo");
    QCOMPARE(scpiObject1->getType(), SCPI::isQuery);
    cSCPIObject *scpiObject2 = m_scpiInterface->getSCPIObject("chil:foo");
    QCOMPARE(scpiObject2->getType(), SCPI::isCmd);
}

void test_scpishortformcmd::checkNodeOnOtherPath2()
{
    QList<ScpiNodeInfo> scpiInfos;

    QStringList list1 = QStringList() << "CHILD1" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});
    QStringList list2 = QStringList() << "CHILD1" << "CHILD1" << "FOO";
    scpiInfos.append({list2, SCPI::isCmd});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 0);

    cSCPIObject *scpiObject2 = m_scpiInterface->getSCPIObject("chil:foo");
    QCOMPARE(scpiObject2->getType(), SCPI::isQuery);
    cSCPIObject *scpiObject1 = m_scpiInterface->getSCPIObject("chil:chil:foo");
    QCOMPARE(scpiObject1->getType(), SCPI::isCmd);
}

void test_scpishortformcmd::checkNodeOnOtherPathSameShortName1()
{
    QList<ScpiNodeInfo> scpiInfos;

    QStringList list1 = QStringList() << "CHILD1" << "CHILD2" << "FOO";
    scpiInfos.append({list1, SCPI::isQuery});
    QStringList list2 = QStringList() << "CHILD1" << "CHILD2";
    scpiInfos.append({list2, SCPI::isCmd});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 0);

    cSCPIObject *scpiObject1 = m_scpiInterface->getSCPIObject("chil:chil:foo");
    QCOMPARE(scpiObject1->getType(), SCPI::isQuery);
    cSCPIObject *scpiObject2 = m_scpiInterface->getSCPIObject("chil:chil");
    QCOMPARE(scpiObject2->getType(), SCPI::isCmd);
}

void test_scpishortformcmd::checkNodeOnOtherPathSameShortName2()
{
    QList<ScpiNodeInfo> scpiInfos;

    QStringList list1 = QStringList() << "CHILD1" << "CHILD2";
    scpiInfos.append({list1, SCPI::isQuery});
    QStringList list2 = QStringList() << "CHILD1" << "CHILD2" << "FOO";
    scpiInfos.append({list2, SCPI::isCmd});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 0);

    cSCPIObject *scpiObject1 = m_scpiInterface->getSCPIObject("chil:chil");
    QCOMPARE(scpiObject1->getType(), SCPI::isQuery);
    cSCPIObject *scpiObject2 = m_scpiInterface->getSCPIObject("chil:chil:foo");
    QCOMPARE(scpiObject2->getType(), SCPI::isCmd);
}

void test_scpishortformcmd::checkLastSameShort1()
{
    QList<ScpiNodeInfo> scpiInfos;

    QStringList list1 = QStringList() << "CHILD1" << "READ";
    scpiInfos.append({list1, SCPI::isQuery});
    QStringList list2 = QStringList() << "CHILD1" << "READ1";
    scpiInfos.append({list2, SCPI::isCmd});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 1);
}

void test_scpishortformcmd::checkLastSameShort2()
{
    QList<ScpiNodeInfo> scpiInfos;

    QStringList list1 = QStringList() << "CHILD1" << "READ1";
    scpiInfos.append({list1, SCPI::isQuery});
    QStringList list2 = QStringList() << "CHILD1" << "READ";
    scpiInfos.append({list2, SCPI::isCmd});
    addScpiObjects(scpiInfos);

    QStringList errorList = m_scpiInterface->checkDoubleShortNames();
    QCOMPARE(errorList.count(), 1);
}

void test_scpishortformcmd::addScpiObjects(QList<ScpiNodeInfo> scpiNodes)
{
    for(const auto &scpiNode : scpiNodes) {
        QStringList nodePath = scpiNode.nodePath;
        SCPITestObjectStub* tmpScpiObject = new SCPITestObjectStub(nodePath.takeLast(), scpiNode.type);
        m_scpiInterface->insertScpiCmd(nodePath, tmpScpiObject);
    }
}

