#include "printnode.h"
#include "../logging.h"


PrintNode::PrintNode(ICtrNode *parent, std::vector<Variable*> *values) :
    ICmdNode(parent), m_values(values)
{
}

PrintNode::~PrintNode()
{
}

void PrintNode::exec(std::function<void(INode*)> &f)
{
    QString str = "";
    for (auto val : *m_values)
        str.append(val->toString());
    Logging::logMsg(str);
}
