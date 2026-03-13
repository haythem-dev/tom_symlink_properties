#include "customerlistdm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/customerlist/customerlistacc_definitions.h>

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

namespace libtender
{
namespace domMod
{
namespace customerlist
{
	using basar::db::aspect::AccessorPropertyTable_YIterator;
	using basar::db::aspect::AccessorPropertyTableRef;
}
}
}

namespace libtender
{
namespace domMod
{
namespace customerlist
{
	const libutil::misc::ClassInfo& CustomerListDM::getClassInfo()
    {
	    static const libutil::misc::ClassInfo info( "CustomerListDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	    return info;
    }

	CustomerListDM::CustomerListDM()
    : m_Logger( libtender::LoggerPool::getLoggerDomModules() )
    {
    	METHODNAME_DEF( CustomerListDM, CustomerListDM )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    }
    
    CustomerListDM::~CustomerListDM()
    {
    	METHODNAME_DEF( CustomerListDM, ~CustomerListDM )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    }

	void CustomerListDM::injectCustomerAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor)
    {
    	METHODNAME_DEF( CustomerListDM, ~injectCustomerAccessor )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    	m_Accessor = accessor;
    }
    
    const log4cplus::Logger& CustomerListDM::getLogger() const
    {
    	METHODNAME_DEF( CustomerListDM, getLogger )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	return m_Logger;
    }

	void CustomerListDM::resetSearchYit()
    {
    	METHODNAME_DEF( CustomerListDM, resetSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	getSearchYIterator()->reset();
    }
    
    AccessorPropertyTable_YIterator CustomerListDM::getSearchYit() const
    {
    	METHODNAME_DEF( CustomerListDM, getSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	return getSearchYIterator()->get();
    }
    
    void CustomerListDM::setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
    	METHODNAME_DEF( CustomerListDM, setSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	getSearchYit() = yit;
    }

	libutil::domMod::SearchYIteratorPtr CustomerListDM::getSearchYIterator() const
    {
    	METHODNAME_DEF( CustomerListDM, getSearchYIterator )
    	BLOG_TRACE_METHOD( getLogger(), fun );

    	if( NULL == m_SearchYIterator.get() )
    	{
    		m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
    		new libutil::domMod::SearchYIterator( properties::CUSTOMER_NO.toDescriptionString() +
			                                      properties::PHARMACY_NAME.toDescriptionString() +
												  properties::BRANCH_NO.toDescriptionString()
												  ));
    	}
    
    	return m_SearchYIterator;
    }
    
    libutil::infrastructure::accessor::IAccessorPtr CustomerListDM::getCustomerCollectionAccessor() const
    {
    	CHECK_INSTANCE_EXCEPTION(m_Accessor.get());
    
    	return m_Accessor;
    }

	basar::db::aspect::AccessorPropertyTable_YIterator CustomerListDM::addEmptyCustomer()
	{
		basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer = get().insert( basar::FOR_INSERT );
		return yitCustomer;
	}

	void CustomerListDM::saveExistingCustomers()
	{
		METHODNAME_DEF( CustomerListDM, saveExistingCustomers )
		BLOG_TRACE_METHOD( getLogger(), fun );

		try
		{
			using libtender::infrastructure::accessor::customerlist::lit::SAVE_CUSTOMER_LIST;
			basar::db::aspect::ExecuteResultInfo executeResultInfo;

			if( !isSavable() )
			{
				return;
			}

			basar::db::aspect::AccessorPropertyTableRef customers = get();
			basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer = customers.begin();

			while( !yitCustomer.isEnd() )
			{
				executeResultInfo = getCustomerCollectionAccessor()->execute( SAVE_CUSTOMER_LIST, yitCustomer );
				++yitCustomer;
			}

			if( executeResultInfo.hasError() )
			{
				THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_CUSTOMER_LIST, executeResultInfo );
			}
		}
		catch( const basar::Exception & e  )
		{
			BLOG_ERROR( getLogger(), e.what().c_str() );
			throw;
		}
	}

	void CustomerListDM::saveDeleteCustomers()
	{
		METHODNAME_DEF( CustomerListDM, saveDeleteCustomers )
		BLOG_TRACE_METHOD( getLogger(), fun );

		try
		{
			using libtender::infrastructure::accessor::customerlist::lit::SAVE_CUSTOMER_LIST;
			basar::db::aspect::ExecuteResultInfo executeResultInfo;

			if( !isSavableForDelete() )
			{
				return;
			}

			basar::db::aspect::AccessorPropertyTableRef customers = get();
			basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer = customers.begin( basar::SS_DELETE );

			while( !yitCustomer.isEnd() )
			{
				executeResultInfo = getCustomerCollectionAccessor()->execute( SAVE_CUSTOMER_LIST, yitCustomer );
				yitCustomer = customers.begin( basar::SS_DELETE );
			}

			if( executeResultInfo.hasError() )
			{
				THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_CUSTOMER_LIST, executeResultInfo );
			}
		}
		catch( const basar::Exception & e  )
		{
			BLOG_ERROR( getLogger(), e.what().c_str() );
			throw;
		}
	}

