#include "articlecollectiondm.h"
#include "businesstypemapper.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/article/articlecollectionacc_definitions.h>

#include <domainmodule/constants.h>

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

namespace libtender
{
namespace domMod
{
namespace article
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
namespace article
{
const libutil::misc::ClassInfo& ArticleCollectionDM::getClassInfo()
{
	static const libutil::misc::ClassInfo info( "ArticleCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	return info;
}

ArticleCollectionDM::ArticleCollectionDM()
: m_Logger( libtender::LoggerPool::getLoggerDomModules() )
{
	METHODNAME_DEF( ArticleCollectionDM, ArticelDM )
	BLOG_TRACE_METHOD( getLogger(), fun );

}

ArticleCollectionDM::~ArticleCollectionDM()
{
	METHODNAME_DEF( ArticleCollectionDM, ~ArticelDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void ArticleCollectionDM::injectArticleAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor)
{
	METHODNAME_DEF( ArticleCollectionDM, ~injectArticleAccessor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	m_Accessor = accessor;
}

const log4cplus::Logger& ArticleCollectionDM::getLogger() const
{
	METHODNAME_DEF( ArticleCollectionDM, getLogger )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_Logger;
}

void ArticleCollectionDM::setAreaID(basar::Int16 areaID)
{
	m_AreaID = areaID;
}

void ArticleCollectionDM::resetSearchYit()
{
	METHODNAME_DEF( ArticleCollectionDM, resetSearchYit )
	BLOG_TRACE_METHOD( m_Logger, fun );

	getSearchYIterator()->reset();
}

AccessorPropertyTable_YIterator ArticleCollectionDM::getSearchYit() const
{
	METHODNAME_DEF( ArticleCollectionDM, getSearchYit )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return getSearchYIterator()->get();
}

void ArticleCollectionDM::setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	METHODNAME_DEF( ArticleCollectionDM, setSearchYit )
	BLOG_TRACE_METHOD( m_Logger, fun );

	getSearchYit() = yit;
}

libutil::domMod::SearchYIteratorPtr ArticleCollectionDM::getSearchYIterator() const
{
	METHODNAME_DEF( ArticleCollectionDM, getSearchYIterator )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( NULL == m_SearchYIterator.get() )
	{
		m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
		new libutil::domMod::SearchYIterator( properties::ARTICLE_CODE.toDescriptionString() +
		                                      properties::ARTICLE_NAME.toDescriptionString() +
											  properties::TENDER_TYPE.toDescriptionString()  +
											  properties::BUSINESS_TYPE.toDescriptionString() ));
	}

	return m_SearchYIterator;
}

libutil::infrastructure::accessor::IAccessorPtr ArticleCollectionDM::getArticleCollectionAccessor() const
{
	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );

	return m_Accessor;
}

void ArticleCollectionDM::findByPattern( AccessorPropertyTable_YIterator yitSearch )
{
	METHODNAME_DEF( ArticleCollectionDM, findByPattern )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( !yitSearch.isContainedAndSet( properties::ARTICLE_CODE ) &&
		!yitSearch.isContainedAndSet( properties::ARTICLE_NAME ) ) return;

	if (yitSearch.isContainedAndSet(properties::TENDER_TYPE))
	{
		tender::TenderTypeEnum tt = static_cast<tender::TenderTypeEnum>(yitSearch.getInt32(properties::TENDER_TYPE));
		article::BusinessTypeEnum bt = BusinessTypeMapper::getBusinessType(tt);
		if (article::BT_NONE != bt)
		{
			yitSearch.setInt16(properties::BUSINESS_TYPE, static_cast<basar::Int16>(bt));
		}
	}

	basar::db::aspect::ExecuteResultInfo executeResultInfo;

	try
	{

        using libtender::infrastructure::accessor::article::lit::SELECT_ARTICLE_BY_PATTERN;
        using libtender::infrastructure::accessor::article::lit::SELECT_ARTICLE_BY_PATTERN_RS;
        if( m_AreaID == constants::REGION_RS )
        {

            executeResultInfo = m_Accessor->execute(SELECT_ARTICLE_BY_PATTERN_RS, yitSearch );
        }
        else
        {
            executeResultInfo = m_Accessor->execute(SELECT_ARTICLE_BY_PATTERN, yitSearch );
        }
	}
	catch( basar::Exception & e )
	{   
        if( m_AreaID == constants::REGION_RS )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, libtender::infrastructure::accessor::article::lit::SELECT_ARTICLE_BY_PATTERN_RS, __FILE__, __LINE__ );
        }
        else
        {
		    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, libtender::infrastructure::accessor::article::lit::SELECT_ARTICLE_BY_PATTERN, __FILE__, __LINE__ );
        }
	    throw;
	}

