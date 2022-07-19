#ifndef SCPISINGLETONFACTORY_H
#define SCPISINGLETONFACTORY_H

#include <scpi.h>
#include <QHash>
#include <QString>
#include <QSharedPointer>

class ScpiSingletonFactory
{
public:
    static cSCPI* getScpiObj(QString name);
private:
    typedef QSharedPointer<cSCPI> scpiPtr;
    static QHash<QString, scpiPtr> m_scpiHash;
};

#endif // SCPISINGLETONFACTORY_H
