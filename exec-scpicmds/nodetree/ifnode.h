#ifndef IFNODE_H
#define IFNODE_H

#include <vector>
#include "ictrnode.h"
#include "ctrnode.h"
#include "condition.h"


class IfNode : public ICtrNode
{
public:
    IfNode(ICtrNode *parent, Condition &cond);
    ~IfNode();
    void append(INode *node) override;
    bool remove(INode* node) override;
    bool prune() override;
    void clear() override;
    bool isEmpty() override;
    bool hasLeaves() override;
    void traverse(std::function<void(INode*)> &f) override;
    void exec(std::function<void(INode*)> &f) override;
    void breakExec() override;
    void switchToElseBranch();
    bool isInElseBranch();

private:
    Condition &m_cond;
    bool m_inIfBranch = true;
    CtrNode m_ifNodes;
    CtrNode m_elseNodes;
    bool m_isEmpty = true;
};

#endif // IFNODE_H
