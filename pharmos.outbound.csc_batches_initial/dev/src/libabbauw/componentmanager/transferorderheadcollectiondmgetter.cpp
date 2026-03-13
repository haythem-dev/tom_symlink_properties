#include "transferorderheadcollectiondmgetter.h"

#include "transferorderheadcollectiondm.h"
#include "transferorderheadcollectiondmptr.h"

namespace libabbauw
{
namespace componentManager
{
    TransferOrderHeadCollectionDMGetter::TransferOrderHeadCollectionDMGetter()
    {
    }

    TransferOrderHeadCollectionDMGetter::~TransferOrderHeadCollectionDMGetter()
    {
    }

    void TransferOrderHeadCollectionDMGetter::setBranchNo( basar::Int16 branchNo )
    {
        m_branchNo = branchNo;
    }

    void TransferOrderHeadCollectionDMGetter::injectConnection( basar::db::aspect::ConnectionRef connection )
    {
        m_Connection = connection;
    }

    void TransferOrderHeadCollectionDMGetter::injectStockCollection( domMod::stock::IStockCollectionDMPtr stockCollection )
    {
        m_StockCollection = stockCollection;
    }

    void TransferOrderHeadCollectionDMGetter::injectOrderParameter( domMod::parameter::IOrderParameterPtr orderParameter )
    {
        m_OrderParameter = orderParameter;
    }

    void TransferOrderHeadCollectionDMGetter::injectTransferOrderFactory( ITransferOrderFactoryPtr transferOrderFactory )
    {
        m_TransferOrderFactory = transferOrderFactory;
    }

    void TransferOrderHeadCollectionDMGetter::injectTransferOrderHeadFactory( ITransferOrderHeadFactoryPtr transferOrderHeadFactory )
    {
        m_TransferOrderHeadFactory = transferOrderHeadFactory;
    }

    void TransferOrderHeadCollectionDMGetter::injectReserveQuantityFactorGetterFactory( IReserveQuantityFactorGetterFactoryPtr reserveQtyFactorGetterFactory )
    {
        m_ReserveQuantityFactorGetterFactory = reserveQtyFactorGetterFactory;
    }

    domMod::transferOrder::ITransferOrderHeadCollectionDMPtr TransferOrderHeadCollectionDMGetter::getTransferOrderHeadCollectionDM()
    {
        domMod::transferOrder::TransferOrderHeadCollectionDMPtr orderHeadCollectionDMPtr( 
                new domMod::transferOrder::TransferOrderHeadCollectionDM( m_Connection ) 
        );
	    orderHeadCollectionDMPtr->injectStockCollection( m_StockCollection );
        orderHeadCollectionDMPtr->injectOrderFactory( m_TransferOrderFactory );
        orderHeadCollectionDMPtr->injectOrderHeadFactory( m_TransferOrderHeadFactory );
        orderHeadCollectionDMPtr->injectReserveQuantityFactorGetterFactory( m_ReserveQuantityFactorGetterFactory );
	    orderHeadCollectionDMPtr->injectOrderParameter( m_OrderParameter );

        orderHeadCollectionDMPtr->init( domMod::transferOrder::TransferOrderHeadCollectionInitParams( m_branchNo ) );

        return orderHeadCollectionDMPtr;
    }
} // end namespace componentManager
} // end namespace libabbauw
