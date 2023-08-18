#ifndef IFNODE
#define IFNODE

#include <vector>
#include "ictrnode.h"
#include "ctrnode.h"


class IfNode : public ICtrNode
{
public:
    IfNode(bool cond);
    ~IfNode();
    void exec() override;
    void append(INode *node) override;
    void switchToElseBranch();

private:
    bool m_cond = false;
    bool m_inIfBranch = true;
    CtrNode m_ifNodes;
    CtrNode m_elseNodes;
};

#endif // IFNODE
