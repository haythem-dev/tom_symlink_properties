#include "customercollectiondm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/customer/customercollectionacc_definitions.h>

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

namespace libtender
{
namespace domMod
{
namespace customer
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
namespace customer
{
	const libutil::misc::ClassInfo& CustomerCollectionDM::getClassInfo()
    {
	    static const libutil::misc::ClassInfo info( "CustomerCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	    return info;
    }

	CustomerCollectionDM::CustomerCollectionDM()
    : m_Logger( libtender::LoggerPool::getLoggerDomModules() )
    {
    	METHODNAME_DEF( CustomerCollectionDM, CustomerCollectionDM )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    }
    
    CustomerCollectionDM::~CustomerCollectionDM()
    {
    	METHODNAME_DEF( CustomerCollectionDM, ~CustomerCollectionDM )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    }

	void CustomerCollectionDM::injectCustomerAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor)
    {
    	METHODNAME_DEF( CustomerCollectionDM, ~injectCustomerAccessor )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    	m_Accessor = accessor;
    }
    
    const log4cplus::Logger& CustomerCollectionDM::getLogger() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, getLogger )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	return m_Logger;
    }

	void CustomerCollectionDM::resetSearchYit()
    {
    	METHODNAME_DEF( CustomerCollectionDM, resetSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	getSearchYIterator()->reset();
    }
    
    AccessorPropertyTable_YIterator CustomerCollectionDM::getSearchYit() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, getSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	return getSearchYIterator()->get();
    }
    
    void CustomerCollectionDM::setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
    	METHODNAME_DEF( CustomerCollectionDM, setSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	getSearchYit() = yit;
    }

	libutil::domMod::SearchYIteratorPtr CustomerCollectionDM::getSearchYIterator() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, getSearchYIterator )
    	BLOG_TRACE_METHOD( getLogger(), fun );

    	if( NULL == m_SearchYIterator.get() )
    	{
    		m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
    		new libutil::domMod::SearchYIterator( properties::CUSTOMER_NO.toDescriptionString() +
			                                      properties::PHARMACY_NAME.toDescriptionString() +
												  properties::BRANCH_NO.toDescriptionString() +
                                                  properties::ARTICLE_CODE.toDescriptionString()+
                                                  properties::PHARMACY_GROUPID.toDescriptionString()
												  ));
    	}
    
    	return m_SearchYIterator;
    }
    
    libutil::infrastructure::accessor::IAccessorPtr CustomerCollectionDM::getCustomerCollectionAccessor() const
    {
    	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
    
    	return m_Accessor;
    }

	void CustomerCollectionDM::findByPattern( AccessorPropertyTable_YIterator yitSearch )
    {
    	METHODNAME_DEF( CustomerCollectionDM, findByPattern )
    	BLOG_TRACE_METHOD( getLogger(), fun );

		if( !yitSearch.isContainedAndSet( properties::CUSTOMER_NO ) &&
			!yitSearch.isContainedAndSet( properties::PHARMACY_NAME ) ) return;

    	using libtender::infrastructure::accessor::customer::lit::SELECT_CUSTOMER_BY_PATTERN;
    	
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
    
	void CustomerCollectionDM::findByCustomerNo( AccessorPropertyTable_YIterator yitSearch )
    {
    	METHODNAME_DEF( CustomerCollectionDM, findByCustomerNo )
    	BLOG_TRACE_METHOD( getLogger(), fun );

		if( !yitSearch.isContainedAndSet( properties::CUSTOMER_NO ) )
			return;

    	using libtender::infrastructure::accessor::customer::lit::SELECT_CUSTOMER_BY_CUSTOMER_NO;
    	
    	basar::db::aspect::ExecuteResultInfo executeResultInfo;
    
    	try
    	{
    		executeResultInfo = m_Accessor->execute(SELECT_CUSTOMER_BY_CUSTOMER_NO, yitSearch );
    	}
    	catch( basar::Exception & e )
    	{
    		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_CUSTOMER_BY_CUSTOMER_NO, __FILE__, __LINE__ );
    	    throw;
    	}
    
    	if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_CUSTOMER_BY_CUSTOMER_NO, executeResultInfo )
        }
    }

    void CustomerCollectionDM::findCustomerByPharmacyGroupId(basar::db::aspect::AccessorPropertyTable_YIterator yit)
    {
        METHODNAME_DEF( CustomerCollectionDM, findCustomerByPharmacyGroupId )
        BLOG_TRACE_METHOD( getLogger(), fun );

        using libtender::infrastructure::accessor::customer::lit::SELECT_CUSTOMER_BY_PHARMACY_GROUP_ID;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            m_Accessor->getPropertyTable().clear();
            executeResultInfo = m_Accessor->execute( SELECT_CUSTOMER_BY_PHARMACY_GROUP_ID, yit );
        }
        catch( basar::Exception& e )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_CUSTOMER_BY_PHARMACY_GROUP_ID, __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_CUSTOMER_BY_PHARMACY_GROUP_ID, executeResultInfo )
        }
    }

	basar::db::aspect::AccessorPropertyTableRef CustomerCollectionDM::get() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, get )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    	return m_Accessor->getPropertyTable();
    }
    
    bool CustomerCollectionDM::isEmpty() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, isEmpty )
    	BLOG_TRACE_METHOD( getLogger(), fun );
        
    	if ( 0 == getCustomerCollectionAccessor().get() || 
    		 m_Accessor->getPropertyTable().isNull()    ||
    		 m_Accessor->getPropertyTable().isEmpty() )
    	{
    		return true;
    	}
    
    	return false;
    }

} // end namespace customer
} // end namespace domMod
} // end namespace libtender
