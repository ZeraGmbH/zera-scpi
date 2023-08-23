#ifndef ICMDNODE_H
#define ICMDNODE_H


#include "inode.h"


class ICmdNode : public INode
{
public:
    ICmdNode(ICtrNode *parent) : INode(parent) {};
    virtual ~ICmdNode() = default;
};

#endif // ICMDNODE_H
