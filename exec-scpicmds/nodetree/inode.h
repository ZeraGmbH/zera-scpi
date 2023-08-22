#ifndef INODE_H
#define INODE_H

#include <functional>

class INode
{
public:
    virtual ~INode() = default;
    virtual void exec(std::function<void(INode*)> &f) = 0; // Execute f
};

#endif // INODE_H
