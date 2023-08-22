#ifndef IFNODE_H
#define IFNODE_H

#include <vector>
#include "ictrnode.h"
#include "ctrnode.h"


class IfNode : public ICtrNode
{
public:
    IfNode(bool cond);
    ~IfNode();
    void exec(std::function<void(INode*)> *f = nullptr) override;
    void append(INode *node) override;
    bool remove(INode* node) override;
    void clear() override;
    bool isEmpty() override;
    bool hasLeaves() override;
    void traverse(std::function<void(INode*)> f) override;
    void switchToElseBranch();

private:
    bool m_cond = false;
    bool m_inIfBranch = true;
    CtrNode m_ifNodes;
    CtrNode m_elseNodes;
    bool m_isEmpty = true;
};

#endif // IFNODE_H
