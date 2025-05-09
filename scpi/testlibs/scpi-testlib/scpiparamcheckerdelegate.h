#ifndef SCPIPARAMCHECKERDELEGATE_H
#define SCPIPARAMCHECKERDELEGATE_H

#include "scpiobject.h"

class ScpiParamCheckerDelegate : public ScpiObject
{
public:
    ScpiParamCheckerDelegate(const QString &name, quint8 type, int paramCountExpected);
    bool executeSCPI(const QString& input, QString& output) override;
private:
    int m_paramCountExpected;
};

#endif // SCPIPARAMCHECKERDELEGATE_H
