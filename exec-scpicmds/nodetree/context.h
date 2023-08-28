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
    void addConst(Variable *var);

private:
    std::vector<Variable*> m_vars;
    std::vector<Variable*> m_consts; // just to hold references, even if their value remains constandt
};


#endif // CONTEXT_H
