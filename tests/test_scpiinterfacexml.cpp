#include "test_scpiinterfacexml.h"
#include "scpi.h"
#include "scpiobject.h"
#include "xmldocumentcompare.h"
#include <QTest>
#include <QStringLiteral>

QTEST_MAIN(test_scpiinterfacexml)

QString xmlLead = "<!DOCTYPE SCPIModel><MODELLIST><DEVICE>dev</DEVICE><MODELS>";
QString xmlTrail = "</MODELS></MODELLIST>";

class SCPITestObject : public cSCPIObject
{
public:
    SCPITestObject(QString name, quint8 type) : cSCPIObject(name, type) { }
    bool executeSCPI(const QString& sInput, QString& sOutput) override
    {
        return true;
    }
};

struct ScpiNodeInfo
{
    QStringList nodePath;
    quint8 type;
};

QString createScpiString(QList<ScpiNodeInfo> scpiNodes)
{
    QList<SCPITestObject*> tmpScpiObjects;
    cSCPI interface("dev");
    for(const auto &scpiNode : scpiNodes) {
        QStringList nodePath = scpiNode.nodePath;
        SCPITestObject* tmpScpiObject = new SCPITestObject(nodePath.takeLast(), scpiNode.type);
        tmpScpiObjects.append(tmpScpiObject);
        interface.insertScpiCmd(nodePath, tmpScpiObject);
    }
    QString exportedXml;
    interface.exportSCPIModelXML(exportedXml);

    while(!tmpScpiObjects.isEmpty())
        delete tmpScpiObjects.takeLast();
    return exportedXml;
}

void test_scpiinterfacexml::oneQuery()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "foo", SCPI::isQuery});
    QString xmlExport = createScpiString(scpiInfos);

    QString scpiModelXml =
            "<root Type='Model,Node'>"
                "<foo ScpiPath='root:foo' Type='Query'/>"
            "</root>";
    const QString xmlExpected = xmlLead + scpiModelXml + xmlTrail;

    XmlDocumentCompare cmp;
    QVERIFY(cmp.compareXml(xmlExport, xmlExpected, true));
}

void test_scpiinterfacexml::oneCmd()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root" << "foo", SCPI::isCmd});
    QString xmlExport = createScpiString(scpiInfos);

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
    QString xmlExport = createScpiString(scpiInfos);

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
    QString xmlExport = createScpiString(scpiInfos);

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
    QString xmlExport = createScpiString(scpiInfos);

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
    QString xmlExport = createScpiString(scpiInfos);

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
}

void test_scpiinterfacexml::twoElementNestedSamePath()
{
    QList<ScpiNodeInfo> scpiInfos;
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "foo", SCPI::isQuery});
    scpiInfos.append({QStringList() << "root1" << "child1" << "child2" << "bar", SCPI::isQuery});
    QString xmlExport = createScpiString(scpiInfos);

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
    QString xmlExport = createScpiString(scpiInfos);

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

// In case more tests are added: To get exported string
//  qWarning("%s", qPrintable(xmlExport.replace("\"", "'")));
// helps

