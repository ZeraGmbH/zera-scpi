#include "scpisingletonfactory.h"

QHash<QString, ScpiSingletonFactory::scpiPtr> ScpiSingletonFactory::m_scpiHash;

cSCPI* ScpiSingletonFactory::getScpiObj(QString name)
{
    scpiPtr &entry = m_scpiHash[name];
    if(!entry) {
        entry = scpiPtr::create(name);
    }
    return entry.data();
}
