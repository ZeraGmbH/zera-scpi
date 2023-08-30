#ifndef CONTEXT_H
#define CONTEXT_H


#include <vector>
#include "variable.h"


class Context
{
public:
    ~Context();
    bool addVar(Variable *var);
    Variable* getVar(std::string name);
    std::vector<Variable*>& getVars();
    bool varExists(std::string varName);
    void clear();

private:
    std::vector<Variable*> m_vars;
};


#endif // CONTEXT_H
