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
    void testShortParams();
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

    void matchMultipleQueryNoParams();

    void checkOneParamOnQuery();
    void checkTwoParamsOnQuery();
    void checkOneParamOnCmd();
    void checkTwoParamsOnCmd();
};

#endif // TEST_SCPIFULLCMDCHECKERFORTEST_H
