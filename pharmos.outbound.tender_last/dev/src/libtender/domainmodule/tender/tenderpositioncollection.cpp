#include "tenderpositioncollection.h"
#include <loggerpool/tenderloggerpool.h>
#include <domainmodule/libtender_properties_definitions.h>
#include <infrastructure/accessor/tender/tenderpositionsacc_definitions.h>

#include <libbasardbaspect_exceptions.h>
#include <libutil/exception.h>

namespace libtender
{
namespace domMod
{
namespace tender
{
	using basar::db::aspect::AccessorPropertyTableRef;
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
const libutil::misc::ClassInfo& TenderPositionCollection::getClassInfo()
{
	static const libutil::misc::ClassInfo info( "TenderPositionCollection", libutil::misc::Version( 1, 0, 0, 0 ) );
	return info;
}

TenderPositionCollection::TenderPositionCollection()
: m_Logger( libtender::LoggerPool::getLoggerDomModules() )
{
}

TenderPositionCollection::~TenderPositionCollection()
{
}

void TenderPositionCollection::injectTenderPositionCollectionAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
{
	m_Accessor = accessor;
}

// TODO: dynamic acces method!!!
void TenderPositionCollection::findByKeyFR(const AccessorPropertyTable_YIterator yitHeader)
{
	METHODNAME_DEF( TenderPositionCollection, findByKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::tender::lit::SELECT_TENDER_POSITIONS_BY_KEY_FR;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		getAccessor()->getPropertyTable().clear();
		executeResultInfo = getAccessor()->execute( SELECT_TENDER_POSITIONS_BY_KEY_FR, yitHeader, true, true, basar::db::aspect::HOLD_RECORDSET );
	}
	catch( basar::Exception & e  )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_TENDER_POSITIONS_BY_KEY_FR, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDER_POSITIONS_BY_KEY_FR, executeResultInfo )
	}
}

void TenderPositionCollection::findByKeyRS(const AccessorPropertyTable_YIterator yitHeader)
{
	METHODNAME_DEF( TenderPositionCollection, findByKeyRS )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::tender::lit::SELECT_TENDER_POSITIONS_BY_KEY_RS;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		getAccessor()->getPropertyTable().clear();
		executeResultInfo = getAccessor()->execute( SELECT_TENDER_POSITIONS_BY_KEY_RS, yitHeader );
	}
	catch( basar::Exception & e  )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_TENDER_POSITIONS_BY_KEY_RS, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDER_POSITIONS_BY_KEY_RS, executeResultInfo )
	}
}

void TenderPositionCollection::findByKeyBG(const AccessorPropertyTable_YIterator yitHeader)
{
	METHODNAME_DEF( TenderPositionCollection, findByKeyBG )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::tender::lit::SELECT_TENDER_POSITIONS_BY_KEY_BG;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		getAccessor()->getPropertyTable().clear();
		executeResultInfo = getAccessor()->execute( SELECT_TENDER_POSITIONS_BY_KEY_BG, yitHeader );
	}
	catch( basar::Exception & e  )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_TENDER_POSITIONS_BY_KEY_BG, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDER_POSITIONS_BY_KEY_BG, executeResultInfo )
	}
}

void TenderPositionCollection::findByKeyCH( const AccessorPropertyTable_YIterator yitHeader )
{
	METHODNAME_DEF( TenderPositionCollection, findByKeyCH )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::tender::lit::SELECT_TENDER_POSITIONS_BY_KEY_CH;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		getAccessor()->getPropertyTable().clear();
		executeResultInfo = getAccessor()->execute( SELECT_TENDER_POSITIONS_BY_KEY_CH, yitHeader );
	}
	catch( basar::Exception & e  )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_TENDER_POSITIONS_BY_KEY_CH, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDER_POSITIONS_BY_KEY_CH, executeResultInfo )
	}
}

basar::db::aspect::AccessorPropertyTable_YIterator TenderPositionCollection::addEmptyPosition()
{
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().insert(basar::FOR_INSERT);

	yitPos.setInt32  ( properties::CONTRACT_QTY.getName(),		0					);
	yitPos.setDecimal( properties::CONTRACT_PRICE.getName(),	basar::Decimal(0)	);
	yitPos.setDecimal( properties::PRICE_INCL_VAT.getName(),	basar::Decimal(0)	);
	yitPos.setDecimal( properties::AEP.getName(),				basar::Decimal(0)	);
	yitPos.setDecimal( properties::ADD_DISCOUNT_PCT.getName(),	basar::Decimal(0)	);
	yitPos.setInt16( properties::DISCOUNT_CALC_FROM.getName(), 0 ); // 0: GEP, calc discount from GEP as per default
	yitPos.setInt16( properties::DISCOUNT_APPLY_TO.getName(), 1 );  // 1: AGP, apply discount to AGP as per default
	yitPos.setInt32( properties::OWNQUOTA_QTY.getName(), 0 );
	yitPos.setInt16( properties::OWNQUOTA_FLAG.getName(), 0 );
	yitPos.setInt16( properties::RECALL_FLAG.getName(), 0 );
	completeForeignKey( yitPos );

	return yitPos;
}

