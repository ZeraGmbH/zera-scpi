#include "loopnode.h"
#include "ifnode.h"
#include "breaknode.h"


BreakNode::BreakNode(ICtrNode *parent) :
    ICmdNode(parent)
{
}

BreakNode::~BreakNode()
{
}

void BreakNode::exec(std::function<void(INode*)> &f)
{
    ICtrNode * parentNode = getParent();
    do {
        bool isLoopNode = dynamic_cast<LoopNode*>(parentNode);
        bool isIfNode = dynamic_cast<IfNode*>(parentNode);

        if (isLoopNode || isIfNode) {
            parentNode->breakExec();
            if (isLoopNode)
                break; // Break the break recursion when reaching the first loop
        }
    } while((parentNode = parentNode->getParent()) != nullptr); // Just to make sure, we stop when reaching the root node. This should never happen as the syntax check should ensure, there is no BREAK outside a LOOP.
}
