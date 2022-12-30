#ifndef TEST_SCPIBENCHMARK_H
#define TEST_SCPIBENCHMARK_H

#include "scpi.h"
#include "scpitestobject.h"
#include <QObject>
#include <QList>

class test_scpibenchmark : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void listInsert();
    void listSearch();
    void listRemove();
private:
    void listInsertLayer(int layer, QStringList parentPath);
    void listSearchLayer(int layer, QStringList parentPath);
    void listRemoveLayer(int layer, QStringList parentPath);
    QList<SCPITestObject*> m_perTestScpiObjects;
    cSCPI *m_scpiInterface;
    int m_listObjectsAdded = 0;
};

#endif // TEST_SCPIBENCHMARK_H
