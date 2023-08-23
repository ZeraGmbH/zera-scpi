#ifndef BREAKNODE_H
#define BREAKNODE_H


#include "inode.h"

class BreakNode : public ICmdNode
{
public:
    BreakNode(ICtrNode *parent);
    ~BreakNode();
    void exec(std::function<void(INode*)> &f);
};


#endif // BREAKNODE_H
