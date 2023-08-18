#include "ifnode.h"


IfNode::IfNode(bool cond) :
    m_cond(cond)
{
}

IfNode::~IfNode()
{
}

void IfNode::exec()
{
    if (m_cond)
        m_ifNodes.exec();
    else
        m_elseNodes.exec();
}

void IfNode::append(INode *node)
{
    if (m_inIfBranch)
        m_ifNodes.append(node);
    else
        m_elseNodes.append(node);
}

void IfNode::switchToElseBranch()
{
    m_inIfBranch = false;
}
