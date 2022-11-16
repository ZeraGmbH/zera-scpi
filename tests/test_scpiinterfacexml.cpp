#include "test_scpiinterfacexml.h"
#include "xmldocumentcompare.h"
#include "scpinode.h"
#include <QTest>
#include <QStringLiteral>

QTEST_MAIN(test_scpiinterfacexml)

QString xmlLead = "<!DOCTYPE SCPIModel><MODELLIST><DEVICE>dev</DEVICE><MODELS>";
QString xmlTrail = "</MODELS></MODELLIST>";

void test_scpiinterfacexml::addEmptyRoot()
{
    m_scpiInterface->insertScpiCmd(QStringList() << "root", nullptr);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT Type='Model,Node'>"
            "</ROOT>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;
    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::addEmptyRootNested()
{
    m_scpiInterface->insertScpiCmd(QStringList() << "root" << "child", nullptr);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT Type='Model,Node'>"
                "<CHILD Type='Node'/>"
            "</ROOT>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;
    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::oneQuery()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "foo", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT Type='Model,Node'>"
                "<FOO ScpiPath='ROOT:FOO' Type='Query'/>"
            "</ROOT>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
    QCOMPARE(ScpiNode::getInstanceCount(), 3);
}

void test_scpiinterfacexml::oneCmd()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "foo", SCPI::isCmd});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT Type='Model,Node'>"
                "<FOO ScpiPath='ROOT:FOO' Type='Command'/>"
            "</ROOT>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::oneCmdwP()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "foo", SCPI::isCmdwP});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT Type='Model,Node'>"
                "<FOO ScpiPath='ROOT:FOO' Type='Command+Par'/>"
            "</ROOT>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::oneXMLCmd()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "foo", SCPI::isXMLCmd});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT Type='Model,Node'>"
                "<FOO ScpiPath='ROOT:FOO' Type=''/>"
            "</ROOT>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::oneElementNested()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "child" << "child1" << "foo", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT Type='Model,Node'>"
                "<CHILD Type='Node'>"
                    "<CHILD1 Type='Node'>"
                        "<FOO Type='Query' ScpiPath='ROOT:CHILD:CHILD1:FOO'/>"
                    "</CHILD1>"
                "</CHILD>"
            "</ROOT>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::twoElementNestedDifferentPath()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root2" << "child3" << "child4" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT1 Type='Model,Node'>"
                "<CHILD1 Type='Node'>"
                    "<CHILD2 Type='Node'>"
                        "<FOO ScpiPath='ROOT1:CHILD1:CHILD2:FOO' Type='Query'/>"
                    "</CHILD2>"
                "</CHILD1>"
            "</ROOT1>"
            "<ROOT2 Type='Model,Node'>"
                "<CHILD3 Type='Node'>"
                    "<CHILD4 Type='Node'>"
                        "<BAR ScpiPath='ROOT2:CHILD3:CHILD4:BAR' Type='Query'/>"
                    "</CHILD4>"
                "</CHILD3>"
            "</ROOT2>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
    QCOMPARE(ScpiNode::getInstanceCount(), 9);
}

