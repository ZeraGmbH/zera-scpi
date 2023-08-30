#ifndef CONDITION_H
#define CONDITION_H


#include "variable.h"


enum class ComparisonType
{
    LT = 0,
    GT,
    LE,
    GE,
    EQ,
    NE,
};


class ICondition
{
public:
    virtual bool eval() = 0;
    virtual ~ICondition() = default;
};


class BoolCondition : public ICondition
{
public:
    BoolCondition(Variable &value);
    ~BoolCondition();
    bool eval() override ;

private:
    Variable &m_value;
};


class ComparisonCondition : public ICondition
{
public:
    ComparisonCondition(Variable &lValue, Variable &rValue, ComparisonType &type);
    bool eval() override;
    static bool getComparisonTypeFromString(std::string comp, ComparisonType &type);
    static bool comparisonTypeValidForVariableType(ComparisonType compType, VariableType varType);

private:
    Variable &m_lValue;
    Variable &m_rValue;
    ComparisonType m_compType;
};


#endif // CONDITION_H
