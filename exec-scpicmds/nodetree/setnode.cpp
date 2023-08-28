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
        m_lValue.setValue(new int(*(int*)m_rValue.getValue()));
        break;
    case VariableType::FLOAT:
        m_lValue.setValue(new float(*(float*)m_rValue.getValue()));
        break;
    case VariableType::BOOL:
        m_lValue.setValue(new bool(*(bool*)m_rValue.getValue()));
        break;
    case VariableType::STRING:
        m_lValue.setValue(new std::string(*(std::string*)m_rValue.getValue()));
        break;
    }
}
