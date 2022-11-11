#ifndef TEST_XMLDOCUMENT_H
#define TEST_XMLDOCUMENT_H

#include <QObject>

class test_xmldocument : public QObject
{
    Q_OBJECT
private slots:
    void loadInvalid();
    void loadValid();
    void setDoc();

    void findEmptySearch();
    void findRootNode();
    void noFindRootNode();
    void findOneChildNode();
    void findTwoChildNodes();
    void findTwoChildNodesNested();
    void findTwoChildNodesHasAttributes();
    void identFoundElemByAttribCount();

    void addRootOnEmpty();
    void addExistingRoot();
    void addInvalidRoot();
    void addInvalidNested1();
    //void addInvalidNested2();
    void addSecondRoot();
    void addNested1OnEmpty();
    void addNested2OnEmpty();
};

#endif // TEST_XMLDOCUMENT_H
