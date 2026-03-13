#include "partnercollectiondm.h"
#include <domainmodule/article/businesstypemapper.h>

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/partner/partnercollectionacc_definitions.h>

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

namespace libtender
{
namespace domMod
{
namespace partner
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
namespace partner
{
	const libutil::misc::ClassInfo& PartnerCollectionDM::getClassInfo()
    {
	    static const libutil::misc::ClassInfo info( "PartnerCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	    return info;
    }

	PartnerCollectionDM::PartnerCollectionDM()
    : m_Logger( libtender::LoggerPool::getLoggerDomModules() )
    {
    	METHODNAME_DEF( CustomerCollectionDM, CustomerCollectionDM )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    }
    
    PartnerCollectionDM::~PartnerCollectionDM()
    {
    	METHODNAME_DEF( CustomerCollectionDM, ~CustomerCollectionDM )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    }

	void PartnerCollectionDM::injectPartnerAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor)
    {
    	METHODNAME_DEF( CustomerCollectionDM, ~injectCustomerAccessor )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    	m_Accessor = accessor;
    }
    
    const log4cplus::Logger& PartnerCollectionDM::getLogger() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, getLogger )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	return m_Logger;
    }

	void PartnerCollectionDM::resetSearchYit()
    {
    	METHODNAME_DEF( CustomerCollectionDM, resetSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	getSearchYIterator()->reset();
    }
    
    AccessorPropertyTable_YIterator PartnerCollectionDM::getSearchYit() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, getSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	return getSearchYIterator()->get();
    }
    
    void PartnerCollectionDM::setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
    	METHODNAME_DEF( CustomerCollectionDM, setSearchYit )
    	BLOG_TRACE_METHOD( m_Logger, fun );
    
    	getSearchYit() = yit;
    }

	libutil::domMod::SearchYIteratorPtr PartnerCollectionDM::getSearchYIterator() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, getSearchYIterator )
    	BLOG_TRACE_METHOD( getLogger(), fun );

    	if( NULL == m_SearchYIterator.get() )
    	{
    		m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
    		new libutil::domMod::SearchYIterator( properties::PARTNER_NO.toDescriptionString() +
			                                      properties::PARTNER_NAME.toDescriptionString() +
												  properties::TENDER_TYPE.toDescriptionString() + 
												  properties::BUSINESS_TYPE.toDescriptionString() 
												  ));
    	}
    
    	return m_SearchYIterator;
    }
    
    libutil::infrastructure::accessor::IAccessorPtr PartnerCollectionDM::getPartnerCollectionAccessor() const
    {
    	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
    
    	return m_Accessor;
    }

	void PartnerCollectionDM::findByPattern( AccessorPropertyTable_YIterator yitSearch )
    {
    	METHODNAME_DEF( CustomerCollectionDM, findByPattern )
    	BLOG_TRACE_METHOD( getLogger(), fun );

		if( yitSearch.isContainedAndSet( properties::TENDER_TYPE ) )
		{
			tender::TenderTypeEnum tt = static_cast<tender::TenderTypeEnum>(yitSearch.getInt32(properties::TENDER_TYPE));
			article::BusinessTypeEnum bt = article::BusinessTypeMapper::getBusinessType(tt);
			if (article::BT_NONE != bt)
			{
				yitSearch.setInt16(properties::BUSINESS_TYPE, static_cast<basar::Int16>(bt));
			}
		}

    	using libtender::infrastructure::accessor::partner::lit::SELECT_PARTNER_BY_PATTERN;
    	
    	basar::db::aspect::ExecuteResultInfo executeResultInfo;
    
    	try
    	{
    		executeResultInfo = m_Accessor->execute( SELECT_PARTNER_BY_PATTERN, yitSearch );
    	}
    	catch( basar::Exception & e )
    	{
    		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PARTNER_BY_PATTERN, __FILE__, __LINE__ );
    	    throw;
    	}
    	if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PARTNER_BY_PATTERN, executeResultInfo )
        }
    
    }
    
	void PartnerCollectionDM::findByPartnerNo( AccessorPropertyTable_YIterator yitSearch )
    {
    	METHODNAME_DEF( CustomerCollectionDM, findByPartnerNo )
    	BLOG_TRACE_METHOD( getLogger(), fun );

		if( !yitSearch.isContainedAndSet( properties::PARTNER_NO ) )
			return;

		if( yitSearch.isContainedAndSet( properties::TENDER_TYPE ) )
		{
			tender::TenderTypeEnum tt = static_cast<tender::TenderTypeEnum>(yitSearch.getInt32(properties::TENDER_TYPE));
			article::BusinessTypeEnum bt = article::BusinessTypeMapper::getBusinessType(tt);
			if (article::BT_NONE != bt)
			{
				yitSearch.setInt16(properties::BUSINESS_TYPE, static_cast<basar::Int16>(bt));
			}
		}

		using libtender::infrastructure::accessor::partner::lit::SELECT_PARTNER_BY_PARTNER_NO;
    	
    	basar::db::aspect::ExecuteResultInfo executeResultInfo;

    	try
    	{
    		executeResultInfo = m_Accessor->execute(SELECT_PARTNER_BY_PARTNER_NO, yitSearch );
    	}
    	catch( basar::Exception & e )
    	{
    		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PARTNER_BY_PARTNER_NO, __FILE__, __LINE__ );
    	    throw;
    	}
    
    	if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PARTNER_BY_PARTNER_NO, executeResultInfo )
        }
    }

    basar::db::aspect::AccessorPropertyTableRef PartnerCollectionDM::get() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, get )
    	BLOG_TRACE_METHOD( getLogger(), fun );
    
    	return m_Accessor->getPropertyTable();
    }
    
    bool PartnerCollectionDM::isEmpty() const
    {
    	METHODNAME_DEF( CustomerCollectionDM, isEmpty )
    	BLOG_TRACE_METHOD( getLogger(), fun );
        
    	if ( 0 == getPartnerCollectionAccessor().get() || 
    		 m_Accessor->getPropertyTable().isNull()   ||
    		 m_Accessor->getPropertyTable().isEmpty() )
    	{
    		return true;
    	}
    
    	return false;
    }

} // end namespace partner
} // end namespace domMod
} // end namespace libtender
