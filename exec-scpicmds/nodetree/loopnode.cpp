#include "loopnode.h"


LoopNode::LoopNode(ICtrNode *parent) :
    ICtrNode(parent), m_cnt(nullptr), m_nodes(parent)
{
}

LoopNode::LoopNode(ICtrNode *parent, Variable &cnt) :
    ICtrNode(parent), m_cnt(&cnt), m_nodes(parent)
{
}

LoopNode::~LoopNode()
{
    clear();
}

void LoopNode::append(INode *node)
{
    m_nodes.append(node);
}

bool LoopNode::remove(INode *node)
{
    return m_nodes.remove(node);
}

bool LoopNode::prune()
{
    return m_nodes.prune();
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

void LoopNode::traverse(std::function<void(INode*)> &f)
{
    m_nodes.traverse(f);
}

void LoopNode::exec(std::function<void(INode*)> &f)
{
    m_break = false;
    if (m_cnt != nullptr) {
        int cnt = *((int*)m_cnt->getValue());
        for (int i = 0; i < cnt; i++) {
            if (m_break)
                break;
            m_nodes.exec(f);
        }
    } else { // endless loop
        while (true) {
            if (m_break)
                break;
            m_nodes.exec(f);
        }
    }
}

void LoopNode::breakExec()
{
    m_nodes.breakExec();
    m_break = true;
}
