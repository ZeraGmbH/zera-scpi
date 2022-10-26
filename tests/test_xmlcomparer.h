#ifndef TEST_XMLCOMPARER_H
#define TEST_XMLCOMPARER_H

#include <QObject>

class test_xmlcomparer : public QObject
{
    Q_OBJECT
private slots:
    void loadInvalid();
    void loadValid();
    void countRootNode();
    void countOneChildNode();
    void countTwoChildNodes();
    void countTwoChildNodesNested();
    void countTwoChildNodesAttributes();

    void findEmptySearch();
    void findRootNode();
    void noFindRootNode();
    void findOneChildNode();
    void findTwoChildNodes();
    void findTwoChildNodesNested();
    void findTwoChildNodesHasAttributes();

    void identFoundElemByAttribCount();
};

#endif // TEST_XMLCOMPARER_H
