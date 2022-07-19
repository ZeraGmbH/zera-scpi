#include "scpisingletonfactory.h"

QHash<QString, ScpiSingletonFactory::scpiPtr> ScpiSingletonFactory::m_scpiHash;

ScpiSingletonFactory::scpiPtr ScpiSingletonFactory::getScpiObj(QString name)
{
    scpiPtr &entry = m_scpiHash[name];
    if(!entry) {
        entry = scpiPtr::create(name);
    }
    return entry;
}
