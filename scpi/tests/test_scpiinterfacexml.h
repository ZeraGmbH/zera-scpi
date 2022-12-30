#ifndef TEST_SCPIINTERFACEXML_H
#define TEST_SCPIINTERFACEXML_H

#include <QObject>
#include <QStringList>
#include "scpi.h"
#include "scpiobject.h"
#include "scpitestobjectstub.h"

class test_scpiinterfacexml : public QObject
{
    Q_OBJECT
private slots:
    void addEmptyRoot();
    void addEmptyRootNested();

    void oneQuery();
    void oneCmd();
    void oneCmdwP();
    void oneXMLCmd();
    void oneElementNested();
    void twoElementNestedDifferentPath();
    void twoElementNestedSamePath();
    void twoElementNestedAlmostSamePath();

    void oneElementNestedRemove();
    void oneElementNestedRemoveReAdd();
    void twoElementNestedSamePathRemoveFirst();
    void twoElementNestedSamePathRemoveFirstUpperCase();
    void twoElementNestedSamePathRemoveNonExistent();
    void twoElementNestedSamePathRemoveParent();
    void twoElementNestedSamePathRemoveGrandParent();
    void twoElementNestedRemoveHalfUp();
    void threeElementAddRemoveFirstThirdWhichIsSecondAfterFirstDelete();

    void twoDifferentCase();

    void iteminstanceCountInit();

    void init();
    void cleanup();
private:
    struct ScpiNodeInfo
    {
        QStringList nodePath;
        quint8 type;
    };

    void addScpiObjects(QList<ScpiNodeInfo> scpiNodes);
    QString createScpiString();

    QList<SCPITestObjectStub*> m_perTestScpiObjects;
    cSCPI *m_scpiInterface;
};

#endif // TEST_SCPIINTERFACEXML_H