void test_scpiinterfacexml::twoElementNestedSamePath()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT1 Type='Model,Node'>"
                "<CHILD1 Type='Node'>"
                    "<CHILD2 Type='Node'>"
                        "<FOO ScpiPath='ROOT1:CHILD1:CHILD2:FOO' Type='Query'/>"
                        "<BAR ScpiPath='ROOT1:CHILD1:CHILD2:BAR' Type='Query'/>"
                    "</CHILD2>"
                "</CHILD1>"
            "</ROOT1>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::twoElementNestedAlmostSamePath()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child3" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<ROOT1 Type='Model,Node'>"
                "<CHILD1 Type='Node'>"
                    "<CHILD2 Type='Node'>"
                        "<FOO ScpiPath='ROOT1:CHILD1:CHILD2:FOO' Type='Query'/>"
                    "</CHILD2>"
                    "<CHILD3 Type='Node'>"
                        "<BAR ScpiPath='ROOT1:CHILD1:CHILD3:BAR' Type='Query'/>"
                    "</CHILD3>"
                "</CHILD1>"
            "</ROOT1>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::oneElementNestedRemove()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "child" << "child1" << "foo", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    m_scpiInterface->delSCPICmds("root:child:child1:foo");
    QCOMPARE(ScpiNode::getInstanceCount(), 1);

    QString xmlExport = createScpiString();
    const QString xmlExpected = xmlLead + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::oneElementNestedRemoveReAdd()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "child" << "child1" << "foo", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QCOMPARE(ScpiNode::getInstanceCount(), 5);
    m_scpiInterface->delSCPICmds("root:child:child1:foo");
    QCOMPARE(ScpiNode::getInstanceCount(), 1);

    scpiInfos.clear(); // !!!
    scpiInfos.append({QStringList() << "root1" << "child" << "child1" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QCOMPARE(ScpiNode::getInstanceCount(), 5);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<ROOT1 Type='Model,Node'>"
                "<CHILD Type='Node'>"
                    "<CHILD1 Type='Node'>"
                        "<BAR Type='Query' ScpiPath='ROOT1:CHILD:CHILD1:BAR'/>"
                    "</CHILD1>"
                "</CHILD>"
            "</ROOT1>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::twoElementNestedSamePathRemoveFirst()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    m_scpiInterface->delSCPICmds("root1:child1:child2:foo");
    QCOMPARE(ScpiNode::getInstanceCount(), 5);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<ROOT1 Type='Model,Node'>"
                "<CHILD1 Type='Node'>"
                    "<CHILD2 Type='Node'>"
                        "<BAR ScpiPath='ROOT1:CHILD1:CHILD2:BAR' Type='Query'/>"
                    "</CHILD2>"
                "</CHILD1>"
            "</ROOT1>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::twoElementNestedSamePathRemoveFirstUpperCase()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    m_scpiInterface->delSCPICmds("ROOT1:CHILD1:CHILD2:FOO");
    QCOMPARE(ScpiNode::getInstanceCount(), 5);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<ROOT1 Type='Model,Node'>"
                "<CHILD1 Type='Node'>"
                    "<CHILD2 Type='Node'>"
                        "<BAR ScpiPath='ROOT1:CHILD1:CHILD2:BAR' Type='Query'/>"
                    "</CHILD2>"
                "</CHILD1>"
            "</ROOT1>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::twoElementNestedSamePathRemoveNonExistent()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    m_scpiInterface->delSCPICmds("root1:child1:child2:honk");
    QCOMPARE(ScpiNode::getInstanceCount(), 6);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<ROOT1 Type='Model,Node'>"
                "<CHILD1 Type='Node'>"
                    "<CHILD2 Type='Node'>"
                        "<FOO ScpiPath='ROOT1:CHILD1:CHILD2:FOO' Type='Query'/>"
                        "<BAR ScpiPath='ROOT1:CHILD1:CHILD2:BAR' Type='Query'/>"
                    "</CHILD2>"
                "</CHILD1>"
            "</ROOT1>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::twoElementNestedSamePathRemoveParent()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    m_scpiInterface->delSCPICmds("root1:child1:child2");
    QCOMPARE(ScpiNode::getInstanceCount(), 1);

    QString xmlExport = createScpiString();
    const QString xmlExpected = xmlLead + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::twoElementNestedSamePathRemoveGrandParent()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    m_scpiInterface->delSCPICmds("root1:child1");
    QCOMPARE(ScpiNode::getInstanceCount(), 1);

    QString xmlExport = createScpiString();
    const QString xmlExpected = xmlLead + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::twoElementNestedRemoveHalfUp()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "child3" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "child4" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    m_scpiInterface->delSCPICmds("root1:child1:child2:child3");
    QCOMPARE(ScpiNode::getInstanceCount(), 6);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<ROOT1 Type='Model,Node'>"
                "<CHILD1 Type='Node'>"
                    "<CHILD2 Type='Node'>"
                        "<CHILD4 Type='Node'>"
                            "<BAR ScpiPath='ROOT1:CHILD1:CHILD2:CHILD4:BAR' Type='Query'/>"
                        "</CHILD4>"
                    "</CHILD2>"
                "</CHILD1>"
            "</ROOT1>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::threeElementAddRemoveFirstThirdWhichIsSecondAfterFirstDelete()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "child1", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root" << "child2", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root" << "child3", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    m_scpiInterface->delSCPICmds("root:child1");
    m_scpiInterface->delSCPICmds("root:child3");
    QCOMPARE(ScpiNode::getInstanceCount(), 3);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<ROOT Type='Model,Node'>"
                "<CHILD2 ScpiPath='ROOT:CHILD2' Type='Query'/>"
            "</ROOT>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::twoDifferentCase()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "ROOT" << "CHILD", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root" << "child", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QCOMPARE(ScpiNode::getInstanceCount(), 3);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<ROOT Type='Model,Node'>"
                "<CHILD ScpiPath='ROOT:CHILD' Type='Query'/>"
            "</ROOT>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::iteminstanceCountInit()
{
    QCOMPARE(ScpiNode::getInstanceCount(), 1);
}

void test_scpiinterfacexml::init()
{
    QCOMPARE(ScpiNode::getInstanceCount(), 0);
    m_scpiInterface = new cSCPI("dev");
}

void test_scpiinterfacexml::cleanup()
{
    while(!m_perTestScpiObjects.isEmpty())
        delete m_perTestScpiObjects.takeLast();
    delete m_scpiInterface;
    QCOMPARE(ScpiNode::getInstanceCount(), 0);
}

void test_scpiinterfacexml::addScpiObjects(QList<ScpiNodeInfo> scpiNodes)
{
    for(const auto &scpiNode : scpiNodes) {
        QStringList nodePath = scpiNode.nodePath;
        SCPITestObject* tmpScpiObject = new SCPITestObject(nodePath.takeLast(), scpiNode.type);
        m_perTestScpiObjects.append(tmpScpiObject);
        m_scpiInterface->insertScpiCmd(nodePath, tmpScpiObject);
    }
}

QString test_scpiinterfacexml::createScpiString()
{
    QString exportedXml;
    m_scpiInterface->exportSCPIModelXML(exportedXml);
    return exportedXml;
}

// In case more tests are added: To get exported string
//  qWarning("%s", qPrintable(xmlExport.replace("\"", "'")));
// helps

