#include "context.h"


bool Context::addVar(Variable *var)
{
    // Check if variable already exists
    if (!getVar(var->getName())) {
        m_vars.push_back(var);
        return true;
    }
    else {
        return false;
    }
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

void Context::addConst(Variable *var)
{
    m_consts.push_back(var);
}
