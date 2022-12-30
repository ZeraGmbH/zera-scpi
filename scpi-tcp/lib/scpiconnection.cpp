#include <scpi.h>
#include "scpiconnection.h"
#include <scpidelegate.h>

cSCPIConnection::cSCPIConnection(cSCPI *scpiInterface)
    : m_pSCPIInterface(scpiInterface)
{
}

cSCPIConnection::~cSCPIConnection()
{
    removeSCPIConnections();
    for (int i = 0; i < m_DelegateList.count(); i++) {
        cSCPIDelegate* ptr = m_DelegateList.at(i);
        delete ptr;
    }
}

void cSCPIConnection::removeSCPIConnections()
{
    if(m_pSCPIInterface) {
        for (int i = 0; i < m_DelegateList.count(); i++) {
            cSCPIDelegate* ptr = m_DelegateList.at(i);
            m_pSCPIInterface->delSCPICmds(ptr->getCommand());
        }
    }
}

void cSCPIConnection::executeCommand(int, QString&, QString&)
{
}
