#ifndef TEST_SCPISHORTFORMCMD_H
#define TEST_SCPISHORTFORMCMD_H

#include <QObject>
#include "scpi.h"
#include "scpiobject.h"
#include "scpitestobjectstub.h"

class test_scpishortformcmd : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void checkNamesForNonNodes();
    void checkZeroDoubleShort();
    void checkOneDoubleShort();
    void checkTwoDoubleShort();

    void checkNodeOnOtherPath1();
    void checkNodeOnOtherPath2();
    void checkNodeOnOtherPathSameShortName1();
    void checkNodeOnOtherPathSameShortName2();

    void checkLastSameShort1();
    void checkLastSameShort2();

private:
    struct ScpiNodeInfo
    {
        QStringList nodePath;
        quint8 type;
    };
    void addScpiObjects(QList<ScpiNodeInfo> scpiNodes);

    cSCPI *m_scpiInterface;
};

#endif // TEST_SCPISHORTFORMCMD_H
