#ifndef TEST_XMLELEMITERATORLIST_H
#define TEST_XMLELEMITERATORLIST_H

#include <QObject>

class test_xmlelemiteratorlist : public QObject
{
    Q_OBJECT
private slots:
    void nextForNullElem();

    void findEmptyDoc();
    void findRoot();
    void findRootHasOne();
    void findOneOnTwoDiffChildren();
    void findOneOnTwoEqualChildren();
};

#endif // TEST_XMLELEMITERATORLIST_H
