#include <scpi.h>

#include "scpiconnection.h"
#include <scpidelegate.h>


cSCPIConnection::cSCPIConnection(QObject *parent)
    : QObject(parent)
{
}


cSCPIConnection::~cSCPIConnection()
{
    removeSCPIConnections();

    cSCPIDelegate* ptr;
    for (int i = 0; i < m_DelegateList.count(); i++)
    {
        ptr = m_DelegateList.at(i);
        delete ptr;
    }
}


void cSCPIConnection::removeSCPIConnections()
{
    cSCPIDelegate* ptr;
    for (int i = 0; i < m_DelegateList.count(); i++)
    {
        ptr = m_DelegateList.at(i);
        m_pSCPIInterface->delSCPICmds(ptr->getCommand());
    }

}


void cSCPIConnection::executeCommand(int, QString&, QString&)
{
}




