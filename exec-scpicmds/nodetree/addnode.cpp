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
    case INT:
        m_lValue.setValue(new int(*(int*)m_lValue.getValue() + *(int*)m_rValue.getValue()));
        break;
    case FLOAT:
        m_lValue.setValue(new float(*(float*)m_lValue.getValue() + *(float*)m_rValue.getValue()));
        break;
    case BOOL:
        ; // Not possible
        break;
    case STRING:
        m_lValue.setValue(new std::string(*(std::string*)m_lValue.getValue() + *(std::string*)m_rValue.getValue()));
        break;
    }
}