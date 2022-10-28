#ifndef TEST_XMLCOMPARER_H
#define TEST_XMLCOMPARER_H

#include <QObject>

class test_xmlcomparer : public QObject
{
    Q_OBJECT
private slots:
    void bothEmpty();
    void bothSameNoAttribs();
    void bothDifferentNoAttribs();
    void bothSameNoAttribsDiffSequence();
    void firstEmptySecondValid();
    void firstValidSecondEmpty();
    void attibsUnequal();
    void attibsEqual();
    void attribsDiffSequence();
};

#endif // TEST_XMLCOMPARER_H
