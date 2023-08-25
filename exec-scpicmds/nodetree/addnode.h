#ifndef ADDNODE_H
#define ADDNODE_H


#include "inode.h"
#include "icmdnode.h"
#include "context.h"


class AddNode : public ICmdNode
{
public:
    AddNode(ICtrNode *parent, Variable &lValue, Variable &rValue);
    ~AddNode();
    void exec(std::function<void(INode*)> &f);

private:
    Variable &m_lValue;
    Variable &m_rValue;
};


#endif // ADDNODE_H
