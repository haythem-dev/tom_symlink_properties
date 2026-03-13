#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADCOLLECTIONDMGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADCOLLECTIONDMGETTER_H

#include "itransferorderheadcollectiondmgetter.h"

#include "itransferorderheadcollectiondmptr.h"

#include "istockcollectiondmptr.h"
#include "iorderparameterptr.h"
#include "itransferorderfactoryptr.h"
#include "itransferorderheadfactoryptr.h"
#include "ireservequantityfactorgetterfactoryptr.h"

#include <libbasardbaspect_connectionref.h>

namespace libabbauw
{
namespace componentManager
{

    class TransferOrderHeadCollectionDMGetter : public ITransferOrderHeadCollectionDMGetter
    {
    public:
        TransferOrderHeadCollectionDMGetter();
        ~TransferOrderHeadCollectionDMGetter();

        void                                                                     setBranchNo( basar::Int16 );
        void                                                                     injectConnection( basar::db::aspect::ConnectionRef );
        void                                                                     injectStockCollection( domMod::stock::IStockCollectionDMPtr );
        void                                                                     injectOrderParameter( domMod::parameter::IOrderParameterPtr );
        void                                                                     injectTransferOrderFactory( ITransferOrderFactoryPtr );
        void                                                                     injectTransferOrderHeadFactory( ITransferOrderHeadFactoryPtr );
        void                                                                     injectReserveQuantityFactorGetterFactory( IReserveQuantityFactorGetterFactoryPtr );

        // from ITransferOrderDMGetter interface
        domMod::transferOrder::ITransferOrderHeadCollectionDMPtr                 getTransferOrderHeadCollectionDM();

    private:
        // forbidden
        TransferOrderHeadCollectionDMGetter( const TransferOrderHeadCollectionDMGetter& );
        TransferOrderHeadCollectionDMGetter& operator = ( const TransferOrderHeadCollectionDMGetter& );

    private:
        basar::Int16                                                             m_branchNo;
        basar::db::aspect::ConnectionRef	                                     m_Connection;
        domMod::stock::IStockCollectionDMPtr                                     m_StockCollection;
        domMod::parameter::IOrderParameterPtr                                    m_OrderParameter;
        ITransferOrderFactoryPtr                                                 m_TransferOrderFactory;
        ITransferOrderHeadFactoryPtr                                             m_TransferOrderHeadFactory;
        IReserveQuantityFactorGetterFactoryPtr                                   m_ReserveQuantityFactorGetterFactory;
    };

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADCOLLECTIONDMGETTER_H
