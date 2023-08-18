#ifndef LOOPNODE
#define LOOPNODE

#include <vector>
#include "ictrnode.h"
#include "ctrnode.h"

class LoopNode : public ICtrNode
{
public:
    LoopNode(int cnt);
    ~LoopNode();
    void exec() override;
    void append(INode *node) override;

private:
    int m_cnt = 0;
    CtrNode m_nodes;
};

#endif // LOOPNODE
