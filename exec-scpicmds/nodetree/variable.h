#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>


enum class VariableType
{
    INT,
    FLOAT,
    BOOL,
    STRING,
};


class Variable
{
public:
    Variable(std::string name, VariableType type);
    virtual ~Variable() = default;
    std::string getName();
    VariableType getType();
    std::string getTypeString();
    virtual std::string toString() = 0;
    virtual std::string toFullString() = 0;
    static bool strIsKeyword(std::string keyword);
    static bool strToVarType(std::string str, VariableType &type);
    static Variable* parseToVar(std::string name, std::string token, VariableType type);
    static bool varNameIsValid(std::string name);
    static std::string typeToString(VariableType type);

protected:
    std::string m_name;
    VariableType m_type;
};


class IntVariable : public Variable
{
public:
    IntVariable(std::string name, int value) : Variable(name, VariableType::INT), m_value(value) {};
    int getValue() { return m_value; };
    void setValue(int value) { m_value = value; };
    std::string toString() override { return std::to_string(m_value); };
    std::string toFullString() override { return m_name + "(" + getTypeString() + ")" + std::to_string(m_value); };

protected:
    int m_value;
};


class FloatVariable : public Variable
{
public:
    FloatVariable(std::string name, float value) : Variable(name, VariableType::FLOAT), m_value(value) {};
    float getValue() { return m_value; };
    void setValue(float value) { m_value = value; };
    std::string toString() override { return std::to_string(m_value); };
    std::string toFullString() override { return m_name + "(" + getTypeString() + ")" + std::to_string(m_value); };

protected:
    float m_value;
};


class BoolVariable : public Variable
{
public:
    BoolVariable(std::string name, bool value) : Variable(name, VariableType::BOOL), m_value(value) {};
    bool getValue() { return m_value; };
    void setValue(bool value) { m_value = value; };
    std::string toString() override { return std::to_string(m_value); };
    std::string toFullString() override { return m_name + "(" + getTypeString() + ")" + (m_value ? "TRUE" : "FALSE"); };

protected:
    bool m_value;
};


class StringVariable : public Variable
{
public:
    StringVariable(std::string name, std::string value) : Variable(name, VariableType::STRING), m_value(value) {};
    std::string getValue() { return m_value; };
    void setValue(std::string value) { m_value = value; };
    std::string toString() override { return m_value; };
    std::string toFullString() override { return m_name + "(" + getTypeString() + ")" + m_value; };

protected:
    std::string m_value;
};

#endif // VARIABLE_H
