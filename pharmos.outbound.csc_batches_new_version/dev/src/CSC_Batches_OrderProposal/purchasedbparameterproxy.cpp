//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief      Implementation of PurchaseDBParameterProxy.
 *              Simply stores and returns the db / server strings that were
 *              passed at construction from the application's command-line args.
 *              No Basar parameter-gateway dependency required.
 */
//-------------------------------------------------------------------------------------------------//

#include "purchasedbparameterproxy.h"

//-------------------------------------------------------------------------------------------------//

namespace abbaUW {
namespace infrastructure {
namespace db {

//-------------------------------------------------------------------------------------------------//

PurchaseDBParameterProxy::PurchaseDBParameterProxy( const basar::VarString& purchaseDB,
                                                    const basar::VarString& purchaseDBServer )
    : m_PurchaseDB( purchaseDB )
    , m_PurchaseDBServer( purchaseDBServer )
{
}

//-------------------------------------------------------------------------------------------------//

PurchaseDBParameterProxy::~PurchaseDBParameterProxy()
{
}

//-------------------------------------------------------------------------------------------------//

const basar::VarString& PurchaseDBParameterProxy::getPurchaseDB() const
{
    return m_PurchaseDB;
}

//-------------------------------------------------------------------------------------------------//

const basar::VarString& PurchaseDBParameterProxy::getPurchaseDBServer() const
{
    return m_PurchaseDBServer;
}

//-------------------------------------------------------------------------------------------------//

} // namespace db
} // namespace infrastructure
} // namespace abbaUW

//-------------------------------------------------------------------------------------------------//
