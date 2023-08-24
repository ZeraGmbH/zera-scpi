#ifndef PRINTNODE_H
#define PRINTNODE_H


#include "inode.h"
#include "icmdnode.h"
#include "context.h"


class PrintNode : public ICmdNode
{
public:
    PrintNode(ICtrNode *parent, std::vector<Variable*> *values, std::function<void(std::string&)> &cbLog);
    ~PrintNode();
    void exec(std::function<void(INode*)> &f);

private:
    std::vector<Variable*> *m_values = nullptr;
    std::function<void(std::string&)> &m_cbLog;
};


#endif // PRINTNODE_H
