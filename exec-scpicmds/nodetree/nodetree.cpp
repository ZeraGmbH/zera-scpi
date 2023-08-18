#include "inode.h"
#include "ictrnode.h"
#include "ctrnode.h"
#include "nodetree.h"


NodeTree::NodeTree()
{
    m_root = new CtrNode();
    m_curCtr = m_root;
    m_parentCtr = m_root;
}

NodeTree::~NodeTree()
{
    delete m_root;
}

void NodeTree::enterContainer(ICtrNode *ctr)
{
    m_curCtr = ctr;
    m_parentCtr->append(m_curCtr);
    m_parentCtr = m_curCtr;
    m_ctrs.push(m_curCtr);
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

void NodeTree::append(INode* node)
{
    m_curCtr->append(node);
}

void NodeTree::exec()
{
    m_root->exec();
}
