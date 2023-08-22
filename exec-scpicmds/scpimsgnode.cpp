#include <iostream>
#include "scpimsgnode.h"


ScpiMsgNode::ScpiMsgNode(std::shared_ptr<MessageData> msg) :
    m_msg(msg)
{
}

void ScpiMsgNode::exec(std::function<void(INode*)> *f)
{
    if (f != nullptr)
        (*f)(this);
}

std::shared_ptr<MessageData> ScpiMsgNode::getMsgData()
{
    return m_msg;
}
