#include "ictrnode.h"
#include "exitnode.h"


ExitNode::ExitNode(ICtrNode *parent) :
    ICmdNode(parent)
{
}

ExitNode::~ExitNode()
{
}

void ExitNode::exec(std::function<void(INode*)> &f)
{
    INode *parentNode = this;

    while((parentNode = parentNode->getParent()) != nullptr)
        dynamic_cast<ICtrNode*>(parentNode)->breakExec();
}
