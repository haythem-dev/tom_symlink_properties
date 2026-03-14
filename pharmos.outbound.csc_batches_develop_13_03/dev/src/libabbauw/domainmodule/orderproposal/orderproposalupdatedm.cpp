#include "orderproposalupdatedm.h"

#include "ipurchasebatchorderposupdater.h"
#include "loggerpool/libabbauw_loggerpool.h"
#include "exceptions/orderproposalexception.h"

#include <libbasardbaspect_accessorpropertytable_yiterator.h>

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
    const libutil::misc::ClassInfo & OrderProposalUpdateDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "OrderProposalUpdateDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

    OrderProposalUpdateDM::OrderProposalUpdateDM()
		: m_Logger( LoggerPool::loggerOrderProposal )
	{
        METHODNAME_DEF( OrderProposalUpdateDM, OrderProposalUpdateDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
	}

	OrderProposalUpdateDM::~OrderProposalUpdateDM()
	{
        METHODNAME_DEF( OrderProposalUpdateDM, ~OrderProposalUpdateDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
	}

    const log4cplus::Logger& OrderProposalUpdateDM::getLogger() const
    {
        return m_Logger;
    }

    void OrderProposalUpdateDM::injectOrderProposal( orderProposal::IOrderProposalPtr orderProposal )
    {
        m_OrderProposal = orderProposal;
    }

    orderProposal::IOrderProposalPtr OrderProposalUpdateDM::getOrderProposal() const
    {
        CHECK_INSTANCE_EXCEPTION( m_OrderProposal.get() );
        return m_OrderProposal;
    }

    void OrderProposalUpdateDM::injectPurchaseBatchOrderPosUpdater( purchaseBatchOrder::IPurchaseBatchOrderPosUpdaterPtr puBatchOrderPosUpdater )
    {
        m_PurchaseBatchOrderPosUpdater = puBatchOrderPosUpdater;
    }

	void OrderProposalUpdateDM::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
    {
        m_TransactionFactory = transactionFactory;
    }

    purchaseBatchOrder::IPurchaseBatchOrderPosUpdaterPtr OrderProposalUpdateDM::getPurchaseBatchOrderPosUpdater() const
    {
        CHECK_INSTANCE_EXCEPTION( m_PurchaseBatchOrderPosUpdater.get() );
        return m_PurchaseBatchOrderPosUpdater;
    }

    basar::db::aspect::AccessorPropertyTable_YIterator  OrderProposalUpdateDM::get() const
    {
        return getOrderProposal()->get();
    }

    bool OrderProposalUpdateDM::isEmpty() const
    {
        return getOrderProposal()->isEmpty();
    }

	basar::db::aspect::AccessorPropertyTable_YIterator OrderProposalUpdateDM::getSearchYit()
	{
		return getOrderProposal()->getSearchYit();
	}

	void OrderProposalUpdateDM::resetSearchYit()
	{
        getOrderProposal()->resetSearchYit();
	}
    
    void OrderProposalUpdateDM::findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( OrderProposalUpdateDM, findByKey )
        BLOG_TRACE_METHOD(getLogger(), fun );

        getOrderProposal()->findByKey( yitSearch );
        if( ! getOrderProposal()->isEmpty() )
        {
            getPurchaseBatchOrderPosUpdater()->getSearchYit().match( getOrderProposal()->get() );
            getPurchaseBatchOrderPosUpdater()->findByKey( getPurchaseBatchOrderPosUpdater()->getSearchYit() );
        }
    }

    void OrderProposalUpdateDM::setOrderNo( basar::Int32 orderNo )
    {
        getOrderProposal()->setOrderNo( orderNo );
        if( ! getPurchaseBatchOrderPosUpdater()->isEmpty() )
        {
            getPurchaseBatchOrderPosUpdater()->setOrderNo( orderNo );
        }
    }

    void OrderProposalUpdateDM::save()
    {
        METHODNAME_DEF( OrderProposalUpdateDM, save )
        BLOG_TRACE_METHOD(getLogger(), fun );

        getOrderProposal()->save();
        getPurchaseBatchOrderPosUpdater()->save();
    }

    void OrderProposalUpdateDM::erase()
    {
        METHODNAME_DEF( OrderProposalUpdateDM, save )
        BLOG_TRACE_METHOD(getLogger(), fun );

		libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();
		
		try
		{
			transaction->begin();

			// first purchase batch order pos updater has to be called because it does some checks and throws exceptions
			// if the conditions are not fullfilled. In this case the order proposal must not be deleted.
			getPurchaseBatchOrderPosUpdater()->erase();
			getOrderProposal()->erase();

			transaction->commit();
		}
		catch ( exceptions::OrderProposalException& e )
		{
			transaction->rollback();

			BLOG_INFO( getLogger(), "Order proposal can not be deleted: " + e.reason() );
			throw;
		}
    }

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw
