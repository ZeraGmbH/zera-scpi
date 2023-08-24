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
    case INT:
        m_lValue.setValue(new int(*(int*)m_rValue.getValue()));
        break;
    case FLOAT:
        m_lValue.setValue(new float(*(float*)m_rValue.getValue()));
        break;
    case BOOL:
        m_lValue.setValue(new bool(*(bool*)m_rValue.getValue()));
        break;
    case STRING:
        m_lValue.setValue(new QString(*(QString*)m_rValue.getValue()));
        break;
    }
}
