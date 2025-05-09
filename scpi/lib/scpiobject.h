#ifndef SCPIOBJECT_H
#define SCPIOBJECT_H

#include "scpi_export.h"
#include <QMap>
#include <memory>

class SCPI_EXPORT ScpiObject {
public:
    ScpiObject();
    ScpiObject(const QString &name, quint8 type);
    virtual ~ScpiObject();
    virtual bool executeSCPI(const QString& sInput, QString& sOutput) = 0;
    const QString getName() const;
    quint8 getType() const;

    void setXmlAttribute(const QString &key, const QString &value);
    typedef QMap<QString, QString> XmlKeyValueMap;
    const XmlKeyValueMap &getXmlAttibuteMap();

    static int getInstanceCount();
private:
    QString m_sName;
    quint8 m_nType;
    QString m_description;
    ScpiObject::XmlKeyValueMap m_xmlAtttibuteMap;
    static int m_instanceCount;
};

typedef std::shared_ptr<ScpiObject> ScpiObjectPtr;

#endif // SCPIOBJECT_H
