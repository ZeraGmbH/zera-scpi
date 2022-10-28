#ifndef TEST_XMLTEXTCOMPARE_H
#define TEST_XMLTEXTCOMPARE_H

#include <QObject>

class test_xmltextcompare : public QObject
{
    Q_OBJECT
private slots:
    void bothEmpty();
    void oneEmpty();
    void equal();
    void unequal();
    void firstHasAttributes();
    void secondHasAttibutes();
};

#endif // TEST_XMLTEXTCOMPARE_H
