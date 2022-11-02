#ifndef TEST_XMLELEMITERATOR_H
#define TEST_XMLELEMITERATOR_H

#include <QObject>

class test_xmlelemiterator : public QObject
{
    Q_OBJECT
private slots:
    void noDocNoParent();
    void noParentNotInDoc();
    void root();
    void childOfRoot();
    void grandChildOfRoot();

    void beginEmpty();
    void beginRoot();
    void endEmpty();
    void endValid();

    void iterateOne();
    void iterateTwoNested();
    void iterateTwoParallel();
    void iterateTwoOneGrand();
    void iterateTwoOneGGrand();
    void iterateTwoOneGGGrand();
    void iterateTwoOneGGGrandMulNeighbors();

};

#endif // TEST_XMLELEMITERATOR_H
