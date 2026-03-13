#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERPOSITIONCONNECTEDFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERPOSITIONCONNECTEDFACTORY_H

#include "itransferorderpositionconnectedfactory.h"

#include "transferorderpositiondmptr.h"
#include "itransferorderpositionptr.h"

#include <libutil/accessor.h>
#include <libutil/dbconnection.h>

//------------------------------------------------------------------------------//
// Forward declarations
//------------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}

namespace libabbauw
{
namespace componentManager
{
    class TransferOrderPositionConnectedFactory : public ITransferOrderPositionConnectedFactory
    {
    public:
        TransferOrderPositionConnectedFactory();
        ~TransferOrderPositionConnectedFactory();

        void                                              injectDBConnection( libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr );

        // from ITransferOrderPositionDisconnectedFactory interface
        domMod::transferOrder::ITransferOrderPositionPtr  create( basar::db::aspect::AccessorPropertyTable_YIterator ) const;

    private:
        basar::db::aspect::ConnectionRef                  getDBConnection()                  const;

        libutil::infrastructure::accessor::IAccessorPtr   getTransferOrderPositionAccessor() const;
        domMod::transferOrder::TransferOrderPositionDMPtr getTransferOrderPositionDM()       const;

    private:
        // forbidden
        TransferOrderPositionConnectedFactory( const TransferOrderPositionConnectedFactory& );
        TransferOrderPositionConnectedFactory& operator = ( const TransferOrderPositionConnectedFactory& );

        libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr m_DBConnectionDataGetter;
    };

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERPOSITIONCONNECTEDFACTORY_H
