#ifndef INODE_H
#define INODE_H

#include <functional>

class INode
{
public:
    virtual ~INode() = default;
    virtual void exec(std::function<void(INode*)> *f = nullptr) = 0; // Execute f not only for each leaf, but also with special logic (e.g. execute it several times in a loop)
};

#endif // INODE_H
