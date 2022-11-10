#ifndef TEST_XMLELEMITERATORSORT_H
#define TEST_XMLELEMITERATORSORT_H

#include <QObject>

class test_xmlelemiteratorsort : public QObject
{
    Q_OBJECT
private slots:
    void noArrayOneNested();
    void noArraySortTwo();
    void noArraySortTwoNested();
    void noArraySortTwoNestedTwo();
};

#endif // TEST_XMLELEMITERATORSORT_H
