#include "scpisingletonfactory.h"

std::unordered_map<QString, ScpiSingletonFactory::scpiPtr> ScpiSingletonFactory::m_scpiHash;

cSCPI* ScpiSingletonFactory::getScpiObj(QString name)
{
    scpiPtr &entry = m_scpiHash[name];
    if(!entry) {
        entry = std::make_unique<cSCPI>(name);
    }
    return entry.get();
}