	void CustomerListDM::findByPattern( AccessorPropertyTable_YIterator yitSearch )
    {
    	METHODNAME_DEF( CustomerListDM, findByPattern )
    	BLOG_TRACE_METHOD( getLogger(), fun );
		
		if( !yitSearch.isContainedAndSet( properties::TENDER_NO ) ||
			!yitSearch.isContainedAndSet( properties::BRANCH_NO ) ||
			!yitSearch.isContainedAndSet( properties::CUSTOMER_NO ) ) return;
		
		using libtender::infrastructure::accessor::customerlist::lit::SELECT_CUSTOMER_BY_PATTERN;
    	basar::db::aspect::ExecuteResultInfo executeResultInfo;
    	
    	try
    	{
    		executeResultInfo = m_Accessor->execute(SELECT_CUSTOMER_BY_PATTERN, yitSearch );
    	}
    	catch( basar::Exception & e )
    	{
    		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_CUSTOMER_BY_PATTERN, __FILE__, __LINE__ );
    	    throw;
    	}
    	
    	if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_CUSTOMER_BY_PATTERN, executeResultInfo )
        }
    }

	void CustomerListDM::findByTenderNo( AccessorPropertyTable_YIterator yitSearch )
	{
		METHODNAME_DEF( CustomerListDM, findByTenderNo )
    	BLOG_TRACE_METHOD( getLogger(), fun );
		
		if( !yitSearch.isContainedAndSet( properties::TENDER_NO ) ) return;
		
		using libtender::infrastructure::accessor::customerlist::lit::SELECT_CUSTOMER_BY_TENDER_NO;
    	basar::db::aspect::ExecuteResultInfo executeResultInfo;
    	
    	try
    	{
	   		executeResultInfo = m_Accessor->execute(SELECT_CUSTOMER_BY_TENDER_NO, yitSearch );
    	}
    	catch( basar::Exception & e )
    	{
    		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_CUSTOMER_BY_TENDER_NO, __FILE__, __LINE__ );
    	    throw;
    	}
    	
    	if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_CUSTOMER_BY_TENDER_NO, executeResultInfo )
        }
	}

	const basar::db::aspect::AccessorPropertyTableRef CustomerListDM::get() const
    {
    	return m_Accessor->getPropertyTable();
    }

	basar::db::aspect::AccessorPropertyTableRef CustomerListDM::get() 
    {
    	return getCustomerCollectionAccessor()->getPropertyTable();
    }
    
    bool CustomerListDM::isEmpty() const
    {
    	METHODNAME_DEF( CustomerListDM, isEmpty )
    	BLOG_TRACE_METHOD( getLogger(), fun );
        
    	if ( 0 == getCustomerCollectionAccessor().get() || 
    		 m_Accessor->getPropertyTable().isNull()    ||
    		 m_Accessor->getPropertyTable().isEmpty() )
    	{
    		return true;
    	}
    
    	return false;
    }

	bool CustomerListDM::isSavable() const
	{
		METHODNAME_DEF( CustomerListDM, isSavable )
    	BLOG_TRACE_METHOD( getLogger(), fun );

		using namespace basar::property;

		PropertyStateSet pss;
		pss.insert( basar::SS_INSERT );
		pss.insert( basar::SS_UPDATE );

		basar::db::aspect::AccessorPropertyTableRef customers = get();
		basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer = customers.begin();

		while( !yitCustomer.isEnd() )
		{
			if( yitCustomer.getState().contains( pss ) )
			{
				return true;
			}

			++yitCustomer;
		}

		return false;
	}

	bool CustomerListDM::isSavableForDelete() const
	{
		METHODNAME_DEF( CustomerListDM, isSavableForDelete )
    	BLOG_TRACE_METHOD( getLogger(), fun );

		using namespace basar::property;

		PropertyStateSet pss;
		pss.insert( basar::SS_DELETE );

		basar::db::aspect::AccessorPropertyTableRef customers = get();
		basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer = customers.begin();

		while( !yitCustomer.isEnd() )
		{
			if( yitCustomer.getState().contains( pss ) )
			{
				return true;
			}

			++yitCustomer;
		}

		return false;
	}

	void CustomerListDM::setTenderNo(basar::Int32 tenderNo)
	{
		basar::db::aspect::AccessorPropertyTableRef customers = get();
		basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer = customers.begin();

        if(!yitCustomer.isNull())
        {
            while(!yitCustomer.isEnd()) 
		    {
		    	yitCustomer.setInt32(properties::TENDER_NO, tenderNo);
		    	++yitCustomer;
		    }
        }
	}

} // end namespace customerlist
} // end namespace domMod
} // end namespace libtender
