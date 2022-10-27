#ifndef TEST_XMLATTRIBCOMPARE_H
#define TEST_XMLATTRIBCOMPARE_H

#include <QObject>

class test_xmlattribcompare : public QObject
{
    Q_OBJECT
private slots:
    void compareBothEmpty();
    void compareNoDoc();
    void compareElemEmpty();
    void compareAttribsEmpty();
    void compareOneUnequal();
    void compareOne();
    void compareTwoElemMissing();
    void compareTwoAttribMissing();
    void compareTwoFirstValUnequal();
    void compareTwoSecondValUnequal();
    void compareTwoDifferentAttribNames();
    void compareTwo();
};

#endif // TEST_XMLATTRIBCOMPARE_H
