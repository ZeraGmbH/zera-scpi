#include "inode.h"
#include "ictrnode.h"
#include "ctrnode.h"
#include "nodetree.h"


NodeTree::NodeTree(ICtrNode *parent) : ICtrNode(parent)
{
    m_root = new CtrNode(parent);
    m_curCtr = m_root;
    m_parentCtr = m_root;
}

NodeTree::~NodeTree()
{
    delete m_root;
    while (m_ctrs.size() != 0)
           m_ctrs.pop();
}

void NodeTree::enterContainer(ICtrNode *ctr)
{
    m_curCtr = ctr; // Set new container node as current container.
    m_parentCtr->append(m_curCtr); // Add current container node as sibling to the parent node.
    m_parentCtr = m_curCtr; // Make the parent point to the current container node for the next recursion step.
    m_ctrs.push(m_curCtr); // Extend path
}

void NodeTree::leaveContainer()
{
    m_ctrs.pop();
    if (!m_ctrs.empty())
        m_curCtr = m_ctrs.top();
    else
        m_curCtr = m_root;
    m_parentCtr = m_curCtr;
}

ICtrNode* NodeTree::getCurrentContainer()
{
    return m_parentCtr;
}

void NodeTree::append(INode* node)
{
    m_curCtr->append(node);
}

bool NodeTree::remove(INode* node)
{
    return m_curCtr->remove(node);
}

bool NodeTree::prune()
{
    return m_curCtr->prune();
}

void NodeTree::clear()
{
    m_root->clear();
    delete m_root;
    m_root = nullptr;
    m_parentCtr = nullptr;
    m_curCtr = nullptr;
    while (m_ctrs.size() != 0)
        m_ctrs.pop();
    bool m_break = false;
}

bool NodeTree::isEmpty()
{
    return m_root->isEmpty();
}

bool NodeTree::hasLeaves()
{
    return m_root->hasLeaves();
}

void NodeTree::traverse(std::function<void(INode*)> &f)
{
    m_root->traverse(f);
}

void NodeTree::exec(std::function<void(INode*)> &f)
{
    m_root->exec(f);
}

void NodeTree::breakExec()
{
    m_break = true;
}

CtrNode* NodeTree::getRoot()
{
    return m_root;
}
