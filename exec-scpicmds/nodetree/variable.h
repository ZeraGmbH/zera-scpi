#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>


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
    Variable(std::string name, VariableType type, void *value);
    ~Variable();
    std::string getName();
    VariableType getType();
    void *getValue();
    void setValue(void *value);
    void deleteValue();
    std::string toString();
    std::string toFullString();
    static bool strIsKeyword(std::string keyword);
    static bool strToVarType(std::string str, VariableType &type);
    static Variable* parseToVar(std::string name, std::string token, VariableType type);
    static bool varNameIsValid(std::string name);

private:
    std::string m_name;
    VariableType m_type;
    void *m_value = nullptr;
};

#endif // VARIABLE_H