void TenderPositionCollection::deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	get().markForDelete( yit );
}

basar::db::aspect::AccessorPropertyTableRef TenderPositionCollection::get()
{
	return getAccessor()->getPropertyTable();
}

const basar::db::aspect::AccessorPropertyTableRef TenderPositionCollection::get() const
{
	return getAccessor()->getPropertyTable();
}

basar::db::aspect::AccessorPropertyTable_YIterator TenderPositionCollection::getByArticleCode( const basar::VarString articleCode )
{
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos;
	basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin();
	for( ; yit != get().end(); ++yit )
	{
		if( articleCode == yit.getString( properties::ARTICLE_CODE ) )
		{
			yitPos = yit;
			break;
		}
	}

	return yitPos;
}

void TenderPositionCollection::setForeignKey( const basar::Int32 id )
{
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().begin();
	for( ; false == yitPos.isEnd(); ++yitPos )
	{
		yitPos.setInt32( properties::TENDER_NO, id );
	}
}

void TenderPositionCollection::setState(const TenderStateEnum tenderState)
{
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().begin();
	for(; false == yitPos.isEnd(); ++yitPos)
	{
		yitPos.setInt16(properties::TENDER_STATE, static_cast<basar::Int16>(tenderState));
	}
}

void TenderPositionCollection::setNameOfUser( const std::string nameOfUser )
{
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().begin();
	for( ; false == yitPos.isEnd(); ++yitPos )
	{
		yitPos.setString( properties::NAME_OF_USER, nameOfUser );
	}
}

void TenderPositionCollection::save()
{
	METHODNAME_DEF( TenderPositionCollection, save )

	using libtender::infrastructure::accessor::tender::lit::SAVE_TENDER_POSITION;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		validate();

		saveDeletePositions();
		saveExistingPositions();
	}
	catch( basar::Exception & e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SAVE_TENDER_POSITION, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_TENDER_POSITION, executeResultInfo );
	}
}

bool TenderPositionCollection::isEmpty() const
{
	if(    0 == m_Accessor.get() 
		|| getAccessor()->getPropertyTable().isNull() 
		|| getAccessor()->getPropertyTable().isEmpty() )
	{
		return true;
	}

	return false;
}

void TenderPositionCollection::validate() const
{
	// TODO
//	getValidator()->validate();
}

bool TenderPositionCollection::isValid() const
{
	return true;
	//return getValidator()->isValid();
}

libutil::infrastructure::accessor::IAccessorPtr TenderPositionCollection::getAccessor() const
{
	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
	return m_Accessor;
}

void TenderPositionCollection::saveDeletePositions()
{
	METHODNAME_DEF( TenderPositionCollection, saveDeletePositions )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::tender::lit::SAVE_TENDER_POSITION;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	AccessorPropertyTable_YIterator yitPos = get().begin( basar::SS_DELETE );

	while( !yitPos.isEnd() )
	{
		if( !hasDeleteStates( yitPos ) )
		{
            ++yitPos;
			continue;
		}

		executeResultInfo = getAccessor()->execute( SAVE_TENDER_POSITION, yitPos );
		yitPos = get().begin( basar::SS_DELETE );

		if( executeResultInfo.hasError() || 1 != executeResultInfo.getAffectedRows() )
		{
			THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_TENDER_POSITION, executeResultInfo )
		}
	}
}

void TenderPositionCollection::saveExistingPositions()
{
	METHODNAME_DEF( TenderPositionCollection, saveExistingPositions )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::tender::lit::SAVE_TENDER_POSITION;

	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	AccessorPropertyTable_YIterator yitPos = get().begin( basar::SS_INSERT | basar::SS_UPDATE );

	for( ; false == yitPos.isEnd(); ++yitPos )
	{
		if( !hasSavableStates( yitPos ) )
		{
			continue;
		}
		// check for missing values
		if (yitPos.isContainedAndSet(properties::DISCOUNT_CALC_FROM) == false)
		{
			yitPos.setInt16(properties::DISCOUNT_CALC_FROM, 0);
		}
		if (yitPos.isContainedAndSet(properties::DISCOUNT_APPLY_TO) == false)
		{
			yitPos.setInt16(properties::DISCOUNT_APPLY_TO, 1);
		}
		executeResultInfo = getAccessor()->execute( SAVE_TENDER_POSITION, yitPos );

		if( executeResultInfo.hasError() || executeResultInfo.m_affectedRows != 1 )
		{
			THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_TENDER_POSITION, executeResultInfo )
		}
	}
}

