#include "ifnode.h"


IfNode::IfNode(ICtrNode *parent, Condition &cond) :
    ICtrNode(parent), m_cond(cond), m_ifNodes(parent), m_elseNodes(parent)
{
}

IfNode::~IfNode()
{
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

bool IfNode::prune()
{
    bool pruned = false;
    pruned = pruned || m_ifNodes.prune();
    pruned = pruned || m_elseNodes.prune();
    return pruned;
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

void IfNode::traverse(std::function<void(INode*)> &f)
{
    m_ifNodes.traverse(f);
    m_elseNodes.traverse(f);
}

void IfNode::exec(std::function<void(INode*)> &f)
{
    if (m_cond.eval())
        m_ifNodes.exec(f);
    else
        m_elseNodes.exec(f);
}

void IfNode::breakExec()
{
    // We just break both, so we don't nned to check, which one is the active one.
    m_ifNodes.breakExec();
    m_elseNodes.breakExec();
}

void IfNode::switchToElseBranch()
{
    m_inIfBranch = false;
}
