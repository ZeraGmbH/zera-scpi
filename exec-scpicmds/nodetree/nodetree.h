#ifndef NODETREE_H
#define NODETREE_H

#include <stack>
#include "inode.h"
#include "ictrnode.h"
#include "ctrnode.h"


class NodeTree : ICtrNode
{
public:
    NodeTree(ICtrNode *parent);
    ~NodeTree();
    void enterContainer(ICtrNode *ctr);
    void leaveContainer();
    ICtrNode* getCurrentContainer();
    void append(INode* node) override;
    bool remove(INode* node) override;
    bool prune() override;
    void clear() override;
    bool isEmpty() override;
    bool hasLeaves() override;
    void traverse(std::function<void(INode*)> &f) override;
    void exec(std::function<void(INode*)> &f) override;
    void breakExec() override;
    CtrNode* getRoot();

private:
    CtrNode *m_root = nullptr;
    ICtrNode *m_parentCtr = nullptr;
    ICtrNode *m_curCtr = nullptr; // Points the current container we're in.
    std::stack<ICtrNode*> m_ctrs; // Keeps track of the current containers parent and parent's parent (i.e. its path to the root node). This is necessary for traversing the tree.
    bool m_break = false;
};

#endif // NODETREE_H
