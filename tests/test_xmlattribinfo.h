#ifndef TEST_XMLATTRIBINFO_H
#define TEST_XMLATTRIBINFO_H

#include <QObject>

class test_xmlattribinfo : public QObject
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

#endif // TEST_XMLATTRIBINFO_H
