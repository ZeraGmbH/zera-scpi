#ifndef SCPINODE_H
#define SCPINODE_H

#include "scpiitem.h"
#include "scpiobject.h"

class cSCPINode: public ScpiItem {
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
