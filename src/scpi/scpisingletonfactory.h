#ifndef SCPISINGLETONFACTORY_H
#define SCPISINGLETONFACTORY_H

#include <scpi.h>
#include <QHash>
#include <QString>
#include <QSharedPointer>

class ScpiSingletonFactory
{
public:
    typedef QSharedPointer<cSCPI> scpiPtr;
    static scpiPtr getScpiObj(QString name);
private:
    static QHash<QString, scpiPtr> m_scpiHash;
};

#endif // SCPISINGLETONFACTORY_H
