#ifndef ICTRNODE_H
#define ICTRNODE_H

#include <functional>
#include "inode.h"

class ICtrNode : public INode
{
public:
    virtual ~ICtrNode() = default;
    virtual void append(INode* node) = 0; // Append node to current container.
    virtual void clear() = 0; // Recursively clear container.
    virtual bool isEmpty() = 0; // Checks if container contains nodes (containers or leaves).
    virtual bool hasLeaves() = 0; // Checks if container contains nodes (only leaves).
    virtual bool remove(INode* node) = 0; // Find and remove all items with given address and return is at least one got found.
    virtual bool prune() = 0; // Remove empty container nodes recursively.
    virtual void traverse(std::function<void(INode*)> &f) = 0; // Traverse the tree (containers and leaves).
};

#endif // ICTRNODE_H
