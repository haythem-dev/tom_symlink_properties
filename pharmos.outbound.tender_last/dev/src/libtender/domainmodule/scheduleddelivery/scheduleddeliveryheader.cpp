#include "scheduleddeliveryheader.h"
#include <loggerpool/tenderloggerpool.h>
#include <domainmodule/libtender_properties_definitions.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliveryheaderacc_definitions.h>

#include <infrastructure/accessor/scheduleddelivery/scheduleddeliveryheaderacc.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliveryheaderacc_definitions.h>

#include <libbasardbaspect_exceptions.h>
#include <libutil/exception.h>
#include <libutil/validator.h>

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace scheduledDelivery {

//-----------------------------------------------------------------------------------------

ScheduledDeliveryHeader::ScheduledDeliveryHeader() :
	m_Logger( libtender::LoggerPool::getLoggerDomModules() )
{
}

ScheduledDeliveryHeader::~ScheduledDeliveryHeader()
{
}

void ScheduledDeliveryHeader::injectScheduledDeliveryHeaderAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
{
	m_Accessor = accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ScheduledDeliveryHeader::getAccessor() const
{
	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
	return m_Accessor;
}

basar::db::aspect::AccessorPropertyTable_YIterator ScheduledDeliveryHeader::getHeader() const
{
	if( true == isEmpty() )
	{
		createNew();
	}

	checkNumberHeader();
	return getAccessor()->getPropertyTable().begin();
}

const log4cplus::Logger& ScheduledDeliveryHeader::getLogger() const
{
	return m_Logger;
}

void ScheduledDeliveryHeader::findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
{
	METHODNAME_DEF( ScheduledDeliveryHeader, findByKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::scheduledDelivery::lit::SELECT_SCHEDULEDDELIVERY_HEADER_BY_KEY;
	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		getAccessor()->getPropertyTable().clear();
		executeResultInfo = getAccessor()->execute( SELECT_SCHEDULEDDELIVERY_HEADER_BY_KEY, yitSearch );
	}
	catch( basar::Exception& e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_SCHEDULEDDELIVERY_HEADER_BY_KEY, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_SCHEDULEDDELIVERY_HEADER_BY_KEY, executeResultInfo )
	}
}

basar::db::aspect::AccessorPropertyTable_YIterator ScheduledDeliveryHeader::get()
{
	return getHeader();
}

const basar::db::aspect::AccessorPropertyTable_YIterator ScheduledDeliveryHeader::get() const
{
	return getHeader();
}

basar::Int32 ScheduledDeliveryHeader::getPrimaryKey() const
{
	METHODNAME_DEF( ScheduledDeliveryHeader, getPrimaryKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return getHeader().getInt32( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.getName() );
}

bool ScheduledDeliveryHeader::isEmpty() const
{
	if( 0 == m_Accessor.get() ||
		getAccessor()->getPropertyTable().isNull() ||
		getAccessor()->getPropertyTable().isEmpty() )
	{
		return true;
	}

	return false;
}

void ScheduledDeliveryHeader::checkNumberHeader() const
{
	METHODNAME_DEF( ScheduledDeliveryHeader, checkNumberHeader )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( 1 != m_Accessor->getPropertyTable().size() )
	{
		basar::VarString msg( "There's none or more than one headers!" );
		throw libutil::exceptions::InconsistentDataException( basar::ExceptInfo( "ScheduledDeliveryHeader::checkNumberHeader()", msg, __FILE__, __LINE__ ) );
	}
}

bool ScheduledDeliveryHeader::isSavable() const
{
	METHODNAME_DEF( TenderHeader, isSavable )
	BLOG_TRACE_METHOD( getLogger(), fun );

	checkNumberHeader();
	return hasSavableStates();
}

void ScheduledDeliveryHeader::createNew() const
{
	METHODNAME_DEF( ScheduledDeliveryHeader, createNew )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::db::aspect::AccessorPropertyTable_YIterator yitHead = getAccessor()->getPropertyTable().insert( basar::FOR_INSERT );
}

bool ScheduledDeliveryHeader::hasSavableStates() const
{
	METHODNAME_DEF( TenderHeader, hasSavableStates )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using namespace basar::property;

	PropertyStateSet pss;
	pss.insert( basar::SS_INSERT );
	pss.insert( basar::SS_UPDATE );
	pss.insert( basar::SS_DELETE );

	return getHeader().getState().contains( pss );
}

basar::Int32 ScheduledDeliveryHeader::getScheduledDeliveryId() const
{
	return getHeader().getInt32(properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID);
}

basar::Int16 ScheduledDeliveryHeader::getBranchNo() const
{
	return getHeader().getInt16(properties::BRANCH_NO);
}

basar::Int32 ScheduledDeliveryHeader::getTenderId() const
{
	return getHeader().getInt32(properties::TENDER_ID);
}

basar::Int32 ScheduledDeliveryHeader::getCustomerNo() const
{
	return getHeader().getInt32(properties::CUSTOMER_NO);
}

basar::Date ScheduledDeliveryHeader::getDeliveryDate() const
{
	return getHeader().getDate(properties::DELIVERY_DATE);
}

basar::Int32 ScheduledDeliveryHeader::getCscOrderNo() const
{
	return getHeader().getInt32(properties::CSC_ORDER_NO);
}

basar::Date ScheduledDeliveryHeader::getCscOrderDate() const
{
	return getHeader().getDate(properties::CSC_ORDER_DATE);
}

void ScheduledDeliveryHeader::setCscOrderNo(basar::Int32 orderno)
{
	getHeader().setInt32(properties::CSC_ORDER_NO, orderno);
}

void ScheduledDeliveryHeader::setCscOrderDate(basar::Date orderdate)
{
	getHeader().setDate(properties::CSC_ORDER_DATE, orderdate);
}


//-----------------------------------------------------------------------------------------

void ScheduledDeliveryHeader::save()
{
	METHODNAME_DEF( ScheduledDeliveryHeader, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::scheduledDelivery::lit::SAVE_SCHEDULEDDELIVERY_HEADER;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		if( !isSavable() )
		{
			return;
		}

		getAccessor()->clearLastInsertedSerials();

		executeResultInfo = getAccessor()->execute( SAVE_SCHEDULEDDELIVERY_HEADER, getHeader() );

		if( getAccessor()->hasLastInsertedSerial() )
		{
			getHeader().setInt32( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID, static_cast<basar::Int32>(getAccessor()->getLastInsertedSerial()) );
		}
	}
	catch( basar::Exception & e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SAVE_SCHEDULEDDELIVERY_HEADER, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_SCHEDULEDDELIVERY_HEADER, executeResultInfo );
	}
}

} //end namespace scheduledDelivery
} //end namespace domMod
} //end namespace libtender
