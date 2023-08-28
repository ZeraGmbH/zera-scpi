#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QObject>
#include "tcphandler.h"
#include "messagedata.h"
#include "ifnode.h"
#include "nodetree.h"
#include "variable.h"
#include "context.h"


enum class ContainerType
{
    LOOP = 0,
    IF,
};


struct CommandParserContext
{
    CommandParserContext(QStringList &fields, std::stack<IfNode*> &ifnodes, std::stack<ContainerType> &ctrTypes, int &fileLineNumber, int &lastElseNodeLineNumber) :
        fields(fields), ifnodes(ifnodes), ctrTypes(ctrTypes), fileLineNumber(fileLineNumber), lastElseNodeLineNumber(lastElseNodeLineNumber) {}

    QStringList &fields;
    std::stack<IfNode*> &ifnodes;
    std::stack<ContainerType> &ctrTypes;
    int fileLineNumber;
    int lastElseNodeLineNumber;
};


class CommandParser : public QObject
{
    Q_OBJECT
public:
    CommandParser(TcpHandler& tcpHandler);
    void setHandleErroneousMessages(unsigned int handleErroneousMessages);
    void setCheckErrorQueue(bool checkErrorQueue);
    void setLoopNumber(quint8 numLoops);
    void setIgnoreExistingVariables(bool ignoreExistingVariables);
    void parseCmdFile(QString strFileName);

signals:
    void done(int exitCode);

private:
    bool msgsAreValid();
    void sendMsgs(unsigned int iterNo = 0);
    void loopAndSendMsgs();
    void removeInvalidMsgs(bool silent);
    void parseVarStatement(CommandParserContext &cpc);
    void parseSetStatement(CommandParserContext &cpc);
    void parseAddStatement(CommandParserContext &cpc);
    void parseLoopStatement(CommandParserContext &cpc);
    void parseBreakStatement(CommandParserContext &cpc);
    void parseExitStatement(CommandParserContext &cpc);
    void parsePrintStatement(CommandParserContext &cpc);
    void parseIfStatement(CommandParserContext &cpc);
    void parseElseStatement(CommandParserContext &cpc);
    void parseEndStatement(CommandParserContext &cpc);

    unsigned int m_handleErroneousMessages = 0;
    bool m_checkErrorQueue = false;
    TcpHandler& m_tcpHandler;
    quint8 m_numLoops = 1;
    bool m_ignoreExistingVariables = false;
    NodeTree m_tree;
    Context gc; // global context
};

#endif // COMMANDPARSER_H
