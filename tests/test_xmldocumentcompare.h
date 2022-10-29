#ifndef TEST_XMLDOCUMENTCOMPARE_H
#define TEST_XMLDOCUMENTCOMPARE_H

#include <QObject>

class test_xmldocumentcompare : public QObject
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
    void textUnequal();
    void textEqual();
    void attibsUnequalTextEqual();
    void attibsEqualTextUnequal();
    void attibsEqualTextEqual();
    void docTypeUnequal();
    void docTypeEqual();
};

#endif // TEST_XMLDOCUMENTCOMPARE_H
