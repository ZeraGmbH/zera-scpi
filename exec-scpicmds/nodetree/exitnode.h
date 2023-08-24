#ifndef EXITNODE_H
#define EXITNODE_H


#include "inode.h"
#include "icmdnode.h"


class ExitNode : public ICmdNode
{
public:
    ExitNode(ICtrNode *parent);
    ~ExitNode();
    void exec(std::function<void(INode*)> &f);
};


#endif // EXITNODE_H
