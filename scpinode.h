#ifndef SCPINODE_H
#define SCPINODE_H

#include <QStandardItem>
#include "scpiobject.h"

/**
  @brief base class to provide scpi nodes for objects
  */
class cSCPINode: public QStandardItem {
public:
    /**
      @b pointer to the real object that this node represents
      */
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
    /**
      @b Readable name of the node used in the model as displayRole
      */
    QString m_sNodeName;
    /**
      @b node type
      @see SCPI::eSCPINodeType
      */
    quint8 m_nType;

};

#endif // SCPINODE_H
