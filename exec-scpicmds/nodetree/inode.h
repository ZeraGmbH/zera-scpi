#ifndef INODE
#define INODE

class INode
{
public:
    virtual ~INode() = default;
    virtual void exec() = 0;
};

#endif // INODE
