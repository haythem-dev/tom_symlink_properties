#include "scheduleddeliverypositioncollection.h"
#include <loggerpool/tenderloggerpool.h>
#include <domainmodule/libtender_properties_definitions.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverypositionsacc_definition.h>
#include <infrastructure/purchasedbconn/ipurchasedbparameterproxy.h>

#include <libbasardbaspect.h>
#include <libbasardbsql.h>
#include <libutil/exception.h>

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace scheduledDelivery {

//-----------------------------------------------------------------------------------------

ScheduledDeliveryPositionCollection::ScheduledDeliveryPositionCollection() :
	m_Logger( libtender::LoggerPool::getLoggerDomModules() )
{
}

ScheduledDeliveryPositionCollection::~ScheduledDeliveryPositionCollection()
{
}

void ScheduledDeliveryPositionCollection::injectScheduledDeliveryPositionCollectionAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
{
	m_Accessor = accessor;
}

void ScheduledDeliveryPositionCollection::injectPurchaseDBParameterProxy( infrastructure::db::IPurchaseDBParameterProxyPtr proxy )
{
	m_PurchaseDBParameterProxy = proxy;
}

basar::db::aspect::AccessorPropertyTableRef ScheduledDeliveryPositionCollection::get()
{
	return getAccessor()->getPropertyTable();
}

const basar::db::aspect::AccessorPropertyTableRef ScheduledDeliveryPositionCollection::get() const
{
	return getAccessor()->getPropertyTable();
}

libutil::infrastructure::accessor::IAccessorPtr ScheduledDeliveryPositionCollection::getAccessor() const
{
	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
	return m_Accessor;
}

const log4cplus::Logger & ScheduledDeliveryPositionCollection::getLogger() const
{
	return m_Logger;
}

basar::db::aspect::AccessorPropertyTable_YIterator ScheduledDeliveryPositionCollection::addEmptyPosition()
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, addEmptyPosition )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().insert( basar::FOR_INSERT );
	completeForeignKey( yitPos );

	return yitPos;
}

void ScheduledDeliveryPositionCollection::deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, deletePosition )
	BLOG_TRACE_METHOD( getLogger(), fun );

	get().markForDelete( yit );
}

void ScheduledDeliveryPositionCollection::save()
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::scheduledDelivery::lit::SAVE_SCHEDULEDDELIVERY_POSITION;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		if( false == isSavable() )
		{
			return;
		}

		saveDeletePositions();
		saveExistingPositions();
	}
	catch( basar::Exception & e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SAVE_SCHEDULEDDELIVERY_POSITION, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_SCHEDULEDDELIVERY_POSITION, executeResultInfo );
	}
}

void ScheduledDeliveryPositionCollection::saveDeletePositions()
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, saveDeletePositions )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::scheduledDelivery::lit::SAVE_SCHEDULEDDELIVERY_POSITION;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().begin( basar::SS_DELETE );

	while( !yitPos.isEnd() )
	{
		executeResultInfo = getAccessor()->execute( SAVE_SCHEDULEDDELIVERY_POSITION, yitPos );

		if( executeResultInfo.hasError() || 1 != executeResultInfo.getAffectedRows() )
		{
			THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_SCHEDULEDDELIVERY_POSITION, executeResultInfo )
		}

		yitPos = get().begin( basar::SS_DELETE );
	}
}

void ScheduledDeliveryPositionCollection::saveExistingPositions()
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, saveExistingPositions )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::scheduledDelivery::lit::SAVE_SCHEDULEDDELIVERY_POSITION;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().begin( basar::SS_INSERT | basar::SS_UPDATE );

	for( ; false == yitPos.isEnd(); ++yitPos )
	{
		executeResultInfo = getAccessor()->execute( SAVE_SCHEDULEDDELIVERY_POSITION, yitPos );

		if( executeResultInfo.hasError() || executeResultInfo.m_affectedRows != 1 )
		{
			THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_SCHEDULEDDELIVERY_POSITION, executeResultInfo )
		}
	}
}

void ScheduledDeliveryPositionCollection::setForeignKey( const basar::Int32 id )
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, setForeignKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().begin();
	for( ; false == yitPos.isEnd(); ++yitPos )
	{
		yitPos.setInt32( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID, id );
	}
}

bool ScheduledDeliveryPositionCollection::isForeignKeySet( const basar::db::aspect::AccessorPropertyTable_YIterator yitPos ) const
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, isForeignKeySet )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return yitPos.isContainedAndSet( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID );
}

