#ifndef TEST_SCPIFULLCMDCHECKERFORTEST_H
#define TEST_SCPIFULLCMDCHECKERFORTEST_H

#include <QObject>

class test_scpifullcmdcheckerfortest : public QObject
{
    Q_OBJECT
private slots:
    void noMatchOnEmpty();
    void checkCmdOnQuery();
    void matchQueryNoParams();
    void matchQueryOneParam();
    void matchQueryTwoParam();
    void noMatchQueryTypo();

    void checkQueryOnCmd();
    void checkCmdWithParamOnCmdNoParam();
    void matchCmdNoParams();

    void checkCmsWithParamNoParam();
    void matchCmdOneParam();

    void matchQueryCmdNoParam();
    void matchQueryCmdParam();
};

#endif // TEST_SCPIFULLCMDCHECKERFORTEST_H
