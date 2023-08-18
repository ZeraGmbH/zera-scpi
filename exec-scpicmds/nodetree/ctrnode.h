#ifndef CTRNODE
#define CTRNODE

#include <vector>
#include "ictrnode.h"

class CtrNode : public ICtrNode
{
public:
  CtrNode();
  ~CtrNode();
  void exec() override;
  void append(INode *node) override;

private:
  std::vector<INode*> m_nodes;
};

#endif // CTRNODE
