#include "loopnode.h"


LoopNode::LoopNode(int cnt) :
    m_cnt(cnt)
{
}

LoopNode::~LoopNode()
{
}

void LoopNode::exec(std::function<void(INode*)> *f)
{
    for (int i = 0; i < m_cnt; i++)
        m_nodes.exec(f);
}

void LoopNode::append(INode *node)
{
    m_nodes.append(node);
}

bool LoopNode::remove(INode *node)
{
    return m_nodes.remove(node);
}

void LoopNode::clear()
{
    m_nodes.clear();
}

bool LoopNode::isEmpty()
{
    return m_nodes.isEmpty();
}

bool LoopNode::hasLeaves()
{
    return m_nodes.hasLeaves();
}

void LoopNode::traverse(std::function<void(INode*)> f)
{
    m_nodes.traverse(f);
}
