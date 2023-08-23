#ifndef SCPIMSGNODE
#define SCPIMSGNODE

#include <QString>
#include "inode.h"
#include "messagedata.h"

class ScpiMsgNode : public INode
{
public:
    ScpiMsgNode(ICtrNode *parent, std::shared_ptr<MessageData> msg);
    void exec(std::function<void(INode*)> &f) override; // Execute f not only for each leaf, but also with special logic (e.g. execute it several times in a loop)
    std::shared_ptr<MessageData> getMsgData();

private:
    std::shared_ptr<MessageData> m_msg;
};

#endif // SCPIMSGNODE
