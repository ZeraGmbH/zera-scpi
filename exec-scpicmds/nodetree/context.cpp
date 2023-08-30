#include "context.h"


Context::~Context()
{
    clear();
}

bool Context::addVar(Variable *var)
{
    // Check if variable already exists (but ignore const variables (they have name == ""))
    std::string name = var->getName();
    if (name == "" || !varExists(name)) {
        m_vars.push_back(var);
        return true;
    }
    return false;
}

Variable* Context::getVar(std::string name)
{
    for (auto var: m_vars) {
        if (var->getName() == name)
            return var;
    }
    return nullptr;
}

std::vector<Variable*>& Context::getVars()
{
    return m_vars;
}

bool Context::varExists(std::string varName)
{
    return (getVar(varName) != nullptr);
}

void Context::clear()
{
    for (auto var : m_vars)
        delete var;
    m_vars.clear();
}
