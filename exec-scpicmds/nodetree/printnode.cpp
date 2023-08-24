#include "printnode.h"


PrintNode::PrintNode(ICtrNode *parent, std::vector<Variable*> *values, std::function<void(std::string&)> &cbLog) :
    ICmdNode(parent), m_values(values), m_cbLog(cbLog)
{
}

PrintNode::~PrintNode()
{
}

void PrintNode::exec(std::function<void(INode*)> &f)
{
    std::string str = "";
    for (auto val : *m_values)
        str.append(val->toString());
    m_cbLog(str);
}
