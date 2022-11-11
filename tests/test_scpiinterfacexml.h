#ifndef TEST_SCPIINTERFACEXML_H
#define TEST_SCPIINTERFACEXML_H

#include <QObject>
#include <QStringList>
#include "scpiobject.h"

class SCPITestObject : public cSCPIObject
{
public:
    SCPITestObject(QString name, quint8 type) : cSCPIObject(name, type) { }
    bool executeSCPI(const QString& sInput, QString& sOutput) override
    {
        return true;
    }
};

class test_scpiinterfacexml : public QObject
{
    Q_OBJECT
private slots:
    void oneQuery();
    void oneCmd();
    void oneCmdwP();
    void oneXMLCmd();
    void oneElementNested();
    void twoElementNestedDifferentPath();
    void twoElementNestedSamePath();
    void twoElementNestedAlmostSamePath();

    void addTwoRootRemoveOne();

    //void init();
    void cleanup();
private:
    struct ScpiNodeInfo
    {
        QStringList nodePath;
        quint8 type;
    };

    QString createScpiString(QList<ScpiNodeInfo> scpiNodes);

    QList<SCPITestObject*> m_perTestScpiObjects;
};

#endif // TEST_SCPIINTERFACEXML_H
