#include "tenderstateuc.h"

#include "domainmodule/tender/itendercollectiondm.h"
#include <libtender/domainmodule/tender/itenderpositioncollection.h>
#include "domainmodule/tender/tenderstateenum.h"
#include "domainmodule/tender/tenderdm.h"
#include "domainmodule/libtender_properties_definitions.h"
#include <loggerpool/tenderloggerpool.h>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
	#include <QCoreApplication>
#pragma warning (pop)

namespace tender
{
namespace useCase
{

TenderStateUC::TenderStateUC(): m_Logger( libtender::LoggerPool::getLoggerUseCases() )
{
	METHODNAME_DEF( TenderStateUC, TenderStateUC );
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_TenderEndDateOffset = 1;
}

TenderStateUC::~TenderStateUC()
{
	METHODNAME_DEF( TenderStateUC, ~TenderStateUC );
	BLOG_TRACE_METHOD( m_Logger, fun );
}

libtender::domMod::tender::ITenderCollectionDMPtr TenderStateUC::getTenderCollectionDM()
{
	METHODNAME_DEF( TenderStateUC, getTenderCollectionDM );
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_TenderCollection;
}

void TenderStateUC::injectTenderCollectionDM(libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM)
{
	METHODNAME_DEF( TenderStateUC, injectTenderCollectionDM );
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_TenderCollection = tenderCollectionDM;
}

basar::cmnutil::Date TenderStateUC::getTenderEndDate(basar::cmnutil::Date dateFrom)
{
	METHODNAME_DEF( TenderStateUC, getTenderEndDate );
	BLOG_TRACE_METHOD( m_Logger, fun );

	using basar::cmnutil::Date;

	Date dateTo = dateFrom;
	dateTo.addMonths(m_OMG_Duration);
	basar::Int32 daysOfMonth = Date::getDaysOfMonth( dateTo.getYear(), dateTo.getMonth() );
	dateTo = Date(dateTo.getYear(), dateTo.getMonth(), daysOfMonth);
	dateTo.addDays(-m_TenderEndDateOffset);
	
	if(basar::cmnutil::Date::SUNDAY == dateTo.getDayOfWeek())
	{
		dateTo.addDays(-2);
	}
	else if(basar::cmnutil::Date::SATURDAY == dateTo.getDayOfWeek())
	{
		dateTo.addDays(-1);
	}

	return dateTo;
}

void TenderStateUC::setTender(libtender::domMod::tender::ITenderPtr tender) 
{
	METHODNAME_DEF( TenderStateUC, setTender );
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_Tender = tender;
}
	
void TenderStateUC::setOMGDuration(const basar::Int32 duration)
{
	METHODNAME_DEF( TenderStateUC, setOMGDuration );
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_OMG_Duration = duration;
}

void TenderStateUC::setTenderEndDateOffset(basar::Int32 days)
{
	METHODNAME_DEF( TenderStateUC, setTenderEndDateOffset );
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_TenderEndDateOffset = days;
}

void TenderStateUC::findActiveTender(const basar::Int32 articleNo, const basar::Int32 tenderNo)
{
	METHODNAME_DEF( TenderMaintainUC, findActiveTender )
	BLOG_TRACE_METHOD( m_Logger, fun );

	getTenderCollectionDM()->resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator findValidTender = getTenderCollectionDM()->getSearchYit();
	findValidTender.setString( properties::PHARMACY_GROUPID, m_Tender->getHeader().getString( properties::PHARMACY_GROUPID ) );
	findValidTender.setInt32( properties::ARTICLE_NO, articleNo );
	findValidTender.setInt16( properties::TENDER_STATE, libtender::domMod::tender::TS_OK );

	if( tenderNo > 0 )
	{
		findValidTender.setInt32( properties::TENDER_NO, tenderNo );
	}

	getTenderCollectionDM()->findValidTenders(findValidTender);
}

void TenderStateUC::findSuccessorTender(const basar::Int32 articleNo, const basar::Int32 tenderNo, const basar::VarString & pharmGrpID, const basar::Date & /*endDate*/)
{
	METHODNAME_DEF( TenderMaintainUC, findSuccessorTender )
	BLOG_TRACE_METHOD( m_Logger, fun );

	getTenderCollectionDM()->resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator findValidTender = getTenderCollectionDM()->getSearchYit();
	findValidTender.setString( properties::PHARMACY_GROUPID, pharmGrpID );
	findValidTender.setInt32( properties::ARTICLE_NO, articleNo );
	findValidTender.setInt32( properties::TENDER_NO, tenderNo );
	findValidTender.setInt16( properties::TENDER_STATE, libtender::domMod::tender::TS_UNCOMPLETED );
	//findValidTender.setDate( properties::DATE_TO, endDate );
	getTenderCollectionDM()->findValidTenders(findValidTender);
}

void TenderStateUC::SaveSuccessorTender()
{
	METHODNAME_DEF( TenderMaintainUC, SaveSuccessorTender )
	BLOG_TRACE_METHOD( m_Logger, fun );

	try
	{
		libtender::domMod::tender::ITenderPtr tender = getTenderCollectionDM()->getTender( getTenderCollectionDM()->get().begin() );
		tender->getHeader().setInt16(properties::TENDER_STATE, libtender::domMod::tender::TS_OK );
		tender->getHeader().setDate(properties::DATE_FROM, basar::cmnutil::Date::getCurrent() );
		tender->getHeader().setDate(properties::DATE_TO, getTenderEndDate() );
		tender->save();
	}
	catch( basar::Exception& e )
	{
		throw e;
	}
}

tender::useCase::StateValidationEnum TenderStateUC::validateStateForNewTender(const basar::Int16 currentState, const basar::Int32 articleNo)
{
	METHODNAME_DEF( TenderMaintainUC, validateStateForNewTender )
	BLOG_TRACE_METHOD( m_Logger, fun );

	try
	{
		findActiveTender( articleNo, 0 );

		//If there is currently an active tender for this position and pharmacygroup
		if(currentState == libtender::domMod::tender::TS_UNCOMPLETED && getTenderCollectionDM()->isEmpty() == false )  
		{
			//Set datefrom and dateto to an invalid date
			basar::Date invalidDate = basar::Date(9999,12,31);
			m_Tender->getHeader().setDate(properties::DATE_FROM, invalidDate);
			m_Tender->getHeader().setDate(properties::DATE_TO, invalidDate);
		}
		//If there is no successor for this position and pharmacygroup, set the tender to active
		else if(currentState == libtender::domMod::tender::TS_UNCOMPLETED && getTenderCollectionDM()->isEmpty() )  
		{
			m_Tender->getHeader().setInt16(properties::TENDER_STATE, libtender::domMod::tender::TS_OK );
			
			if(libtender::domMod::tender::TS_OK == m_Tender->getTenderState()) 
			{
				m_Tender->getHeader().setDate(properties::DATE_TO, getTenderEndDate(m_Tender->getHeader().getDate(properties::DATE_FROM)));
			}
		}
		//If state is active, but there is already an active tender for this postion and pharmacygroup
		else if(currentState == libtender::domMod::tender::TS_OK && getTenderCollectionDM()->isEmpty() == false )
		{
			return tender::useCase::ACTIVE_TENDER_ALREADY_EXIST;
		}
	}
	catch( basar::Exception& e )
	{
		throw e;
	}

	return tender::useCase::NONE;
}

tender::useCase::StateValidationEnum TenderStateUC::validateStateForExistingTender(const basar::Int16 oldState, const basar::Int16 currentState, const basar::Int32 articleNo)
{
	METHODNAME_DEF( TenderMaintainUC, validateStateForExistingTender )
	BLOG_TRACE_METHOD( m_Logger, fun );

	try
	{
		basar::Int32 tenderNo = m_Tender->getHeader().getInt32( properties::TENDER_NO );
		
		if( oldState == currentState && oldState == libtender::domMod::tender::TS_OK ) //cheking State is  not changed. Pharmacy groupId or article no
		{                                                                              // are changed -checking any valid Tender
			findActiveTender(articleNo,tenderNo);
			if( ! getTenderCollectionDM()->isEmpty() ) 
			{
				return tender::useCase::ACTIVE_TENDER_ALREADY_EXIST;
			}
		}
		else if( oldState == currentState && oldState == libtender::domMod::tender::TS_UNCOMPLETED ) //cheking State is  not changed. Pharmacy groupId or article no
		{
			findActiveTender(articleNo,tenderNo);
			if( getTenderCollectionDM()->isEmpty() )
			{
				m_Tender->getHeader().setInt16(properties::TENDER_STATE, libtender::domMod::tender::TS_OK );
			}
			else
			{
				//Set datefrom and dateto to an invalid date
				basar::Date invalidDate = basar::Date(9999,12,31);
				m_Tender->getHeader().setDate(properties::DATE_FROM, invalidDate);
				m_Tender->getHeader().setDate(properties::DATE_TO, invalidDate);
			}
		}
		else if( oldState == libtender::domMod::tender::TS_OK && currentState == libtender::domMod::tender::TS_UNCOMPLETED ) // checking State 1 --> 0
		{
			return tender::useCase::WRONG_TENDER_STATE_CHANGE;
		}
		else if( ( oldState == libtender::domMod::tender::TS_MANUALLY_CLOSED ||
				   oldState == libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED ) &&
				   currentState != oldState ) // checking State 9 or 8 --> 0 or 1
		{
			findActiveTender(articleNo,tenderNo); 
			if( ! getTenderCollectionDM()->isEmpty() ) 
			{
				return tender::useCase::ACTIVE_TENDER_ALREADY_EXIST;
			}
		}
		else if(oldState == libtender::domMod::tender::TS_UNCOMPLETED && currentState == libtender::domMod::tender::TS_OK)// checking State 0 --> 1
		{
			findActiveTender(articleNo,tenderNo);
			if( ! getTenderCollectionDM()->isEmpty() ) // Checking Active Tender 
			{
				 return tender::useCase::ACTIVE_TENDER_ALREADY_EXIST;
			}

			m_Tender->getHeader().setDate(properties::DATE_FROM, basar::cmnutil::Date::getCurrent() );
			m_Tender->getHeader().setDate(properties::DATE_TO, getTenderEndDate() );
		}
		else if(oldState == libtender::domMod::tender::TS_OK && ( currentState == libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED || // checking State 1 -->  8 0r 9
																  currentState == libtender::domMod::tender::TS_MANUALLY_CLOSED ) )
		{
			findSuccessorTender( articleNo, tenderNo, 
								m_Tender->getHeader().getString( properties::PHARMACY_GROUPID ), 
								m_Tender->getHeader().getDate( properties::DATE_TO ));
			if( false == getTenderCollectionDM()->isEmpty() ) // Checking Successor Tender 
			{
				 SaveSuccessorTender();
			}
		}
	}
	catch( basar::Exception& e )
	{
		throw e;
	}

	return tender::useCase::NONE;
}

basar::VarString TenderStateUC::getValidationErrorMessage(const StateValidationEnum stateError)
{
	switch( stateError )
	{
		case tender::useCase::ACTIVE_TENDER_ALREADY_EXIST:
			return QCoreApplication::translate( "tender::useCase::TenderStateUC", "Active Tender does already exist." ).toLocal8Bit().constData();
		case tender::useCase::WRONG_TENDER_STATE_CHANGE:
			return QCoreApplication::translate( "tender::useCase::TenderStateUC", "It's not allowed to change Tender state from Active to Capturing Data." ).toLocal8Bit().constData();
		default:
			return "";
	}
}

tender::useCase::StateValidationEnum TenderStateUC::validateTenderState(const basar::Int16 oldTenderState)
{
	METHODNAME_DEF( TenderStateUC, validateTenderState );
	BLOG_TRACE_METHOD( m_Logger, fun );

	try
	{
		bool isNewTender = m_Tender->getHeader().isContainedAndSet(properties::TENDER_NO) == false;
		basar::db::aspect::AccessorPropertyTable_YIterator yitTenderPos= m_Tender->getPositionCollection()->get().begin(basar::SS_CLEAN |
																														basar::SS_UPDATE |
																														basar::SS_INSERT );
		if(yitTenderPos.isEnd())
		{
			return tender::useCase::UNKNOWN;
		}

		basar::Int32 articleNo = yitTenderPos.getInt32( properties::ARTICLE_NO );
		basar::Int16 currentState = m_Tender->getHeader().getInt16(properties::TENDER_STATE);

		if(isNewTender)
		{
			return validateStateForNewTender( currentState, articleNo );
		}
		else
		{
			return validateStateForExistingTender(oldTenderState, currentState, articleNo);
		}
	}
	catch( basar::Exception& e )
	{
		throw e;
	}
}

tender::useCase::StateValidationEnum TenderStateUC::validateTenderStateForDelete(const basar::Int32 articleNo, const basar::Int32 tenderNo,
																				 const basar::VarString & pharmGrpID, const basar::Date & endDate)
{
	METHODNAME_DEF( TenderStateUC, validateTenderStateForDelete );
	BLOG_TRACE_METHOD( m_Logger, fun );

	try
	{
		findSuccessorTender( articleNo, tenderNo, pharmGrpID, endDate );
		if( false == getTenderCollectionDM()->isEmpty() ) // Checking Successor Tender 
		{
			SaveSuccessorTender();
		}
	}
	catch( basar::Exception& e )
	{
		throw e;
	}

	return tender::useCase::NONE;
}


}
}

