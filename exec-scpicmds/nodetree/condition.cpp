#include "condition.h"


BoolCondition::BoolCondition(BoolVariable &value) :
    m_value(value)
{
}

BoolCondition::~BoolCondition()
{
}

bool BoolCondition::eval()
{
    return m_value.getValue();
}


ComparisonCondition::ComparisonCondition(Variable &lValue, Variable &rValue, ComparisonType &type) :
    m_lValue(lValue), m_rValue(rValue), m_compType(type)
{
}

bool ComparisonCondition::eval()
{
    if (m_lValue.getType() == VariableType::INT) {
        int lVal = dynamic_cast<IntVariable&>(m_lValue).getValue();
        int rVal = dynamic_cast<IntVariable&>(m_rValue).getValue();

        switch (m_compType) {
        case ComparisonType::LT:
            return lVal < rVal;
        case ComparisonType::GT:
            return lVal > rVal;
        case ComparisonType::LE:
            return lVal <= rVal;
        case ComparisonType::GE:
            return lVal >= rVal;
        case ComparisonType::EQ:
            return lVal == rVal;
        case ComparisonType::NE:
            return lVal != rVal;
        }
    }
    else if (m_lValue.getType() == VariableType::FLOAT) {
        float lVal = dynamic_cast<FloatVariable&>(m_lValue).getValue();
        float rVal = dynamic_cast<FloatVariable&>(m_rValue).getValue();

        switch (m_compType) {
        case ComparisonType::LT:
            return lVal < rVal;
        case ComparisonType::GT:
            return lVal > rVal;
        case ComparisonType::LE:
            return lVal <= rVal;
        case ComparisonType::GE:
            return lVal >= rVal;
        case ComparisonType::EQ:
            return lVal == rVal;
        case ComparisonType::NE:
            return lVal != rVal;
        }
    }
    else if (m_lValue.getType() == VariableType::BOOL) {
        bool lVal = dynamic_cast<BoolVariable&>(m_lValue).getValue();
        bool rVal = dynamic_cast<BoolVariable&>(m_rValue).getValue();

        switch (m_compType) {
        case ComparisonType::LT:
        case ComparisonType::GT:
        case ComparisonType::LE:
        case ComparisonType::GE:
            ; // Invalid comparison
        case ComparisonType::EQ:
            return lVal == rVal;
        case ComparisonType::NE:
            return lVal != rVal;
        }
    }
    else if (m_lValue.getType() == VariableType::STRING) {
        std::string lVal = dynamic_cast<StringVariable&>(m_lValue).getValue();
        std::string rVal = dynamic_cast<StringVariable&>(m_rValue).getValue();

        switch (m_compType) {
        case ComparisonType::LT:
        case ComparisonType::GT:
        case ComparisonType::LE:
        case ComparisonType::GE:
            ; // Invalid comparison
        case ComparisonType::EQ:
            return lVal == rVal;
        case ComparisonType::NE:
            return lVal != rVal;
        }
    }

    return false; // Default (should never happen)
}

bool ComparisonCondition::getComparisonTypeFromString(std::string comp, ComparisonType &type)
{
    if (comp == "LT")
        type = ComparisonType::LT;
    else if (comp == "GT")
        type = ComparisonType::GT;
    else if (comp == "LE")
        type = ComparisonType::LE;
    else if (comp == "GE")
        type = ComparisonType::GE;
    else if (comp == "EQ")
        type = ComparisonType::EQ;
    else if (comp == "NE")
        type = ComparisonType::NE;
    else
        return false;

    return true;
}

bool ComparisonCondition::comparisonTypeValidForVariableType(ComparisonType compType, VariableType varType)
{
    bool errorFound = false;
    switch (varType) {
    case VariableType::INT:
        return true; // All comparisons are valid
    case VariableType::FLOAT:
        return true; // All comparisons are valid
    case VariableType::BOOL:
        if (compType == ComparisonType::EQ || compType == ComparisonType::NE)
            return true;
    case VariableType::STRING:
        if (compType == ComparisonType::EQ || compType == ComparisonType::NE)
            return true;
    }
    return false;
}
