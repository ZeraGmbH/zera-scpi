#ifndef TEST_XMLELEMITER_H
#define TEST_XMLELEMITER_H

#include <QObject>

class test_xmlelemiter : public QObject
{
    Q_OBJECT
private slots:
    void noDocNoParent();
    void noParentNotInDoc();
    void root();
    void childOfRoot();
    void grandChildOfRoot();
};

#endif // TEST_XMLELEMITER_H