void ScheduledDeliveryPositionCollection::completeForeignKey( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, completeForeignKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( isForeignKeySet( yit ) )
	{
		return;
	}

	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().begin();
	for( ; false == yitPos.isEnd(); ++yitPos )
	{
		if( yitPos.isContainedAndSet( properties::TENDER_NO ) )
		{
			yit.setInt32( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID, yitPos.getInt32( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID ) );
			break;
		}
	}
}

libutil::domMod::SearchYIteratorPtr ScheduledDeliveryPositionCollection::getSearchYIterator() const
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, getSearchYIterator )
	BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

	if( NULL == m_SearchYIterator.get() )
	{
		m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
			new libutil::domMod::SearchYIterator(
			properties::TENDER_ID.toDescriptionString() +
			properties::TENDER_STATE.toDescriptionString() +
			properties::ARTICLE_CODE.toDescriptionString() +
			properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toDescriptionString() +
			properties::DELIVERY_DATE.toDescriptionString() +
			properties::PURCHASE_DB.toDescriptionString() +
			properties::PURCHASE_DBSRV.toDescriptionString()
			)
			);
	}
	m_SearchYIterator->get().setString( properties::PURCHASE_DBSRV,	m_PurchaseDBParameterProxy->getPurchaseDBServer() );
	m_SearchYIterator->get().setString( properties::PURCHASE_DB,	m_PurchaseDBParameterProxy->getPurchaseDB() );
	return m_SearchYIterator;
}

void ScheduledDeliveryPositionCollection::resetSearchYit()
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, resetSearchYit )
	BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

	getSearchYIterator()->reset();
}

basar::db::aspect::AccessorPropertyTable_YIterator ScheduledDeliveryPositionCollection::getSearchYit() const
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, getSearchYit )
	BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

	return getSearchYIterator()->get();
}


bool ScheduledDeliveryPositionCollection::isSavable() const
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, isSavable )
		BLOG_TRACE_METHOD( getLogger(), fun );

	checkNumberPositions();

	bool isKeySet =	 true;

	basar::db::aspect::AccessorPropertyTableRef propTab = get();
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = propTab.begin();

	for( ; false == yitPos.isEnd(); ++yitPos )
	{
		isKeySet &= isForeignKeySet ( yitPos );
	}

	return ( isKeySet );
}

void ScheduledDeliveryPositionCollection::checkNumberPositions() const
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, checkNumberPositions )
		BLOG_TRACE_METHOD( getLogger(), fun );

	if( 0 == get().size() )
	{
		basar::VarString msg( "There are no positions!" );
		throw libutil::exceptions::InconsistentDataException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
	}
}

void ScheduledDeliveryPositionCollection::findByKey( basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, findByKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::scheduledDelivery::lit::SELECT_SCHEDULEDDELIVERY_POSITIONS_BY_KEY;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		getAccessor()->getPropertyTable().clear();
		executeResultInfo = getAccessor()->execute( SELECT_SCHEDULEDDELIVERY_POSITIONS_BY_KEY, yit );
	}
	catch( basar::Exception& e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_SCHEDULEDDELIVERY_POSITIONS_BY_KEY, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_SCHEDULEDDELIVERY_POSITIONS_BY_KEY, executeResultInfo )
	}
}

bool ScheduledDeliveryPositionCollection::isEmpty() const
{
	if( 0 == m_Accessor.get() || 
		getAccessor()->getPropertyTable().isNull() || 
		getAccessor()->getPropertyTable().isEmpty() )
	{
		return true;
	}

	return false;
}

basar::Int32 ScheduledDeliveryPositionCollection::getAlreadyReservedQuantity( const basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	METHODNAME_DEF( ScheduledDeliveryPositionCollection, getAlreadyReservedQuantity )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::scheduledDelivery::lit::SELECT_ALREADY_RESERVED_QUANTITY;

	std::pair <bool, basar::Decimal> result;
	try
	{
		result = getAccessor()->executeAggregate( SELECT_ALREADY_RESERVED_QUANTITY, yit );
	}
	catch( basar::Exception& e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_ALREADY_RESERVED_QUANTITY, __FILE__, __LINE__ );
		throw;
	}
	return result.second.toInt32();
}

//-----------------------------------------------------------------------------------------

} //end namespace scheduledDelivery
} //end namespace domMod
} //end namespace libtender
