#ifndef TEST_XMLATTRIBINFO_H
#define TEST_XMLATTRIBINFO_H

#include <QObject>

class test_xmlattribinfo : public QObject
{
    Q_OBJECT
private slots:
    void compareBothEmpty();
    void compareElemEmpty();
    void compareAttribsEmpty();
    void compareOneUnequal();
    void compareOne();
    void compareNoDoc();
};

#endif // TEST_XMLATTRIBINFO_H
