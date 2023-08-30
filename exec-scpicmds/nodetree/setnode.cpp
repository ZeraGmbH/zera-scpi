#include "setnode.h"
#include "ictrnode.h"


SetNode::SetNode(ICtrNode *parent, Variable &lValue, Variable &rValue) :
    ICmdNode(parent), m_lValue(lValue), m_rValue(rValue)
{
}

SetNode::~SetNode()
{
}

void SetNode::exec(std::function<void(INode*)> &f)
{
    switch (m_lValue.getType()) {
    case VariableType::INT:
        dynamic_cast<IntVariable&>(m_lValue).setValue(dynamic_cast<IntVariable&>(m_rValue).getValue());
        break;
    case VariableType::FLOAT:
        dynamic_cast<FloatVariable&>(m_lValue).setValue(dynamic_cast<FloatVariable&>(m_rValue).getValue());
        break;
    case VariableType::BOOL:
        dynamic_cast<BoolVariable&>(m_lValue).setValue(dynamic_cast<BoolVariable&>(m_rValue).getValue());
        break;
    case VariableType::STRING:
        dynamic_cast<StringVariable&>(m_lValue).setValue(dynamic_cast<StringVariable&>(m_rValue).getValue());
        break;
    }
}
