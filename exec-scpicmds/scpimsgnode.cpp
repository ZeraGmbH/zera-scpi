#include <iostream>
#include "scpimsgnode.h"


ScpiMsgNode::ScpiMsgNode(ICtrNode *parent, std::shared_ptr<MessageData> msg) :
    m_msg(msg), INode(parent)
{
}

void ScpiMsgNode::exec(std::function<void(INode*)> &f)
{
    f(this);
}

std::shared_ptr<MessageData> ScpiMsgNode::getMsgData()
{
    return m_msg;
}
