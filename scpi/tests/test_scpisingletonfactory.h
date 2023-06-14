#ifndef TEST_SINGLETONFACTORY
#define TEST_SINGLETONFACTORY

#include <QTest>

class test_scpisingletonfactory : public QObject
{
    Q_OBJECT
private slots:
    void askOneOnce();
    void askOneTwice();
};

#endif
