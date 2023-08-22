#include "ifnode.h"


IfNode::IfNode(bool cond) :
    m_cond(cond)
{
}

IfNode::~IfNode()
{
}

void IfNode::exec(std::function<void(INode*)> *f)
{
    if (m_cond)
        m_ifNodes.exec(f);
    else
        m_elseNodes.exec(f);
}

void IfNode::append(INode *node)
{
    if (m_inIfBranch)
        m_ifNodes.append(node);
    else
        m_elseNodes.append(node);

    m_isEmpty = false;
}

bool IfNode::remove(INode *node)
{
    bool removed = false;
    removed = removed || m_ifNodes.remove(node);
    removed = removed || m_elseNodes.remove(node);
    return removed;
}

void IfNode::clear()
{
    m_ifNodes.clear();
    m_elseNodes.clear();
    m_isEmpty = true;
}

bool IfNode::isEmpty()
{
    CtrNode &nodes = m_inIfBranch ? m_ifNodes : m_elseNodes;
    return nodes.isEmpty();
}

bool IfNode::hasLeaves()
{
    return m_ifNodes.hasLeaves() || m_elseNodes.hasLeaves();
}

void IfNode::traverse(std::function<void(INode*)> f)
{
    if (m_cond)
        m_ifNodes.traverse(f);
    else
        m_elseNodes.traverse(f);
}

void IfNode::switchToElseBranch()
{
    m_inIfBranch = false;
}
