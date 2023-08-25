#include "variable.h"


Variable::Variable(std::string name, VariableType type, void *value)
    : m_name(name), m_type(type), m_value(value)
{
}

Variable::~Variable()
{
    deleteValue();
}

std::string Variable::getName() {
    return m_name;
}

VariableType Variable::getType()
{
    return m_type;
}

void *Variable::getValue()
{
    return this->m_value;
}

void Variable::setValue(void *value)
{
    deleteValue(); // delete old value
    this->m_value = value;
}

void Variable::deleteValue()
{
    switch (m_type) {
    case INT:
        delete (int*)m_value;
        break;
    case FLOAT:
        delete (float*)m_value;
        break;
    case BOOL:
        delete (bool*)m_value;
        break;
    case STRING:
        delete (std::string*)m_value;
        break;
    }
}

std::string Variable::toString()
{
    switch (m_type) {
    case INT:
        return std::to_string(*((int*)m_value));
    case FLOAT:
        return std::to_string(*((float*)m_value));
    case BOOL:
        return std::string((*((bool*)m_value)) ? "TRUE" : "FALSE");
    case STRING:
        return *((std::string*)m_value);
    }
}

std::string Variable::toFullString()
{
    switch (m_type) {
    case INT:
        return m_name + "(INT)" + std::to_string(*((int*)m_value));
    case FLOAT:
        return m_name + "(FLOAT)" + std::to_string(*((float*)m_value));
    case BOOL:
        return m_name + "(BOOL)" + (*((bool*)m_value) ? "TRUE" : "FALSE");
    case STRING:
        return m_name + "(STRING)" + *((std::string*)m_value);
    }
}
