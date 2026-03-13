//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief      Holds purchasedb / purchasedbsrv strings for use by PuBatchOrderPosDM.
 *              Values are supplied directly at construction time from the
 *              application's command-line args (m_DB / m_Server), avoiding
 *              any dependency on a Basar parameter-gateway header that is not
 *              present in this Basar version.
 *              Mirrors the intent of the tender PurchaseDBParameterProxy pattern.
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_INFRASTRUCTURE_DB_PURCHASEDBPARAMETERPROXY_H
#define GUARD_ABBAUW_INFRASTRUCTURE_DB_PURCHASEDBPARAMETERPROXY_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ipurchasedbparameterproxy.h"
#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace abbaUW {
namespace infrastructure {
namespace db {

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class PurchaseDBParameterProxy : public IPurchaseDBParameterProxy
{
public:
    // Supply the dblink database-name and server-name at construction.
    // In CSC_Batches_OrderProposal these come from the command-line args
    // m_DB (-> PURCHASE_DB) and m_Server (-> PURCHASE_DBSRV).
    PurchaseDBParameterProxy( const basar::VarString& purchaseDB,
                              const basar::VarString& purchaseDBServer );

    virtual ~PurchaseDBParameterProxy();

    virtual const basar::VarString& getPurchaseDB()       const;
    virtual const basar::VarString& getPurchaseDBServer() const;

private:
    PurchaseDBParameterProxy();
    PurchaseDBParameterProxy( const PurchaseDBParameterProxy& );
    PurchaseDBParameterProxy& operator=( const PurchaseDBParameterProxy& );

    basar::VarString m_PurchaseDB;
    basar::VarString m_PurchaseDBServer;
};

//-------------------------------------------------------------------------------------------------//

} // namespace db
} // namespace infrastructure
} // namespace abbaUW

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_ABBAUW_INFRASTRUCTURE_DB_PURCHASEDBPARAMETERPROXY_H
