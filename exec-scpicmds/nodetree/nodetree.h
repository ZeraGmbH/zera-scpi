#ifndef NODETREE_H
#define NODETREE_H

#include <stack>
#include "inode.h"
#include "ictrnode.h"
#include "ctrnode.h"


class NodeTree : ICtrNode
{
public:
    NodeTree();
    ~NodeTree();
    void enterContainer(ICtrNode *ctr);
    void leaveContainer();
    void append(INode* node) override;
    bool remove(INode* node) override;
    void clear() override;
    bool isEmpty() override;
    bool hasLeaves() override;
    void exec(std::function<void(INode*)> *f = nullptr) override;
    void traverse(std::function<void(INode*)> f) override;
    CtrNode *getRoot();

private:
    CtrNode *m_root = nullptr;
    ICtrNode *m_parentCtr = nullptr;
    ICtrNode *m_curCtr = nullptr;
    std::stack<ICtrNode*> m_ctrs; // for traversing the tree
};

#endif // NODETREE_H
