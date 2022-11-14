#ifndef TEST_SCPIINTERFACE_H
#define TEST_SCPIINTERFACE_H

#include <QObject>

class test_scpiinterface : public QObject
{
    Q_OBJECT
private slots:
    void mostSimpleAddFindAndLearnBehaviour();
    void addRoot();
    void addFindTwoRoot();
    void addFindTwoNestedSamePath();
    void addFindTwoNestedIdentical();
    void addFindTwoNestedDiffPath();
    void addFindTwoNestedAlmostDiffPath();
};

#endif // TEST_SCPIINTERFACE_H
