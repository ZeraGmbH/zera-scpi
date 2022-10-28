#ifndef TEST_XMLATTRIBCOMPARE_H
#define TEST_XMLATTRIBCOMPARE_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>

class test_xmlattribcompare : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void compareBothEmpty();
    void compareNoDocRemainsEmptyAlthoughSet();
    void compareElemEmpty1();
    void compareElemEmpty2();
    void compareOneUnequal();
    void compareOne();
    void compareTwoElem1MissingAttrib();
    void compareTwoElem2MissingAttrib();
    void compareTwoFirstValUnequal();
    void compareTwoSecondValUnequal();
    void compareTwoDifferentAttribNames();
    void compareTwo();
    void compareSameText();
    void compareDiffText();
private:
    QDomDocument *m_doc;
    QDomElement *m_elem1;
    QDomElement *m_elem2;
};

#endif // TEST_XMLATTRIBCOMPARE_H
