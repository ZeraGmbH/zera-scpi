#include "context.h"


bool Context::addVar(Variable *var)
{
    // Checken ob Variable bereits existiert.
    if (!getVar(var->getName())) {
        m_vars.push_back(var);
        return true;
    }
    else {
        return false;
    }
}

Variable* Context::getVar(QString name)
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

bool Context::varExists(QString varName)
{
    return (getVar(varName) != nullptr);
}

void Context::addConst(Variable *var)
{
    m_consts.push_back(var);
}
