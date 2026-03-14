//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "purchasebatchorderposdm.h"

#include "infrastructure/accessor/purchasebatchorder/purchasebatchorderposacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"
#include "exceptions/orderproposalexception.h"

#include <libutil/searchyiterator.h>
#include <libutil/exception.h>
#include <libutil/util.h>
#include <libutil/accessor.h>

#include <libutil/misc/datetime/datetime.h>

#include <boost/make_shared.hpp>

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace purchaseBatchOrder
{
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
    const libutil::misc::ClassInfo & PurchaseBatchOrderPosDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "PurchaseBatchOrderPosDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

    PurchaseBatchOrderPosDM::PurchaseBatchOrderPosDM()
		: m_Logger( LoggerPool::loggerOrderProposal )
	{
        METHODNAME_DEF( PurchaseBatchOrderPosDM, PurchaseBatchOrderPosDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
	}

	PurchaseBatchOrderPosDM::~PurchaseBatchOrderPosDM()
	{
        METHODNAME_DEF( PurchaseBatchOrderPosDM, ~PurchaseBatchOrderPosDM )
        BLOG_TRACE_METHOD(getLogger(), fun );
	}

    const log4cplus::Logger& PurchaseBatchOrderPosDM::getLogger() const
    {
        return m_Logger;
    }

    void PurchaseBatchOrderPosDM::injectPurchaseBatchOrderPosAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
    {
        m_Accessor = accessor;
    }

    libutil::infrastructure::accessor::IAccessorPtr PurchaseBatchOrderPosDM::getAccessor() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
        return m_Accessor;
    }

    basar::db::aspect::AccessorPropertyTable_YIterator PurchaseBatchOrderPosDM::get()
	{
        const PurchaseBatchOrderPosDM& me = *this; // force use of const method
        return me.get();
	}

    basar::db::aspect::AccessorPropertyTable_YIterator PurchaseBatchOrderPosDM::get() const
	{
        // get or create new one if not existing
        basar::db::aspect::AccessorPropertyTable_YIterator yit;
        if( ! isEmpty() )
        {
            yit = getAccessor()->getPropertyTable().begin();
        }
        else
        {
            yit = getAccessor()->getPropertyTable().insert( basar::FOR_INSERT );
        }

        return yit;
	}

    bool PurchaseBatchOrderPosDM::isEmpty() const
    {
        return getAccessor()->getPropertyTable().isEmpty();
    }

	basar::db::aspect::AccessorPropertyTable_YIterator PurchaseBatchOrderPosDM::getSearchYit()
	{
		return getSearchYIterator()->get();
	}

	void PurchaseBatchOrderPosDM::resetSearchYit()
	{
        getSearchYIterator()->reset();
	}
    
    void PurchaseBatchOrderPosDM::findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( PurchaseBatchOrderPosDM, findByKey )
        BLOG_TRACE_METHOD(getLogger(), fun );

        using libabbauw::infrastructure::accessor::purchaseBatchOrder::lit::SELECT_PURCHASE_BATCH_ORDER_POS_BY_KEY;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            executeResultInfo = m_Accessor->execute( SELECT_PURCHASE_BATCH_ORDER_POS_BY_KEY, yitSearch );
        }
        catch( basar::Exception & e  )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, getLogger(), fun, SELECT_PURCHASE_BATCH_ORDER_POS_BY_KEY, __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PURCHASE_BATCH_ORDER_POS_BY_KEY, executeResultInfo )
        }
    }

    void PurchaseBatchOrderPosDM::setOrderNo( basar::Int32 orderNo )
    {
        get().setInt32( properties::ORDERNO_UPDATE, orderNo );
    }

    void PurchaseBatchOrderPosDM::save()
    {
        METHODNAME_DEF( PurchaseBatchOrderPosDM, save )
        BLOG_TRACE_METHOD(getLogger(), fun );

        if( isEmpty() )
        {
            return;
        }

        using libabbauw::infrastructure::accessor::purchaseBatchOrder::lit::SAVE_PURCHASE_BATCH_ORDER_POS;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            executeResultInfo = m_Accessor->execute( SAVE_PURCHASE_BATCH_ORDER_POS, get() );
        }
        catch( basar::Exception & e  )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, getLogger(), fun, SAVE_PURCHASE_BATCH_ORDER_POS, __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_PURCHASE_BATCH_ORDER_POS, executeResultInfo )
        }
    }

	void PurchaseBatchOrderPosDM::checkErasable() const
	{
		METHODNAME_DEF( PurchaseBatchOrderPosDM, checkErasable )
        BLOG_TRACE_METHOD( getLogger(), fun );

		if( ! get().getDateTime( properties::ORDER_PROP_PROCESSED_TIME ).isValid() &&
			get().getInt32( properties::ORDER_PROP_NO ) == 0 )
		{
			return;  // good-case, we accept
		}


		if( get().getDateTime( properties::ORDER_PROP_PROCESSED_TIME ).isValid() &&
			get().getInt32( properties::ORDER_PROP_NO ) != 0 &&
			get().getInt32( properties::PURCHASEORDERNO ) == 0 )
		{
			libutil::misc::DateTime yesterday( basar::cmnutil::Date::getCurrent() );
			yesterday.addDays( -1 );
			
			if( get().getDateTime( properties::ORDER_PROP_PROCESSED_TIME ) <= yesterday )
			{
				return;  // good-case, we accept
			}
			else
			{
				basar::ExceptInfo info( fun, "", __FILE__, __LINE__ );
				throw exceptions::OrderProposalException( info, exceptions::OrderProposalException::ORDER_IS_TODAY );
			}
		}

		if( get().getInt32( properties::PURCHASEORDERNO ) != 0 )
		{
			basar::ExceptInfo info( fun, "", __FILE__, __LINE__ );
			throw exceptions::OrderProposalException( info, exceptions::OrderProposalException::ORDER_IS_DONE );
		}

		return;  // by default, we accept
	}

    void PurchaseBatchOrderPosDM::erase()
    {
        METHODNAME_DEF( PurchaseBatchOrderPosDM, erase )
        BLOG_TRACE_METHOD(getLogger(), fun );

        if( isEmpty() )
        {
            return;
        }

		checkErasable();

        using libabbauw::infrastructure::accessor::purchaseBatchOrder::lit::DELETE_PURCHASE_BATCH_ORDER_POS;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            executeResultInfo = m_Accessor->execute( DELETE_PURCHASE_BATCH_ORDER_POS, get() );
        }
        catch( basar::Exception & e  )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, getLogger(), fun, DELETE_PURCHASE_BATCH_ORDER_POS, 
                                                                             __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, DELETE_PURCHASE_BATCH_ORDER_POS, executeResultInfo )
        }
    }

    libutil::domMod::SearchYIteratorPtr PurchaseBatchOrderPosDM::getSearchYIterator()
    {
        METHODNAME_DEF( PurchaseBatchOrderPosDM, getSearchYIterator )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if( NULL == m_SearchYIterator.get() )
        {
            m_SearchYIterator = boost::make_shared< libutil::domMod::SearchYIterator >( properties::BRANCHNO.toDescriptionString() +
                                                                                        properties::ORDERNO.toDescriptionString()  +
                                                                                        properties::ARTICLENO.toDescriptionString()
                                                                                      );
        }

        return m_SearchYIterator;
    }

} // end namespace purchaseBatchOrder
} // end namespace domMod
} // end namespace libabbauw
