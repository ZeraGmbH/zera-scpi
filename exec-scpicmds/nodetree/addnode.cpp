#include "addnode.h"
#include "ictrnode.h"


AddNode::AddNode(ICtrNode *parent, Variable &lValue, Variable &rValue) :
    ICmdNode(parent), m_lValue(lValue), m_rValue(rValue)
{
}

AddNode::~AddNode()
{
}

void AddNode::exec(std::function<void(INode*)> &f)
{
    switch (m_lValue.getType()) {
    case VariableType::INT:
        dynamic_cast<IntVariable&>(m_lValue).setValue(dynamic_cast<IntVariable&>(m_lValue).getValue() + dynamic_cast<IntVariable&>(m_rValue).getValue());
        break;
    case VariableType::FLOAT:
        dynamic_cast<FloatVariable&>(m_lValue).setValue(dynamic_cast<FloatVariable&>(m_lValue).getValue() + dynamic_cast<FloatVariable&>(m_rValue).getValue());
        break;
    case VariableType::BOOL:
        ; // Not possible
        break;
    case VariableType::STRING:
        dynamic_cast<StringVariable&>(m_lValue).setValue(dynamic_cast<StringVariable&>(m_lValue).getValue() + dynamic_cast<StringVariable&>(m_rValue).getValue());
        break;
    }
}
