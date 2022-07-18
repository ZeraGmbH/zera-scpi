#ifndef PROTONETCOMMAND_H
#define PROTONETCOMMAND_H

#include <QByteArray>
#include <QString>

class XiQNetPeer;

class cProtonetCommand
{
public:
    cProtonetCommand(XiQNetPeer* peer, bool hasClientId, bool withOutput, QByteArray clientid, quint32 messagenr, QString input, quint8 scpiType = 0);
    cProtonetCommand(const cProtonetCommand* protoCmd);
    XiQNetPeer* m_pPeer;
    bool m_bhasClientId;
    bool m_bwithOutput;
    QByteArray m_clientId;
    quint32 m_nmessageNr;
    QString m_sInput;
    QString m_sOutput;
    quint8 m_nSCPIType = 0; // just for SEC - we have to get rid!!
};

#endif // PROTONETCOMMAND_H