void TenderPositionCollection::completeForeignKey( basar::db::aspect::AccessorPropertyTable_YIterator yitRhs )
{
	if( isForeignKeySet( yitRhs ) )
	{
		return;
	}

	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = get().begin();
	for( ; false == yitPos.isEnd(); ++yitPos )
	{
		if( yitPos.isContainedAndSet( properties::TENDER_NO ) )
		{
			yitRhs.setInt32( properties::TENDER_NO, yitPos.getInt32( properties::TENDER_NO ) );
			break;
		}
	}
}

bool TenderPositionCollection::isForeignKeySet( const basar::db::aspect::AccessorPropertyTable_YIterator yitPos ) const
{
	return yitPos.isContainedAndSet( properties::TENDER_NO );
}

bool TenderPositionCollection::isSavable() const
{
	METHODNAME_DEF( TenderPositionCollection, isSavable )
	BLOG_TRACE_METHOD( getLogger(), fun );

	checkNumberPositions();

	bool isKeySet = true;
	bool isSavable = false;

	basar::db::aspect::AccessorPropertyTableRef propTab = get();
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = propTab.begin();
	for( ; false == yitPos.isEnd(); ++yitPos )
	{
		isKeySet &= isForeignKeySet ( yitPos );
		isSavable |= hasSavableStates( yitPos );
	}

	return ( isKeySet && isSavable );
}

void TenderPositionCollection::checkNumberPositions() const
{
	METHODNAME_DEF( TenderPositionCollection, checkNumberPositions )

	if( 0 == get().size() )
	{
		basar::VarString msg( "There are no positions!" );
		throw libutil::exceptions::InconsistentDataException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
	}
}

bool TenderPositionCollection::hasSavableStates( const basar::db::aspect::AccessorPropertyTable_YIterator yitPos ) const
{
	if( basar::SS_INSERT == yitPos.getState( properties::ARTICLE_CODE ).getState() ||
		basar::SS_INSERT == yitPos.getState( properties::ARTICLE_NO ).getState() ||
		basar::SS_DELETE == yitPos.getState( properties::ARTICLE_CODE).getState() ||
		basar::SS_DELETE == yitPos.getState( properties::ARTICLE_NO).getState() )
	{
		return true;
	}

	if( basar::SS_UPDATE == yitPos.getState( properties::CONTRACT_QTY ).getState() )
	{
		return true;
	}

	if( basar::SS_UPDATE == yitPos.getState( properties::CONTRACT_PRICE ).getState() )
	{
		return true;
	}

	if( basar::SS_UPDATE == yitPos.getState( properties::ADD_DISCOUNT_PCT ).getState() )
	{
		return true;
	}

	if( basar::SS_UPDATE == yitPos.getState( properties::TENDER_STATE ).getState() )
	{
		return true;
	}

	if( basar::SS_UPDATE == yitPos.getState( properties::OWNQUOTA_QTY ).getState() )
	{
		return true;
	}

	if( basar::SS_UPDATE == yitPos.getState( properties::OWNQUOTA_FLAG ).getState() )
	{
		return true;
	}

	if( basar::SS_UPDATE == yitPos.getState( properties::RECALL_FLAG ).getState() )
	{
		return true;
	}
	if (yitPos.isContainedAndSet(properties::DISCOUNT_CALC_FROM))
	{
		if (basar::SS_UPDATE == yitPos.getState(properties::DISCOUNT_CALC_FROM).getState())
		{
			return true;
		}
	}
	if (yitPos.isContainedAndSet(properties::DISCOUNT_APPLY_TO))
	{
		if (basar::SS_UPDATE == yitPos.getState(properties::DISCOUNT_APPLY_TO).getState())
		{
			return true;
		}
	}
	return false;
}

bool TenderPositionCollection::hasDeleteStates( const basar::db::aspect::AccessorPropertyTable_YIterator yitPos ) const
{
	if( basar::SS_DELETE != yitPos.getState( properties::TENDER_NO ).getState() )
	{
		return false;
	}

	if( basar::SS_DELETE != yitPos.getState( properties::ARTICLE_CODE).getState() &&
		basar::SS_DELETE != yitPos.getState( properties::ARTICLE_NO).getState() )
	{
		return false;
	}

	return true;
}

const log4cplus::Logger & TenderPositionCollection::getLogger() const
{
	return m_Logger;
}

} // end namespace tender
} // end namespace domMod
} // end namespace libtender
