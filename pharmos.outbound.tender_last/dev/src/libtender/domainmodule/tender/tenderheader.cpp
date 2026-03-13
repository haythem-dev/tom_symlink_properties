#include "tenderheader.h"

#include "tendertypeenum.h"
#include <loggerpool/tenderloggerpool.h>
#include <domainmodule/libtender_properties_definitions.h>
#include <infrastructure/accessor/tender/tenderheaderacc_definitions.h>

#include <libbasardbaspect_exceptions.h>
#include <libutil/exception.h>
#include <libutil/validator.h>

namespace libtender
{
namespace domMod
{
namespace tender
{
	using basar::db::aspect::AccessorPropertyTable_YIterator;
}
}
}

namespace libtender
{
namespace domMod
{
namespace tender
{
const libutil::misc::ClassInfo& TenderHeader::getClassInfo()
{
	static const libutil::misc::ClassInfo info( "TenderHeader", libutil::misc::Version( 1, 0, 0, 0 ) );
	return info;
}

TenderHeader::TenderHeader()
: m_Logger( libtender::LoggerPool::getLoggerDomModules() )
{
}

TenderHeader::~TenderHeader()
{
}

void TenderHeader::injectTenderHeaderAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
{
	m_Accessor = accessor;
}

libutil::infrastructure::accessor::IAccessorPtr TenderHeader::getAccessor() const
{
	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
	return m_Accessor;
}

//void TenderHeader::injectValidator( libutil::misc::IValidatorPtr validator )
//{
//	m_Validator = validator;
//}

//libutil::misc::IValidatorPtr TenderHeader::getValidator() const
//{
//    CHECK_INSTANCE_EXCEPTION( m_Validator.get() );
//    return m_Validator;
//}

basar::db::aspect::AccessorPropertyTable_YIterator TenderHeader::get()
{
	return getHeader();
}

const basar::db::aspect::AccessorPropertyTable_YIterator TenderHeader::get() const
{
	return getHeader();
}

basar::db::aspect::AccessorPropertyTable_YIterator TenderHeader::getHeader() const
{
	if( true == isEmpty() )
	{
		createNew();
	}

	checkNumberHeader();
	return getAccessor()->getPropertyTable().begin();
}

basar::Int32 TenderHeader::getPrimaryKey() const
{
	return getHeader().getInt32( properties::TENDER_NO.getName() );
}

bool TenderHeader::isEmpty() const
{
	if(    0 == m_Accessor.get()
		|| getAccessor()->getPropertyTable().isNull()
		|| getAccessor()->getPropertyTable().isEmpty() )
	{
		return true;
	}

	return false;
}

void TenderHeader::validate() const
{
	//getValidator()->validate( getHeader() );
}

bool TenderHeader::isValid() const
{
	//return getValidator()->isValid();
	return true;
}

TenderStateEnum TenderHeader::getState() const
{
	return static_cast<TenderStateEnum>( getHeader().getInt16( properties::TENDER_STATE ) );
}

void TenderHeader::setState( const TenderStateEnum state )
{
	getHeader().setInt16( properties::TENDER_STATE, static_cast<basar::Int16>( state ) );
}

const log4cplus::Logger& TenderHeader::getLogger() const
{
	return m_Logger;
}

bool TenderHeader::isSavable() const
{
	METHODNAME_DEF( TenderHeader, isSavable )
	BLOG_TRACE_METHOD( getLogger(), fun );

	checkNumberHeader();
	return hasSavableStates();
}

bool TenderHeader::hasSavableStates() const
{
	METHODNAME_DEF( TenderHeader, hasSavableStates )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using namespace basar::property;

	//Because of serial
	basar::db::aspect::AccessorPropertyTable_XIterator xit = getHeader().begin( basar::SS_INSERT );
	int insert = 0;
	while( !xit.isEnd() )
	{
		++insert;
		++xit;
	}

	PropertyStateSet pss;

	pss.insert( basar::SS_UPDATE );
	pss.insert( basar::SS_DELETE );

	if( getHeader().getState().contains( pss ) ||
		1 < insert )
	{
		return true;
	}

	return false ;
}

void TenderHeader::checkNumberHeader() const
{
	METHODNAME_DEF( TenderHeader, checkNumberHeader )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( 1 != getAccessor()->getPropertyTable().size() )
	{
		basar::VarString msg( "There's none or more than one headers!" );
		throw libutil::exceptions::InconsistentDataException( basar::ExceptInfo( "TenderHeader::checkNumberHeader()", msg, __FILE__, __LINE__ ) );
	}
}

void TenderHeader::save()
{
	METHODNAME_DEF( TenderHeader, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::tender::lit::SAVE_TENDER_HEADER;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		validate();

		if( !isSavable() )
		{
			return;
		}

		getAccessor()->clearLastInsertedSerials();
		executeResultInfo = getAccessor()->execute( SAVE_TENDER_HEADER, getHeader() );

		if( getAccessor()->hasLastInsertedSerial() )
		{
			getHeader().setInt32( properties::TENDER_NO, static_cast<basar::Int32>(getAccessor()->getLastInsertedSerial()) );
		}
	}
	catch( basar::Exception & e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SAVE_TENDER_HEADER, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_TENDER_HEADER, executeResultInfo );
	}
}

void TenderHeader::createNew() const
{
	basar::db::aspect::AccessorPropertyTable_YIterator yitHead = getAccessor()->getPropertyTable().insert( basar::FOR_INSERT );

	yitHead.setInt16( properties::TENDER_STATE, 3 );
	yitHead.setInt32( properties::TENDER_TYPE.getName(), 1 );

	yitHead.setDecimal( properties::EXCEEDANCE_PCT, basar::Decimal() );
	yitHead.setInt32( properties::PHARMACY_CHAIN_NO.getName(), 0 );
}

void TenderHeader::findByKey( const AccessorPropertyTable_YIterator yitHeader )
{
	METHODNAME_DEF( TenderHeader, findByKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::tender::lit::SELECT_TENDER_HEADER_BY_KEY;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		getAccessor()->getPropertyTable().clear();
		executeResultInfo = getAccessor()->execute( SELECT_TENDER_HEADER_BY_KEY, yitHeader, true, true, basar::db::aspect::HOLD_RECORDSET );
	}
	catch( basar::Exception & e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_TENDER_HEADER_BY_KEY, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDER_HEADER_BY_KEY, executeResultInfo )
	}
}

} // end namespace tender
} // end namespace domMod
} // end namespace libtender
