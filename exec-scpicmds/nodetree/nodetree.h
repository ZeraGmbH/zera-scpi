#include <stack>
#include "inode.h"
#include "ictrnode.h"


class NodeTree : ICtrNode
{
public:
    NodeTree();
    ~NodeTree();
    void enterContainer(ICtrNode *ctr);
    void leaveContainer();
    void append(INode* node) override;
    void exec() override;

private:
    ICtrNode *m_root = nullptr;
    ICtrNode *m_parentCtr = nullptr;
    ICtrNode *m_curCtr = nullptr;
    std::stack<ICtrNode*> m_ctrs; // for traversing the tree
};