	if( executeResultInfo.hasError() )
    {
            if( m_AreaID == constants::REGION_RS )
            {
                THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, libtender::infrastructure::accessor::article::lit::SELECT_ARTICLE_BY_PATTERN_RS, executeResultInfo )
            }
            else
            {
                THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, libtender::infrastructure::accessor::article::lit::SELECT_ARTICLE_BY_PATTERN, executeResultInfo )
            }
    }


}

void ArticleCollectionDM::findByArticleCode( AccessorPropertyTable_YIterator yitSearch )
{
	METHODNAME_DEF( ArticleCollectionDM, findByArticleCode )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( !yitSearch.isContainedAndSet( properties::ARTICLE_CODE ) )
		return;

	if (yitSearch.isContainedAndSet(properties::TENDER_TYPE))
	{
		tender::TenderTypeEnum tt = static_cast<tender::TenderTypeEnum>(yitSearch.getInt32(properties::TENDER_TYPE));
		article::BusinessTypeEnum bt =  BusinessTypeMapper::getBusinessType(tt);
		if (article::BT_NONE != bt)
		{
			yitSearch.setInt16(properties::BUSINESS_TYPE, static_cast<basar::Int16>(bt));
		}
	}

	using libtender::infrastructure::accessor::article::lit::SELECT_ARTICLE_BY_ARTICLE_CODE;

	using libtender::infrastructure::accessor::article::lit::SELECT_ARTICLE_BY_ARTICLE_CODE_RS;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;

	try
	{
        if( m_AreaID == constants::REGION_RS )
        {

            executeResultInfo = m_Accessor->execute(SELECT_ARTICLE_BY_ARTICLE_CODE_RS, yitSearch );
        }
        else
        {
            executeResultInfo = m_Accessor->execute(SELECT_ARTICLE_BY_ARTICLE_CODE, yitSearch );
        }
	}
	catch( basar::Exception & e )
	{
        if( m_AreaID == constants::REGION_RS )
        {

		    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_ARTICLE_BY_ARTICLE_CODE_RS, __FILE__, __LINE__ );
	        throw;
        }
        else
        {
		    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_ARTICLE_BY_ARTICLE_CODE, __FILE__, __LINE__ );

	        throw;
        }

	}

	if( executeResultInfo.hasError() )
    {
        if( m_AreaID == constants::REGION_RS )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_ARTICLE_BY_ARTICLE_CODE_RS, executeResultInfo )
        }
        else
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_ARTICLE_BY_ARTICLE_CODE, executeResultInfo )
        }
    }
}

basar::db::aspect::AccessorPropertyTableRef ArticleCollectionDM::get() const
{
	METHODNAME_DEF( ArticleCollectionDM, get )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return m_Accessor->getPropertyTable();
}

bool ArticleCollectionDM::isEmpty() const
{
	METHODNAME_DEF( ArticleCollectionDM, isEmpty )
	BLOG_TRACE_METHOD( getLogger(), fun );
    
	if ( 0 == getArticleCollectionAccessor().get() || 
		 m_Accessor->getPropertyTable().isNull()   ||
		 m_Accessor->getPropertyTable().isEmpty() )
	{
		return true;
	}

	return false;
}

} //end namespace article
} //end namespace domMod
} //end namespace libtender
