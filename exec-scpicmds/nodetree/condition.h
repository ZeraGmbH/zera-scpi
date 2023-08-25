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


class Condition
{
public:
    virtual bool eval() = 0;
    virtual ~Condition() = default;
};


class BoolCondition : public Condition
{
public:
    BoolCondition(Variable &value);
    bool eval() override ;

private:
    Variable &m_value;
};


class ComparisonCondition : public Condition
{
public:
    ComparisonCondition(Variable &lValue, Variable &rValue, ComparisonType &type);
    bool eval() override;
    static bool getComparisonTypeFromString(std::string comp, ComparisonType &type);

private:
    Variable &m_lValue;
    Variable &m_rValue;
    ComparisonType m_compType;
};


#endif // CONDITION_H
