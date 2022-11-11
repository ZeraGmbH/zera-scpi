#include "test_scpiinterfacexml.h"
#include "xmldocumentcompare.h"
#include "scpiitem.h"
#include <QTest>
#include <QStringLiteral>

QTEST_MAIN(test_scpiinterfacexml)

QString xmlLead = "<!DOCTYPE SCPIModel><MODELLIST><DEVICE>dev</DEVICE><MODELS>";
QString xmlTrail = "</MODELS></MODELLIST>";


void test_scpiinterfacexml::oneQuery()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "foo", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<root Type='Model,Node'>"
                "<foo ScpiPath='root:foo' Type='Query'/>"
            "</root>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
    QCOMPARE(ScpiItem::getInstanceCount(), 3);
}

void test_scpiinterfacexml::oneCmd()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "foo", SCPI::isCmd});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<root Type='Model,Node'>"
                "<foo ScpiPath='root:foo' Type='Command'/>"
            "</root>";
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
            "<root Type='Model,Node'>"
                "<foo ScpiPath='root:foo' Type='Command+Par'/>"
            "</root>";
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
            "<root Type='Model,Node'>"
                "<foo ScpiPath='root:foo' Type=''/>"
            "</root>";
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
            "<root Type='Model,Node'>"
                "<child Type='Node'>"
                    "<child1 Type='Node'>"
                        "<foo Type='Query' ScpiPath='root:child:child1:foo'/>"
                    "</child1>"
                "</child>"
            "</root>";
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
            "<root1 Type='Model,Node'>"
                "<child1 Type='Node'>"
                    "<child2 Type='Node'>"
                        "<foo ScpiPath='root1:child1:child2:foo' Type='Query'/>"
                    "</child2>"
                "</child1>"
            "</root1>"
            "<root2 Type='Model,Node'>"
                "<child3 Type='Node'>"
                    "<child4 Type='Node'>"
                        "<bar ScpiPath='root2:child3:child4:bar' Type='Query'/>"
                    "</child4>"
                "</child3>"
            "</root2>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
    QCOMPARE(ScpiItem::getInstanceCount(), 9);
}

void test_scpiinterfacexml::twoElementNestedSamePath()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QString xmlExport = createScpiString();

    QString scpiModelXml =
            "<root1 Type='Model,Node'>"
                "<child1 Type='Node'>"
                    "<child2 Type='Node'>"
                        "<foo ScpiPath='root1:child1:child2:foo' Type='Query'/>"
                        "<bar ScpiPath='root1:child1:child2:bar' Type='Query'/>"
                    "</child2>"
                "</child1>"
            "</root1>";
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
            "<root1 Type='Model,Node'>"
                "<child1 Type='Node'>"
                    "<child2 Type='Node'>"
                        "<foo ScpiPath='root1:child1:child2:foo' Type='Query'/>"
                    "</child2>"
                    "<child3 Type='Node'>"
                        "<bar ScpiPath='root1:child1:child3:bar' Type='Query'/>"
                    "</child3>"
                "</child1>"
            "</root1>";
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
    QCOMPARE(ScpiItem::getInstanceCount(), 1);

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
    m_scpiInterface->delSCPICmds("root:child:child1:foo");

    scpiInfos.clear(); // !!!
    scpiInfos.append({QStringList() << "root1" << "child" << "child1" << "bar", SCPI::isQuery});
    addScpiObjects(scpiInfos);
    QCOMPARE(ScpiItem::getInstanceCount(), 5);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<root1 Type='Model,Node'>"
                "<child Type='Node'>"
                    "<child1 Type='Node'>"
                        "<bar Type='Query' ScpiPath='root1:child:child1:bar'/>"
                    "</child1>"
                "</child>"
            "</root1>";
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
    QCOMPARE(ScpiItem::getInstanceCount(), 5);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<root1 Type='Model,Node'>"
                "<child1 Type='Node'>"
                    "<child2 Type='Node'>"
                        "<bar ScpiPath='root1:child1:child2:bar' Type='Query'/>"
                    "</child2>"
                "</child1>"
            "</root1>";
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
    QCOMPARE(ScpiItem::getInstanceCount(), 6);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<root1 Type='Model,Node'>"
                "<child1 Type='Node'>"
                    "<child2 Type='Node'>"
                        "<foo ScpiPath='root1:child1:child2:foo' Type='Query'/>"
                        "<bar ScpiPath='root1:child1:child2:bar' Type='Query'/>"
                    "</child2>"
                "</child1>"
            "</root1>";
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
    QCOMPARE(ScpiItem::getInstanceCount(), 1);

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
    QCOMPARE(ScpiItem::getInstanceCount(), 1);

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
    QCOMPARE(ScpiItem::getInstanceCount(), 6);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<root1 Type='Model,Node'>"
                "<child1 Type='Node'>"
                    "<child2 Type='Node'>"
                        "<child4 Type='Node'>"
                            "<bar ScpiPath='root1:child1:child2:child4:bar' Type='Query'/>"
                        "</child4>"
                    "</child2>"
                "</child1>"
            "</root1>";
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
    QCOMPARE(ScpiItem::getInstanceCount(), 3);

    QString xmlExport = createScpiString();
    QString scpiModelXml =
            "<root Type='Model,Node'>"
                "<child2 ScpiPath='root:child2' Type='Query'/>"
            "</root>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::iteminstanceCountInit()
{
    QCOMPARE(ScpiItem::getInstanceCount(), 1);
}

void test_scpiinterfacexml::init()
{
    QCOMPARE(ScpiItem::getInstanceCount(), 0);
    m_scpiInterface = new cSCPI("dev");
}

void test_scpiinterfacexml::cleanup()
{
    while(!m_perTestScpiObjects.isEmpty())
        delete m_perTestScpiObjects.takeLast();
    delete m_scpiInterface;
    QCOMPARE(ScpiItem::getInstanceCount(), 0);
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

