#ifndef ICTRNODE
#define ICTRNODE

#include "inode.h"

class ICtrNode : public INode
{
public:
    virtual ~ICtrNode() = default;
    virtual void append(INode* cmd) = 0;
};

#endif // ICTRNODE
