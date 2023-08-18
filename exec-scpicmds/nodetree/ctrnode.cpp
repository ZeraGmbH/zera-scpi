#include <iostream>
#include "ctrnode.h"


CtrNode::CtrNode()
{
}

CtrNode::~CtrNode()
{
    for (auto iter = m_nodes.begin(); iter < m_nodes.end(); iter++)
        delete (*iter);
    m_nodes.clear();
}

void CtrNode::exec()
{
    for (auto iter = m_nodes.begin(); iter < m_nodes.end(); iter++)
        (*iter)->exec();
}

void CtrNode::append(INode *node)
{
    m_nodes.push_back(node);
}
