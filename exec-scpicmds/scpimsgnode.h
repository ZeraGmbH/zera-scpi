#ifndef SCPIMSGNODE
#define SCPIMSGNODE

#include <QString>
#include "inode.h"
#include "messagedata.h"

class ScpiMsgNode : public INode
{
public:
    ScpiMsgNode(std::shared_ptr<MessageData> msg);
    void exec(std::function<void(INode*)> *f = nullptr) override;
    std::shared_ptr<MessageData> getMsgData();

private:
    std::shared_ptr<MessageData> m_msg;
};

#endif // SCPIMSGNODE
