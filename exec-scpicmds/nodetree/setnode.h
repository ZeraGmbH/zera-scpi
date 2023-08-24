#ifndef SETNODE_H
#define SETNODE_H


#include "inode.h"
#include "icmdnode.h"
#include "context.h"


class SetNode : public ICmdNode
{
public:
    SetNode(ICtrNode *parent, Variable &lValue, Variable &rValue);
    ~SetNode();
    void exec(std::function<void(INode*)> &f);

private:
    //Context &m_context;
    Variable &m_lValue;
    Variable &m_rValue;
};


#endif // SETNODE_H
