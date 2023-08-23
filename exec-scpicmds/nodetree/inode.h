#ifndef INODE_H
#define INODE_H

#include <functional>


class ICtrNode;


class INode
{
public:
    INode(ICtrNode *parent = nullptr) : m_parent(parent) {};
    ICtrNode *getParent() { return m_parent; };
    virtual ~INode() = default;
    virtual void exec(std::function<void(INode*)> &f) = 0; // Execute f()

private:
    ICtrNode *m_parent = nullptr;
};

#endif // INODE_H
