#include "test_scpibenchmark.h"
#include "scpinode.h"
#include <QTest>
#include <math.h>

QTEST_MAIN(test_scpibenchmark)

static const int perLayerNodes = 10;
static const int treeDepth = 4;

void test_scpibenchmark::initTestCase()
{
    m_scpiInterface = new cSCPI("dev");
}

void test_scpibenchmark::cleanupTestCase()
{
    while(!m_perTestScpiObjects.isEmpty())
        delete m_perTestScpiObjects.takeLast();
    delete m_scpiInterface;
}

void test_scpibenchmark::listInsertLayer(int layer, QStringList parentPath)
{
    if(layer >= treeDepth)
        return;
    for(int node=0; node<perLayerNodes; node++) {
        QString name = QString("node%1_%2").arg(layer).arg(node);
        if(layer == treeDepth-1) {
            SCPITestObject* tmpScpiObject = new SCPITestObject(name, SCPI::isQuery);
            m_perTestScpiObjects.append(tmpScpiObject);
            m_scpiInterface->insertScpiCmd(parentPath, tmpScpiObject);
            m_listObjectsAdded++;
        }
        listInsertLayer(layer+1, parentPath<<name);
    }
}

void test_scpibenchmark::listSearchLayer(int layer, QStringList parentPath)
{
    if(layer >= treeDepth)
        return;
    for(int node=0; node<perLayerNodes; node++) {
        QString name = QString("node%1_%2").arg(layer).arg(node);
        if(layer == treeDepth-1) {
            QString parentName = parentPath.join(":");
            QVERIFY(m_scpiInterface->getSCPIObject(parentName + + ":" + name));
        }
        listSearchLayer(layer+1, parentPath<<name);
    }
}

void test_scpibenchmark::listRemoveLayer(int layer, QStringList parentPath)
{
    if(layer >= treeDepth)
        return;
    for(int node=0; node<perLayerNodes; node++) {
        QString name = QString("node%1_%2").arg(layer).arg(node);
        if(layer == treeDepth-1) {
            QString parentName = parentPath.join(":");
            m_scpiInterface->delSCPICmds(parentName + + ":" + name);
        }
        listRemoveLayer(layer+1, parentPath<<name);
    }
}

void test_scpibenchmark::listInsert()
{
    QBENCHMARK_ONCE {
        listInsertLayer(0, QStringList());
    }
    qInfo("SCPI nodes created: %i", ScpiNode::getInstanceCount());
    qInfo("SCPI objects added: %i", m_listObjectsAdded);
}

void test_scpibenchmark::listSearch()
{
    QBENCHMARK {
        listSearchLayer(0, QStringList());
    }
}

void test_scpibenchmark::listRemove()
{
    QBENCHMARK_ONCE {
        listRemoveLayer(0, QStringList());
    }
}

