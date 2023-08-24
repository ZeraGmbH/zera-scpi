#ifndef VARIABLE_H
#define VARIABLE_H


#include <QString>


enum VariableType
{
    INT,
    FLOAT,
    BOOL,
    STRING
};


class Variable
{
public:
    Variable(QString name, VariableType type, void *value);
    ~Variable();
    QString getName();
    VariableType getType();
    void *getValue();
    void setValue(void *value);
    void deleteValue();
    QString toString();
    QString toFullString();

private:
    QString m_name;
    VariableType m_type;
    void *m_value = nullptr;
};

#endif // VARIABLE_H
