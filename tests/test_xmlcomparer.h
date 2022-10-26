#ifndef TEST_XMLCOMPARER_H
#define TEST_XMLCOMPARER_H

#include <QObject>

class test_xmlcomparer : public QObject
{
    Q_OBJECT
private slots:
    void loadInvalid();
    void loadValid();
    void checkRootNode();
    void checkOneChildNode();
    void checkTwoChildNodes();
    void checkTwoChildNodesNested();
};

#endif // TEST_XMLCOMPARER_H
