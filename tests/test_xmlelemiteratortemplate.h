#ifndef TEST_XMLELEMITERATORTEMPLATE_H
#define TEST_XMLELEMITERATORTEMPLATE_H

#include <QObject>

class test_xmlelemiteratortemplate : public QObject
{
    Q_OBJECT
private slots:
    void noDocNoParent();
    void noParentNotInDoc();
    void root();
    void childOfRoot();
    void grandChildOfRoot();
};

#endif // TEST_XMLELEMITERATORTEMPLATE_H
