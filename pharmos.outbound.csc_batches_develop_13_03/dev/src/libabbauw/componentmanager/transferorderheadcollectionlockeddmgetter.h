#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADCOLLECTIONLOCKEDDMGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADCOLLECTIONLOCKEDDMGETTER_H

#include "itransferorderheadcollectiondmgetter.h"
#include "itransferorderheadcollectiondmgetterptr.h"

#include "iorderlockfactoryptr.h"
#include <ilockmanagerptr.h>

namespace libabbauw
{
namespace componentManager
{

    class TransferOrderHeadCollectionLockedDMGetter : public ITransferOrderHeadCollectionDMGetter
    {
    public:
        TransferOrderHeadCollectionLockedDMGetter();
        ~TransferOrderHeadCollectionLockedDMGetter();

        void                                                                     injectLockManager( libLockManager::ILockManagerPtr );
        void                                                                     injectTransferOrderHeadCollectionDMGetter( ITransferOrderHeadCollectionDMGetterPtr );
        void                                                                     injectTransferOrderLockFactory( lock::IOrderLockFactoryPtr );

        // from ITransferOrderDMGetter interface
        domMod::transferOrder::ITransferOrderHeadCollectionDMPtr                 getTransferOrderHeadCollectionDM();

    private:
        // forbidden
        TransferOrderHeadCollectionLockedDMGetter( const TransferOrderHeadCollectionLockedDMGetter& );
        TransferOrderHeadCollectionLockedDMGetter& operator = ( const TransferOrderHeadCollectionLockedDMGetter& );

    private:
        libLockManager::ILockManagerPtr	                                         m_LockManager;
        ITransferOrderHeadCollectionDMGetterPtr                                  m_TransferOrderHeadCollectionDMGetter;
        lock::IOrderLockFactoryPtr                                               m_TransferOrderLockFactory;
    };

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADCOLLECTIONLOCKEDDMGETTER_H
