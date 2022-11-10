#ifndef TEST_XMLELEMITERATORSORT_H
#define TEST_XMLELEMITERATORSORT_H

#include <QObject>

class test_xmlelemiteratorsort : public QObject
{
    Q_OBJECT
private slots:
    void noFreakOutOnEmpty();

    void noArrayOneNested();
    void noArraySortTwo();
    void noArraySortTwoNested();
    void noArraySortTwoNestedTwo();

    void arraySortTwoNestedTwo();
    void arraySortTwoNestedTwoMixNonArray();
    void arraySortTwoNestedTwoMixNonArraySort();
    void arraySortTwoNestedTwoMixNonArrayObscure();
};

#endif // TEST_XMLELEMITERATORSORT_H
