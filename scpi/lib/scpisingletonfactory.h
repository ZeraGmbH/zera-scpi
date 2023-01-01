#ifndef SCPISINGLETONFACTORY_H
#define SCPISINGLETONFACTORY_H

#include <scpi.h>
#include <QString>
#include <memory>
#include <unordered_map>

class ScpiSingletonFactory
{
public:
    static cSCPI* getScpiObj(QString name);
private:
    typedef std::unique_ptr<cSCPI> scpiPtr;
    static std::unordered_map<QString, scpiPtr> m_scpiHash;
};

#endif // SCPISINGLETONFACTORY_H
