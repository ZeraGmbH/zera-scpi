#ifndef TEST_XMLDOCUMENT_H
#define TEST_XMLDOCUMENT_H

#include <QObject>

class test_xmldocument : public QObject
{
    Q_OBJECT
private slots:
    void loadInvalid();
    void loadValid();

    void findEmptySearch();
    void findRootNode();
    void noFindRootNode();
    void findOneChildNode();
    void findTwoChildNodes();
    void findTwoChildNodesNested();
    void findTwoChildNodesHasAttributes();

    void identFoundElemByAttribCount();
};

#endif // TEST_XMLDOCUMENT_H
