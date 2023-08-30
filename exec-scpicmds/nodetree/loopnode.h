#ifndef LOOPNODE_H
#define LOOPNODE_H

#include <vector>
#include "ictrnode.h"
#include "ctrnode.h"
#include "variable.h"

class LoopNode : public ICtrNode
{
public:
    LoopNode(ICtrNode *parent);
    LoopNode(ICtrNode *parent, IntVariable &cnt);
    ~LoopNode();
    void append(INode *node) override;
    bool remove(INode *node) override;
    bool prune() override;
    void clear() override;
    bool isEmpty() override;
    bool hasLeaves() override;
    void traverse(std::function<void(INode*)> &f) override;
    void exec(std::function<void(INode*)> &f) override;
    void breakExec() override;
private:
    IntVariable *m_cnt = nullptr;
    CtrNode m_nodes;
    bool m_break = false;
};

#endif // LOOPNODE_H
