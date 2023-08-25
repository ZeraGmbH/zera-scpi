#include "condition.h"


BoolCondition::BoolCondition(Variable &value) :
    m_value(value)
{
}

bool BoolCondition::eval()
{
    return *((bool*)m_value.getValue());
}


ComparisonCondition::ComparisonCondition(Variable &lValue, Variable &rValue, ComparisonType &type) :
    m_lValue(lValue), m_rValue(rValue), m_compType(type)
{
}

bool ComparisonCondition::eval()
{
    if (m_lValue.getType() == VariableType::INT) {
        int lVal = *(int*)m_lValue.getValue();
        int rVal = *(int*)m_rValue.getValue();

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
        float lVal = *(float*)m_lValue.getValue();
        float rVal = *(float*)m_rValue.getValue();

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
        bool lVal = *(bool*)m_lValue.getValue();
        bool rVal = *(bool*)m_rValue.getValue();

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
        std::string lVal = *(std::string*)m_lValue.getValue();
        std::string rVal = *(std::string*)m_rValue.getValue();

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
