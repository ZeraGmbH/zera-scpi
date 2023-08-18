#include "loopnode.h"


LoopNode::LoopNode(int cnt) :
    m_cnt(cnt)
{
}

LoopNode::~LoopNode()
{
}

void LoopNode::exec()
{
    for (int i = 0; i < m_cnt; i++)
    {
        m_nodes.exec();
    }
}

void LoopNode::append(INode *node)
{
    m_nodes.append(node);
}
