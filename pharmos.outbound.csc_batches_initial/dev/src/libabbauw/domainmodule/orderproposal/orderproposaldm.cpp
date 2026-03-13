//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "orderproposaldm.h"

#include "infrastructure/accessor/orderproposal/orderproposalacc.h"
#include "infrastructure/accessor/orderproposal/orderproposalacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

#include <libutil/exception.h>
#include <libutil/util.h>

#include <boost/make_shared.hpp>

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
    const libutil::misc::ClassInfo & OrderProposalDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "OrderProposalDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

    OrderProposalDM::OrderProposalDM()
		: m_Logger( LoggerPool::loggerOrderProposal )
	{
        METHODNAME_DEF( OrderProposalDM, OrderProposalDM )
        BLOG_TRACE_METHOD( getLogger(), fun );
	}

	OrderProposalDM::~OrderProposalDM()
	{
        METHODNAME_DEF( OrderProposalDM, ~OrderProposalDM )
        BLOG_TRACE_METHOD( getLogger(), fun );
	}

    const log4cplus::Logger& OrderProposalDM::getLogger() const
    {
        return m_Logger;
    }

    void OrderProposalDM::injectOrderProposalAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
    {
        m_Accessor = accessor;
    }

    libutil::infrastructure::accessor::IAccessorPtr OrderProposalDM::getAccessor() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
        return m_Accessor;
    }

    basar::db::aspect::AccessorPropertyTable_YIterator OrderProposalDM::get()
	{
        const OrderProposalDM& me = *this; // force use of const method
        return me.get();
	}

    basar::db::aspect::AccessorPropertyTable_YIterator OrderProposalDM::get() const
	{
        // get order proposal or create new one if not existing
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

    bool OrderProposalDM::isEmpty() const
    {
        return getAccessor()->getPropertyTable().isEmpty();
    }

	basar::db::aspect::AccessorPropertyTable_YIterator OrderProposalDM::getSearchYit()
	{
		return getSearchYIterator()->get();
	}

	void OrderProposalDM::resetSearchYit()
	{
        getSearchYIterator()->reset();
	}
    
    void OrderProposalDM::findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( OrderProposalDM, findByKey )
        BLOG_TRACE_METHOD(getLogger(), fun );

        using libabbauw::infrastructure::accessor::orderProposal::lit::SELECT_ORDER_PROPOSAL_BY_KEY;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            executeResultInfo = m_Accessor->execute( SELECT_ORDER_PROPOSAL_BY_KEY, yitSearch );
        }
        catch( basar::Exception & e  )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, getLogger(), fun, SELECT_ORDER_PROPOSAL_BY_KEY, __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_ORDER_PROPOSAL_BY_KEY, executeResultInfo )
        }
    }

    void OrderProposalDM::setOrderNo( basar::Int32 orderNo )
    {
        get().setInt32( properties::ORDERNO_UPDATE, orderNo );
    }

    void OrderProposalDM::save()
    {
        METHODNAME_DEF( OrderProposalDM, save )
        BLOG_TRACE_METHOD(getLogger(), fun );

        if( isEmpty() )
        {
            return;
        }

        using libabbauw::infrastructure::accessor::orderProposal::lit::SAVE_ORDER_PROPOSAL;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            executeResultInfo = m_Accessor->execute( SAVE_ORDER_PROPOSAL, get() );
        }
        catch( basar::Exception & e  )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, getLogger(), fun, SAVE_ORDER_PROPOSAL, __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_ORDER_PROPOSAL, executeResultInfo )
        }
    }

    void OrderProposalDM::erase()
    {
        METHODNAME_DEF( OrderProposalDM, erase )
        BLOG_TRACE_METHOD(getLogger(), fun );

        if( isEmpty() )
        {
            return;
        }

        using libabbauw::infrastructure::accessor::orderProposal::lit::DELETE_ORDER_PROPOSAL;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            executeResultInfo = m_Accessor->execute( DELETE_ORDER_PROPOSAL, get() );
        }
        catch( basar::Exception & e  )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, getLogger(), fun, DELETE_ORDER_PROPOSAL, __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, DELETE_ORDER_PROPOSAL, executeResultInfo )
        }
    }

    void OrderProposalDM::clear()
    {
        get().clear();
    }

    libutil::domMod::SearchYIteratorPtr OrderProposalDM::getSearchYIterator()
    {
        METHODNAME_DEF( OrderProposalDM, getSearchYIterator )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if( NULL == m_SearchYIterator.get() )
        {
            m_SearchYIterator = boost::make_shared< libutil::domMod::SearchYIterator >( properties::BRANCHNO.toDescriptionString() +
                                                                                        properties::ORDERNO.toDescriptionString()  +
                                                                                        properties::POSNO.toDescriptionString()
                                                                                      );
        }

        return m_SearchYIterator;
    }

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw
