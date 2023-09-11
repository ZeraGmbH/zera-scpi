#include "test_scpishortformcmd.h"
#include "scpinode.h"
#include <QTest>

QTEST_MAIN(test_scpishortformcmd)

void test_scpishortformcmd::init()
{
    QCOMPARE(ScpiNode::getInstanceCount(), 0);
    m_scpiInterface = new cSCPI();
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
    QList<QStringList> scpiShortList;
    m_scpiInterface->createFullNameList(scpiShortList);
    QCOMPARE(scpiShortList.count(), 3);
    QVERIFY(scpiShortList.contains(list1));
    QVERIFY(scpiShortList.contains(list2));
    QVERIFY(scpiShortList.contains(list3));
}

void test_scpishortformcmd::addScpiObjects(QList<ScpiNodeInfo> scpiNodes)
{
    for(const auto &scpiNode : scpiNodes) {
        QStringList nodePath = scpiNode.nodePath;
        SCPITestObjectStub* tmpScpiObject = new SCPITestObjectStub(nodePath.takeLast(), scpiNode.type);
        m_scpiInterface->insertScpiCmd(nodePath, tmpScpiObject);
    }
}

