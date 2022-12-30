#ifndef TEST_XMLELEMITERATORTREE_H
#define TEST_XMLELEMITERATORTREE_H

#include <QObject>

class test_xmlelemiteratortree : public QObject
{
    Q_OBJECT
private slots:
    void beginEmpty();
    void beginRoot();

    void iterateOne();
    void iterateTwoNested();
    void iterateTwoParallel();
    void iterateTwoOneGrand();
    void iterateTwoOneGGrand();
    void iterateTwoOneGGGrand();
    void iterateTwoOneGGGrandMulNeighbors();
    void iterateNestedArray();

};

#endif // TEST_XMLELEMITERATORTREE_H
