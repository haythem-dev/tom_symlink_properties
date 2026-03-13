#include "articlepricedm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/article/articlepriceacc_definitions.h>

#include <domainmodule/constants.h>

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>


namespace libtender {
namespace domMod {
namespace article {
	using basar::db::aspect::AccessorPropertyTable_YIterator;
	using basar::db::aspect::AccessorPropertyTableRef;
}
}
}

namespace libtender {
namespace domMod {
namespace article {

const libutil::misc::ClassInfo& ArticlePriceDM::getClassInfo()
{
	static const libutil::misc::ClassInfo info( "ArticlePriceDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	return info;
}

ArticlePriceDM::ArticlePriceDM()
: m_Logger( libtender::LoggerPool::getLoggerDomModules() )
{
	METHODNAME_DEF( ArticlePriceDM, ArticelDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

ArticlePriceDM::~ArticlePriceDM()
{
	METHODNAME_DEF( ArticlePriceDM, ~ArticelDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void ArticlePriceDM::injectArticlePriceAccessor(libutil::infrastructure::accessor::IAccessorPtr accessor)
{
	METHODNAME_DEF( ArticlePriceDM, injectArticlePriceAccessor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	m_Accessor = accessor;
}

void ArticlePriceDM::setAreaID(basar::Int16 areaID)
{
	m_AreaID = areaID;
}

const log4cplus::Logger& ArticlePriceDM::getLogger() const
{
	METHODNAME_DEF( ArticlePriceDM, getLogger )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_Logger;
}

libutil::infrastructure::accessor::IAccessorPtr ArticlePriceDM::getArticlePriceAccessor() const
{
	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );

	return m_Accessor;
}

void ArticlePriceDM::findByArticleCode(AccessorPropertyTable_YIterator yitPos)
{
	METHODNAME_DEF( ArticlePriceDM, findByArticleCode )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( !yitPos.isContainedAndSet( properties::ARTICLE_CODE ) )
	{
		return;
	}

	if( m_AreaID == constants::REGION_BG )
	{
		findByArticleCodeBG(yitPos);
	}
	else
	{
		findByArticleCodeOtherCountries(yitPos);
	}
}

void ArticlePriceDM::findByArticleCodeOtherCountries(AccessorPropertyTable_YIterator yitPos)
{
	METHODNAME_DEF( ArticlePriceDM, findByArticleCodeOtherCountries )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::article::lit::SELECT_PRICE_BY_ARTICLECODE;
	
	basar::db::aspect::ExecuteResultInfo executeResultInfo;

	try
	{
		executeResultInfo = m_Accessor->execute(SELECT_PRICE_BY_ARTICLECODE, yitPos );
	}
	catch( basar::Exception & e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PRICE_BY_ARTICLECODE, __FILE__, __LINE__ );
	    throw;
	}

	if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PRICE_BY_ARTICLECODE, executeResultInfo )
    }
}

void ArticlePriceDM::findByArticleCodeBG(AccessorPropertyTable_YIterator yitPos)
{
	METHODNAME_DEF( ArticlePriceDM, findByArticleCodeBG )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::article::lit::SELECT_PRICE_BY_ARTICLECODE_BG;
	
	basar::db::aspect::ExecuteResultInfo executeResultInfo;

	try
	{
		executeResultInfo = m_Accessor->execute(SELECT_PRICE_BY_ARTICLECODE_BG, yitPos);
	}
	catch( basar::Exception & e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PRICE_BY_ARTICLECODE_BG, __FILE__, __LINE__ );
	    throw;
	}

	if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PRICE_BY_ARTICLECODE_BG, executeResultInfo )
    }
}

basar::db::aspect::AccessorPropertyTableRef ArticlePriceDM::get() const
{
	METHODNAME_DEF( ArticlePriceDM, get )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return m_Accessor->getPropertyTable();
}

bool ArticlePriceDM::isEmpty() const
{
	METHODNAME_DEF( ArticlePriceDM, isEmpty )
	BLOG_TRACE_METHOD( getLogger(), fun );
    
	if ( 0 == getArticlePriceAccessor().get() || 
		 m_Accessor->getPropertyTable().isNull()   ||
		 m_Accessor->getPropertyTable().isEmpty() )
	{
		return true;
	}

	return false;
}

} 
} 
} 
