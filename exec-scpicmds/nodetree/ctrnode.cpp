#include <iostream>
#include "ctrnode.h"


CtrNode::CtrNode()
{
}

CtrNode::~CtrNode()
{
    this->clear();
}

void CtrNode::exec(std::function<void(INode*)> *f)
{
    for (auto &it : m_nodes)
        it->exec(f);
}

void CtrNode::append(INode *node)
{
    m_nodes.push_back(node);
    m_isEmpty = false;
}

bool CtrNode::remove(INode* node)
{
    bool found = false;

    ICtrNode *n = nullptr;
    int i = 0;
    for (auto it: m_nodes) {
        n = dynamic_cast<ICtrNode*>(it);
        if(n != nullptr) {
            found = found || n->remove(node);
        }
        else {
            if (node == it) // Found
            {
                delete (it);
                m_nodes.erase(m_nodes.begin() + i); // Delete item
                found = true;
            }
        }
        i++;
    }

    if (m_nodes.size() == 0)
        m_isEmpty = false;

    return found;
}

void CtrNode::clear()
{
    ICtrNode *node = nullptr;
    for (auto it: m_nodes) {
        node = dynamic_cast<ICtrNode*>(it);
        if(node != nullptr)
            node->clear();
        delete (it);
    }
    m_nodes.clear();
    m_isEmpty = true;
}

bool CtrNode::isEmpty()
{
    return m_isEmpty;
}

bool CtrNode::hasLeaves()
{
    ICtrNode *node = nullptr;
    for (auto it: m_nodes) {
        node = dynamic_cast<ICtrNode*>(it);
        if(node == nullptr)
            return true;
    }

    return false;
}

void CtrNode::traverse(std::function<void(INode*)> f)
{
    for (auto &it : m_nodes) {
        ICtrNode *node = dynamic_cast<ICtrNode*>(it);
        if(node != nullptr)
            node->traverse(f);
        else
            f(it);
    }
}
