#ifndef SCPINODE_H
#define SCPINODE_H

#include <QStandardItem>
#include "scpiobject.h"

/**
  @brief base class to provide scpi nodes for objects
  */
class cSCPINode: public QStandardItem {
public:
    cSCPIObject* m_pSCPIObject;
    /**
      @b Initialise the const variables
      @param[in] sNodeName name of the node
      @param t type of the node
      @param[in] pSCPIObject object that handles the command
      */
    cSCPINode(const QString& sNodeName, quint8 t, cSCPIObject* pSCPIObject);
    /**
      @b Returns the node's type
      @relates SCPI::scpiNodeType
      */
    quint8 getType();
    /**
      @b Returns the node's QStandardItem type (UserType)
      */
    virtual int type();
    /**
      @b Returns the node's name if role = Qt::DisplayRole, and a description of node's type if role = Qt::ToolTipRole
      */
    //virtual QVariant data(int role = Qt::UserRole + 1 );
    virtual QVariant data ( int role = Qt::UserRole + 1 ) const;
private:
    QString m_sNodeName;
    quint8 m_nType;

};

#endif // SCPINODE_H
