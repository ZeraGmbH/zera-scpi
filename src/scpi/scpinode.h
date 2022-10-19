#ifndef SCPINODE_H
#define SCPINODE_H

#include <QStandardItem>
#include "scpiobject.h"

class cSCPINode: public QStandardItem {
public:
    cSCPIObject* m_pSCPIObject;

    cSCPINode(const QString& sNodeName, quint8 t, cSCPIObject* pSCPIObject);
    quint8 getType();
    void setType(quint8 type);
    const QString &getName();

private:
    QString m_sNodeName;
    quint8 m_nType;

};

#endif // SCPINODE_H
