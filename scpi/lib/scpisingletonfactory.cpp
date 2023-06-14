#include "scpisingletonfactory.h"

cSCPI* ScpiSingletonFactory::m_instance = nullptr;

cSCPI* ScpiSingletonFactory::getScpiObj()
{
    if(!m_instance) {
        m_instance = new cSCPI();
    }
    return m_instance;
}
