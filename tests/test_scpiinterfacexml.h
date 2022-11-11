#ifndef TEST_SCPIINTERFACEXML_H
#define TEST_SCPIINTERFACEXML_H

#include <QObject>

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
};

#endif // TEST_SCPIINTERFACEXML_H
