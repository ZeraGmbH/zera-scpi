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

    void iterateOnePre();
    void iterateOnePost();

    void iterateTwoNested();
    void iterateTwoParallel();
    void iterateTwoOneGrand();
    void iterateTwoOneGGrand();
    void iterateTwoOneGGGrand();
    void iterateTwoOneGGGrandMulNeighbours();

};

#endif // TEST_XMLELEMITERATOR_H
