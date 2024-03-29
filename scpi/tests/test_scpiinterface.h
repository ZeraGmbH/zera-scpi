#ifndef TEST_SCPIINTERFACE_H
#define TEST_SCPIINTERFACE_H

#include <QObject>

class test_scpiinterface : public QObject
{
    Q_OBJECT
private slots:
    void mostSimpleAddFindAndLearnBehaviour();
    void findEmpty();
    void addRoot();
    void addFindTwoRoot();
    void addFindTwoNestedSamePath();
    void addFindTwoIdenticalSecondOverwrites();
    void addFindTwoNestedDiffPath();
    void addFindTwoNestedAlmostDiffPath();
    void addFindCaseInsensitive1();
    void addFindCaseInsensitive2();
    void addFindExactShortLong();
    void addFindExactShortLongVowelA();
    void addFindExactShortLongVowelE();
    void addFindExactShortLongVowelI();
    void addFindExactShortLongVowelO();
    void addFindExactShortLongVowelU();
    void addFindTwoWithSameShortButDifferentLong();
    void emptyParentNodeCorrection();
    void emptyParentNodeCorrectionMultiple();
};

#endif // TEST_SCPIINTERFACE_H
