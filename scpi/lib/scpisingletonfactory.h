#ifndef SCPISINGLETONFACTORY_H
#define SCPISINGLETONFACTORY_H

#include <scpi.h>
#include <QString>
#include <memory>
#include <unordered_map>

class ScpiSingletonFactory
{
public:
    static cSCPI* getScpiObj();
private:
    static cSCPI* m_instance;
};

#endif // SCPISINGLETONFACTORY_H
