#include "pharmacychaincollectiondm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/pharmacychain/pharmacychaincollectionacc_definitions.h>

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

namespace libtender
{
namespace domMod
{
namespace pharmacychain
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
namespace pharmacychain
{
	const libutil::misc::ClassInfo& PharmacyChainCollectionDM::getClassInfo()
    {
	    static const libutil::misc::ClassInfo info( "PharmacyChainCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	    return info;
    }

	PharmacyChainCollectionDM::PharmacyChainCollectionDM()
    : m_Logger( libtender::LoggerPool::getLoggerDomModules() )
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, PharmacyChainCollectionDM )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    }
    
    PharmacyChainCollectionDM::~PharmacyChainCollectionDM()
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, ~PharmacyChainCollectionDM )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    }

	void PharmacyChainCollectionDM::injectPharmacyChainAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor)
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, ~injectPharmacyChainAccessor )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    	m_Accessor = accessor;
    }
    
    const log4cplus::Logger& PharmacyChainCollectionDM::getLogger() const
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, getLogger )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	return m_Logger;
    }

	void PharmacyChainCollectionDM::resetSearchYit()
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, resetSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	getSearchYIterator()->reset();
    }
    
    AccessorPropertyTable_YIterator PharmacyChainCollectionDM::getSearchYit() const
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, getSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	return getSearchYIterator()->get();
    }
    
    void PharmacyChainCollectionDM::setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, setSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	getSearchYit() = yit;
    }

	libutil::domMod::SearchYIteratorPtr PharmacyChainCollectionDM::getSearchYIterator() const
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, getSearchYIterator )
    	BLOG_TRACE_METHOD( getLogger(), fun );

    	if( NULL == m_SearchYIterator.get() )
    	{
    		m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
			new libutil::domMod::SearchYIterator( properties::PHARMACY_CHAIN_NO.toDescriptionString() +
												  properties::CUSTOMER_NO.toDescriptionString() + 
												  properties::PHARMACY_NAME.toDescriptionString() +
												  properties::CUSTOMERGROUPID.toDescriptionString() 
												  ));
    	}
    
    	return m_SearchYIterator;
    }
    
    libutil::infrastructure::accessor::IAccessorPtr PharmacyChainCollectionDM::getPharmacyChainCollectionAccessor() const
    {
    	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
    
    	return m_Accessor;
    }

	void PharmacyChainCollectionDM::findByPattern( AccessorPropertyTable_YIterator yitSearch )
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, findByPattern )
    	BLOG_TRACE_METHOD( getLogger(), fun );

    	using libtender::infrastructure::accessor::pharmacychain::lit::SELECT_PHARMACYCHAIN_BY_PATTERN;
    	
    	basar::db::aspect::ExecuteResultInfo executeResultInfo;
    
    	try
    	{
    		executeResultInfo = m_Accessor->execute( SELECT_PHARMACYCHAIN_BY_PATTERN, yitSearch );
    	}
    	catch( basar::Exception & e )
    	{
    		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PHARMACYCHAIN_BY_PATTERN, __FILE__, __LINE__ );
    	    throw;
    	}
    
    	if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PHARMACYCHAIN_BY_PATTERN, executeResultInfo )
        }
    }
    
	void PharmacyChainCollectionDM::findByPharmacyChainNo( AccessorPropertyTable_YIterator yitSearch )
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, findByPharmacyChainNo )
    	BLOG_TRACE_METHOD( getLogger(), fun );

		if( !yitSearch.isContainedAndSet( properties::PHARMACY_CHAIN_NO ) )
			return;

		using libtender::infrastructure::accessor::pharmacychain::lit::SELECT_PHARMACYCHAIN_BY_PHARMACYCHAINNO;
    	
    	basar::db::aspect::ExecuteResultInfo executeResultInfo;

    	try
    	{
    		executeResultInfo = m_Accessor->execute(SELECT_PHARMACYCHAIN_BY_PHARMACYCHAINNO, yitSearch );
    	}
    	catch( basar::Exception & e )
    	{
    		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PHARMACYCHAIN_BY_PHARMACYCHAINNO, __FILE__, __LINE__ );
    	    throw;
    	}
    
    	if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PHARMACYCHAIN_BY_PHARMACYCHAINNO, executeResultInfo )
        }
    }

	void PharmacyChainCollectionDM::findCustomerGroupIDByPharmacyChain( AccessorPropertyTable_YIterator yitSearch )
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, findCustomerGroupIDByPharmacyChain )
    	BLOG_TRACE_METHOD( getLogger(), fun );

		if( !yitSearch.isContainedAndSet( properties::PHARMACY_CHAIN_NO ) || !yitSearch.isContainedAndSet( properties::CUSTOMERGROUPID )  )
		{
			return;
		}


		using libtender::infrastructure::accessor::pharmacychain::lit::SELECT_CUSTOMERGROUPID_BY_PHARMACYCHAIN;
    	
    	basar::db::aspect::ExecuteResultInfo executeResultInfo;

    	try
    	{
    		executeResultInfo = m_Accessor->execute(SELECT_CUSTOMERGROUPID_BY_PHARMACYCHAIN, yitSearch );
    	}
    	catch( basar::Exception & e )
    	{
    		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_CUSTOMERGROUPID_BY_PHARMACYCHAIN, __FILE__, __LINE__ );
    	    throw;
    	}
    
    	if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_CUSTOMERGROUPID_BY_PHARMACYCHAIN, executeResultInfo )
        }
    }

    basar::db::aspect::AccessorPropertyTableRef PharmacyChainCollectionDM::get() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, get )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    	return m_Accessor->getPropertyTable();
    }
    
    bool PharmacyChainCollectionDM::isEmpty() const
    {
    	METHODNAME_DEF( PharmacyChainCollectionDM, isEmpty )
    	BLOG_TRACE_METHOD( getLogger(), fun );
        
    	if ( 0 == getPharmacyChainCollectionAccessor().get() || 
    		 m_Accessor->getPropertyTable().isNull()   ||
    		 m_Accessor->getPropertyTable().isEmpty() )
    	{
    		return true;
    	}
    
    	return false;
    }

} // end namespace PharmacyChain
} // end namespace domMod
} // end namespace libtender
