#include "scheduleddeliverymaintainuc.h"
#include "scheduleddeliverymaintainvc.h"

#include <libtender/domainmodule/constants.h>
#include "tender/infrastructure/iusecasegetter.h"
#include "tender/usecase/ipickeruc.h"
#include <libtender/domainmodule/tender/tendertypeenum.h>

#include <libtender/domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverycollectiondm.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryheader.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverypositioncollection.h>
#include <libtender/domainmodule/purchaseorderproposal/ipurchaseorderproposal.h>
#include <libtender/domainmodule/tender/itenderprogress.h>
#include <libtender/domainmodule/customerlist/icustomerlistdm.h>

#include <loggerpool/tenderloggerpool.h>

#include <libutil/usecase.h>
#include <libutil/infrastructurecomponent.h>
#include <libbasardbaspect_exceptions.h>
#include <libtender/domainmodule/tender/itender.h>
#include <libtender/domainmodule/tender/itenderpositioncollection.h>
#include "libtender/domainmodule/libtender_properties_definitions.h"

//--------------------------------------------------------------------------------------------------//

namespace tender {
namespace useCase {

//--------------------------------------------------------------------------------------------------//

BEGIN_HANDLERS_REGISTRATION( ScheduledDeliveryMaintainUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, SaveDeliveryRequested               )
	SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, CheckDateRequested				    )
	SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, DeselectPositionRequested           )
	SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, SelectPositionRequested             )
	SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, SelectPositionCollectionRequested   )
	SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, DeselectPositionCollectionRequested )
	SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, GetAvailableQuantitiesRequested		)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, ChangeQuantityRequested				)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, ChangeBranchNoPOPRequested			)
    SYSTEM_EVENT_HANDLER_REGISTRATION( ScheduledDeliveryMaintainUC, SelectCustomerRequested             )
END_HANDLERS_REGISTRATION()

//--------------------------------------------------------------------------------------------------//

ScheduledDeliveryMaintainUC::ScheduledDeliveryMaintainUC() : libutil::useCase::IRunnable(),
	m_Logger( libtender::LoggerPool::getLoggerUseCases() ),
	m_ModifyExistingDelivery( false )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, ScheduledDeliveryMaintainUC )
	BLOG_TRACE_METHOD( m_Logger, fun );

	registerEventHandlers( true );
}

//--------------------------------------------------------------------------------------------------//

ScheduledDeliveryMaintainUC::~ScheduledDeliveryMaintainUC()
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, ~ScheduledDeliveryMaintainUC )
	BLOG_TRACE_METHOD( m_Logger, fun );

	registerEventHandlers( false );
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::injectUseCaseGetter(tender::componentManager::IUseCaseGetterPtr useCaseGetter)
{
	METHODNAME_DEF(ScheduledDeliveryMaintainUC, injectUseCaseGetter)
	BLOG_TRACE_METHOD(m_Logger, fun);

	m_UseCaseGetter = useCaseGetter;
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::injectScheduledDeliveryMaintainVC( tender::viewConn::IScheduledDeliveryMaintainVCPtr scheduledDeliveryMaintainVC )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, injectScheduledDeliveryMaintainVC )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_ScheduledDeliveryMaintainVC = scheduledDeliveryMaintainVC;
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::injectPurchaseOrderProposalDM( libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr popp )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, injectPurchaseOrderProposalDM )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_PurchaseOrderProposal = popp;
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgressDM )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, injectPurchaseOrderProposalDM )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_TenderProgress = tenderProgressDM;
}

void ScheduledDeliveryMaintainUC::injectCustomerCollectionDM( libtender::domMod::customer::ICustomerCollectionDMPtr customercoll )
{
    METHODNAME_DEF( ScheduledDeliveryMaintainUC, injectCustomerCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_CustomerCollection = customercoll;

}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::setScheduledDelivery( libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, setSchedule )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_ScheduledDelivery = scheduledDelivery;
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::setTender( libtender::domMod::tender::ITenderPtr tender )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, setTender )
		BLOG_TRACE_METHOD( m_Logger, fun );

	m_Tender = tender;
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::setScheduledDeliveryCollection( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr collection )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, setScheduledDeliveryCollection )
		BLOG_TRACE_METHOD( m_Logger, fun );

	m_ScheduledDeliveryCollection = collection;
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::run()
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, run )
	BLOG_TRACE_METHOD( m_Logger, fun );

	getScheduledDeliveryMaintainVC()->clear();

	if( NULL != m_Tender.get() )
	{
		getScheduledDeliveryMaintainVC()->matchFromTender( m_Tender );
	}

	if( NULL != m_ScheduledDelivery.get() )
	{
		getScheduledDeliveryMaintainVC()->matchFromScheduledDelivery( m_ScheduledDelivery );
	}

	getScheduledDeliveryMaintainVC()->show();
}

//--------------------------------------------------------------------------------------------------//

libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr ScheduledDeliveryMaintainUC::getScheduledDelivery()
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, getScheduledDelivery )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_ScheduledDelivery;
}

//--------------------------------------------------------------------------------------------------//

libtender::domMod::tender::ITenderPtr ScheduledDeliveryMaintainUC::getTender()
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, getTender )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_Tender;
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::init()
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, init )
	BLOG_TRACE_METHOD( m_Logger, fun );

}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::reset()
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, reset )
	BLOG_TRACE_METHOD( m_Logger, fun );

	init();
}

//--------------------------------------------------------------------------------------------------//

const log4cplus::Logger & ScheduledDeliveryMaintainUC::getLogger() const
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, getLogger )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_Logger;
}

//--------------------------------------------------------------------------------------------------//

tender::viewConn::IScheduledDeliveryMaintainVCPtr ScheduledDeliveryMaintainUC::getScheduledDeliveryMaintainVC()
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, getScheduledDeliveryMaintainVC )
	BLOG_TRACE_METHOD( m_Logger, fun );

	CHECK_INSTANCE_EXCEPTION( m_ScheduledDeliveryMaintainVC.get() );
	return m_ScheduledDeliveryMaintainVC;
}

bool ScheduledDeliveryMaintainUC::isBranchValid( int branch, int tenderType )
{
	
	//Serbia 
	if( libtender::domMod::tender::TT_STANDARD == tenderType )
	{
		if( 6 == branch || 8 == branch || 9 == branch )
		{
			return true;
		}
	}

	//Bulgaria 
	if( libtender::domMod::tender::TT_HOSPITAL == tenderType || libtender::domMod::tender::TT_STATE == tenderType )
	{
		if( 43 == branch || 48 == branch )
		{
			return true;
		}
	}

	//France 
	if( libtender::domMod::tender::TT_PREWHOLESALE <= tenderType )
	{
		if( 43 == branch || 48 == branch )
		{
			return true;
		}
	}

	return false;
}

basar::Int16 ScheduledDeliveryMaintainUC::getDefaultBranch( int tenderType )
{
	switch( tenderType )
	{
		case 1:
		case 2: 
		case 3: 
		case 4:
		case 5: return getTender()->getHeader().getInt16(properties::BRANCH_NO);
			break;
		case 6: return 43;
			break;
		default: return 0;
	}
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( ScheduledDeliveryMaintainUC, SaveDeliveryRequested )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, SaveDeliveryRequested )
	BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;

	try
	{
		getScheduledDeliveryMaintainVC()->matchToScheduledDelivery( getScheduledDelivery() );
        getScheduledDelivery()->getHeader()->get().setInt32( properties::TENDER_ID, m_Tender->getHeader().getInt32( properties::TENDER_NO ) );
		getScheduledDelivery()->getHeader()->get().setInt16( properties::BRANCH_NO, m_Tender->getHeader().getInt16( properties::BRANCH_NO ) );

		// TODO: DK SubUC

				YitList delList = getPositionsToDelete();
				YitList chgList = getPositionsToChange();
				m_ScheduledDelivery->save();
				transferDeletedPositionsToPurchase(delList);
				transferChangedPositionsToPurchase(chgList);

		getScheduledDeliveryMaintainVC()->close();
	}
	catch( const basar::Exception& e )
	{
		BLOG_ERROR( m_Logger, e.what().c_str() );
		throw;
	}

	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( ScheduledDeliveryMaintainUC, CheckDateRequested )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, CheckDateRequested )
	BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;

	try
	{
		getScheduledDeliveryMaintainVC()->matchToScheduledDelivery( getScheduledDelivery() );
		getScheduledDelivery()->getHeader()->get().setInt16( properties::BRANCH_NO, m_Tender->getHeader().getInt16( properties::BRANCH_NO ) );
		getScheduledDelivery()->getHeader()->get().setInt32( properties::TENDER_ID, m_Tender->getHeader().getInt32( properties::TENDER_NO ) );

		if( m_ScheduledDeliveryCollection->isExisting( getScheduledDelivery()->getHeader()->get() ) )
		{				
			getScheduledDeliveryMaintainVC()->displayDeliveryAlreadyExists();
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR );
		}

		if( m_Tender->getHeader().getDate( properties::DATE_FROM ).getDate() > getScheduledDelivery()->getHeader()->get().getDate( properties::DELIVERY_DATE ).getDate() ||  
			m_Tender->getHeader().getDate( properties::DATE_TO ).getDate() < getScheduledDelivery()->getHeader()->get().getDate( properties::DELIVERY_DATE ).getDate() )
		{
			getScheduledDeliveryMaintainVC()->displayInvalidDeliveryDate();
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR );
		}
	}
	catch( const basar::Exception& e )
	{
		BLOG_ERROR( m_Logger, e.what().c_str() );
		throw;
	}

	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( ScheduledDeliveryMaintainUC, SelectPositionRequested )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, SelectPositionRequested )
	BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;

	try
	{
		basar::db::aspect::AccessorPropertyTable_YIterator currentItem = getScheduledDeliveryMaintainVC()->getCurrentAvailableArticle( m_Tender->getPositionCollection()->get() );

		if (currentItem.isNull() || currentItem.isEnd() || 0 == currentItem.getInt32( properties::QUANTITY_AVAILABLE ) )
		{
			getScheduledDeliveryMaintainVC()->displayPositionHasNoAvailableQuantity();
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
		}

		basar::db::aspect::AccessorPropertyTable_YIterator selectedItem = getScheduledDelivery()->getPositionCollection()->get().begin();

		//TODO: move to ScheduledDeliveryCollectionDM					
		for( ; !selectedItem.isEnd(); ++selectedItem )
		{
			if( currentItem.getString( properties::ARTICLE_CODE ) == selectedItem.getString( properties::ARTICLE_CODE ) &&
				selectedItem.getState( properties::ARTICLE_CODE.getName() ) == basar::SS_DELETE )
			{
				//Preventing that the same item is inserted again, after first deleting it and then adding it again!
				selectedItem.setPropertyState( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.getName(), basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::ARTICLE_CODE.getName(),					   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::ARTICLE_NAME.getName(),					   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::QUANTITY.getName(),						   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::QUANTITY_DELIVERED.getName(),			   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::LAST_CHANGED_DATETIME.getName(),			   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::CREATION_DATETIME.getName(),				   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::ARTICLE_NO.getName(),					   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::PURCHASE_ORDER_PROPOSAL_FLAG.getName(),	   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::ORDER_PROPOSAL_NO.getName(),				   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::BRANCH_NO_POP.getName(),					   basar::SS_UPDATE );
				selectedItem.setPropertyState( properties::PURCHASE_ORDER_NO.getName(),				   basar::SS_UPDATE );
				
				getScheduledDeliveryMaintainVC()->matchFromPositionCollection( getScheduledDelivery()->getPositionCollection()->get() );

				return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
			}
		}

		basar::db::aspect::AccessorPropertyTable_YIterator toBeAdded = getScheduledDelivery()->getPositionCollection()->get().insert( basar::FOR_INSERT );
		 
		QString articlename = currentItem.getString( properties::ARTICLE_NAME ).c_str();

		toBeAdded.setString( properties::ARTICLE_CODE, currentItem.getString( properties::ARTICLE_CODE ) );
		toBeAdded.setInt32 ( properties::ARTICLE_NO,   currentItem.getInt32( properties::ARTICLE_NO    ) );
		toBeAdded.setString( properties::ARTICLE_NAME, currentItem.getString( properties::ARTICLE_NAME ) );
		toBeAdded.setInt32 ( properties::QUANTITY, 0 );
		toBeAdded.setInt16 ( properties::PURCHASE_ORDER_PROPOSAL_FLAG, getTender()->getHeader().getInt16(properties::PURCHASE_ORDER_PROPOSAL_FLAG) );
		toBeAdded.setInt16 ( properties::BRANCH_NO_POP, getDefaultBranch( getTender()->getHeader().getInt32( properties::TENDER_TYPE ) ) );
	
		getScheduledDeliveryMaintainVC()->matchFromPositionCollection( getScheduledDelivery()->getPositionCollection()->get() );
	}
	catch( const basar::Exception& e )
	{
		BLOG_ERROR( m_Logger, e.what().c_str() );
		throw;
	}

	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( ScheduledDeliveryMaintainUC, DeselectPositionRequested )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, DeselectPositionRequested )
	BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;

	try
	{
		basar::db::aspect::AccessorPropertyTableRef        selectedPositionCollectionPropTab = getScheduledDelivery()->getPositionCollection()->get();
		basar::db::aspect::AccessorPropertyTable_YIterator articleToBeDeselected =  getScheduledDeliveryMaintainVC()->getCurrentSelectedArticle( getScheduledDelivery()->getPositionCollection()->get() );
		
		if (articleToBeDeselected.isNull() || articleToBeDeselected.isEnd())
		{
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
		}

		if(  basar::SS_CLEAN  == articleToBeDeselected.getState( properties::ARTICLE_CODE.getName()).getState() || 
			 basar::SS_UPDATE == articleToBeDeselected.getState( properties::ARTICLE_CODE.getName()).getState() )
		{
			articleToBeDeselected.setInt32(properties::QUANTITY, 0);
			articleToBeDeselected.setInt16 (properties::PURCHASE_ORDER_PROPOSAL_FLAG, 0);  // TODO FLAG
			selectedPositionCollectionPropTab.markForDelete( articleToBeDeselected );
		}

		if( basar::SS_INSERT == articleToBeDeselected.getState( properties::ARTICLE_CODE.getName()).getState() )
		{
			selectedPositionCollectionPropTab.erase( articleToBeDeselected );
		}

		getScheduledDeliveryMaintainVC()->matchFromPositionCollection( getScheduledDelivery()->getPositionCollection()->get() );
	}

	catch( const basar::Exception& e )
	{
		BLOG_ERROR( m_Logger, e.what().c_str() );
		throw;
	}

	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( ScheduledDeliveryMaintainUC, SelectPositionCollectionRequested )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, SelectPositionCollectionRequested )
	BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;

	try
	{
		basar::db::aspect::AccessorPropertyTableRef selectedPositionCollectionPropTab = getScheduledDelivery()->getPositionCollection()->get();
		basar::db::aspect::AccessorPropertyTable_YIterator availableItem = m_Tender->getPositionCollection()->get().begin();

		bool isAdded = false;

		for( ; !availableItem.isEnd(); ++availableItem )
		{
			isAdded = false;
			basar::db::aspect::AccessorPropertyTable_YIterator selectedItem = selectedPositionCollectionPropTab.begin();
			
			//TODO: move to ScheduledDeliveryCollectionDM					
			for( ; !selectedItem.isEnd(); ++selectedItem )
			{
				if( selectedItem.getString( properties::ARTICLE_CODE.getName() ) == availableItem.getString( properties::ARTICLE_CODE.getName() ) )
				{
					if( 0 == availableItem.getInt32( properties::QUANTITY_AVAILABLE ) )
					{
						isAdded = true;
						continue;
					}
					
					if( selectedItem.getState( properties::ARTICLE_CODE.getName() ) == basar::SS_DELETE ) 
					{
						//Preventing that the same item is inserted again, after first deleting it and then adding it again!
						selectedItem.setPropertyState( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.getName(), basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::ARTICLE_CODE.getName(),					   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::ARTICLE_NAME.getName(),					   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::QUANTITY.getName(),						   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::QUANTITY_DELIVERED.getName(),			   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::LAST_CHANGED_DATETIME.getName(),			   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::CREATION_DATETIME.getName(),				   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::ARTICLE_NO.getName(),					   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::PURCHASE_ORDER_PROPOSAL_FLAG.getName(),	   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::ORDER_PROPOSAL_NO.getName(),				   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::BRANCH_NO_POP.getName(),					   basar::SS_UPDATE );
						selectedItem.setPropertyState( properties::PURCHASE_ORDER_NO.getName(),				   basar::SS_UPDATE );
					}

					isAdded = true;
					break;
				}
			}

			if( !isAdded &&
				0 < availableItem.getInt32( properties::QUANTITY_AVAILABLE ) )
			{
				basar::db::aspect::AccessorPropertyTable_YIterator toBeAdded = selectedPositionCollectionPropTab.insert( basar::FOR_INSERT );

				toBeAdded.setString( properties::ARTICLE_CODE, availableItem.getString( properties::ARTICLE_CODE ) );
				toBeAdded.setInt32 ( properties::ARTICLE_NO,   availableItem.getInt32 ( properties::ARTICLE_NO   ) );
				toBeAdded.setString( properties::ARTICLE_NAME, availableItem.getString( properties::ARTICLE_NAME ) );
				toBeAdded.setInt32 ( properties::QUANTITY, 0);
				toBeAdded.setInt16 ( properties::PURCHASE_ORDER_PROPOSAL_FLAG, getTender()->getHeader().getInt16(properties::PURCHASE_ORDER_PROPOSAL_FLAG) );
				toBeAdded.setInt16 ( properties::BRANCH_NO_POP, getDefaultBranch( getTender()->getHeader().getInt32( properties::TENDER_TYPE ) ) );
			}
		}

		getScheduledDeliveryMaintainVC()->matchFromPositionCollection( getScheduledDelivery()->getPositionCollection()->get() );
	}
	catch( const basar::Exception& e )
	{
		BLOG_ERROR( m_Logger, e.what().c_str() );
		throw;
	}

	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( ScheduledDeliveryMaintainUC, DeselectPositionCollectionRequested )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, DeselectPositionCollectionRequested )
	BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;

	try
	{
		basar::db::aspect::AccessorPropertyTableRef scheduledDeliveryPositionCollectionPropTab = getScheduledDelivery()->getPositionCollection()->get();

		basar::db::aspect::AccessorPropertyTable_YIterator position = scheduledDeliveryPositionCollectionPropTab.begin();

		//TODO: move to ScheduledDeliveryCollectionDM					
		while( !position.isEnd() )
		{
			switch( position.getState( properties::ARTICLE_CODE.getName()).getState() )
			{
			case basar::SS_UPDATE :
			case basar::SS_CLEAN :
				position.setInt32(properties::QUANTITY, 0);	// reset in case we have to reuse
				position.setInt16 (properties::PURCHASE_ORDER_PROPOSAL_FLAG, 0); //TODO FLAG
				scheduledDeliveryPositionCollectionPropTab.markForDelete( position );
				++position;
				break;
			case basar::SS_INSERT :
					position = scheduledDeliveryPositionCollectionPropTab.erase( position );
				break;
			default :
				++position;
				break;
			}
		}

		getScheduledDeliveryMaintainVC()->matchFromPositionCollection( getScheduledDelivery()->getPositionCollection()->get() );
	}
	catch( const basar::Exception& e )
	{
		BLOG_ERROR( m_Logger, e.what().c_str() );
		throw;
	}

	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( ScheduledDeliveryMaintainUC, GetAvailableQuantitiesRequested )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, GetAvailableQuantitiesRequested )
	BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;

	try
	{
		basar::db::aspect::AccessorPropertyTableRef articleCollection =  m_Tender->getPositionCollection()->get();
		basar::db::aspect::AccessorPropertyTable_YIterator article = articleCollection.begin();

		//TODO: move to ScheduledDeliveryCollectionDM					
		for( ; !article.isEnd(); ++article )
		{
			//past quantity------------------------------
			m_TenderProgress->resetSearchYit();
			m_TenderProgress->getSearchYit().setInt32( properties::ARTICLE_NO, article.getInt32( properties::ARTICLE_NO ) );
			m_TenderProgress->getSearchYit().setInt32( properties::TENDER_NO, m_Tender->getHeader().getInt32( properties::TENDER_NO ) );
            m_TenderProgress->findArticleQuantityByPattern( m_TenderProgress->getSearchYit() );

			basar::Int32 pastQty = 0;
			if( !m_TenderProgress->get().isEmpty() )
			{
				pastQty = m_TenderProgress->get().begin().getInt32( properties::QUANTITY_DELIVERED );
			}
			//past quantity------------------------------

			//future quantity----------------------------
			libtender::domMod::scheduledDelivery::IScheduledDeliveryPositionCollectionPtr positionCollection =
			getScheduledDelivery()->getPositionCollection();
				
			positionCollection->resetSearchYit();
			positionCollection->getSearchYit().setInt32(  properties::TENDER_ID,     m_Tender->getHeader().getInt32(properties::TENDER_NO) );
			positionCollection->getSearchYit().setString( properties::ARTICLE_CODE,  article.getString(properties::ARTICLE_CODE));

			if( m_ScheduledDelivery->getHeader()->get().isContainedAndSet( properties::DELIVERY_DATE ) )
			{
				positionCollection->getSearchYit().setDate(   properties::DELIVERY_DATE, m_ScheduledDelivery->getHeader()->get().getDate( properties::DELIVERY_DATE ) );
			}

			positionCollection->getSearchYit().setPropertyState( properties::TENDER_ID.getName(), basar::SS_DELETE );
			positionCollection->getSearchYit().setPropertyState( properties::ARTICLE_CODE.getName(), basar::SS_DELETE );

			basar::Int32 futureQty = positionCollection->getAlreadyReservedQuantity( positionCollection->getSearchYit() );
			//future quantity----------------------------
	
			//contract quantity--------------------------
			basar::Int32 tenderQty = article.getInt32( properties::CONTRACT_QTY );
			//contract quantity--------------------------

			//available quantity-------------------------
			basar::Int32 availableQty = tenderQty - ( pastQty + futureQty );
			//available quantity-------------------------
			
			article.setInt32( properties::QUANTITY_AVAILABLE, availableQty );
		}
	}
	catch( const basar::Exception& e )
	{
		BLOG_ERROR( m_Logger, e.what().c_str() );
		throw;
	}

	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( ScheduledDeliveryMaintainUC, ChangeQuantityRequested )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, ChangeQuantityRequested )
	BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;

	try
	{
		basar::db::aspect::AccessorPropertyTable_YIterator selectedArticle = getScheduledDeliveryMaintainVC()->getCurrentSelectedArticle( m_ScheduledDelivery->getPositionCollection() ->get() );
		basar::db::aspect::AccessorPropertyTableRef availableArticleCollection = m_Tender->getPositionCollection()->get();

		basar::db::aspect::AccessorPropertyTable_YIterator availableArticle = availableArticleCollection.begin();

		//TODO: move to ScheduledDeliveryCollectionDM					
		for( ; !availableArticle.isEnd(); ++availableArticle )
		{	
			if( selectedArticle.getString( properties::ARTICLE_CODE ) == availableArticle.getString( properties::ARTICLE_CODE ) )
			{
				if( selectedArticle.getInt32( properties::QUANTITY ) > availableArticle.getInt32( properties::QUANTITY_AVAILABLE ) )
				{
					selectedArticle.setInt32( properties::QUANTITY, availableArticle.getInt32( properties::QUANTITY_AVAILABLE ) );

					getScheduledDeliveryMaintainVC()->matchFromPositionCollection( m_ScheduledDelivery->getPositionCollection()->get() );
					getScheduledDeliveryMaintainVC()->displayQuantityExceedance( availableArticle.getInt32( properties::QUANTITY_AVAILABLE ) );
				}
			}
		}
		
	}
	catch( const basar::Exception& e )
	{
		BLOG_ERROR( m_Logger, e.what().c_str() );
		throw;
	}

	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( ScheduledDeliveryMaintainUC, ChangeBranchNoPOPRequested )
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, ChangeBranchNoPOPRequested )
	BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;

	try
	{
		basar::db::aspect::AccessorPropertyTable_YIterator selectedArticle = getScheduledDeliveryMaintainVC()->getCurrentSelectedArticle( m_ScheduledDelivery->getPositionCollection()->get() );

		int branchNoPop = selectedArticle.getInt16( properties::BRANCH_NO_POP );
		int tenderType = getTender()->getHeader().getInt32( properties::TENDER_TYPE );

		if( !isBranchValid( branchNoPop, tenderType ) )
		{
			selectedArticle.setInt16( properties::BRANCH_NO_POP, getDefaultBranch( tenderType ) );
			getScheduledDeliveryMaintainVC()->matchFromPositionCollection( m_ScheduledDelivery->getPositionCollection()->get() );
			getScheduledDeliveryMaintainVC()->displayBranchNoNotValid();
		}					
	}
	catch( const basar::Exception& e )
	{
		BLOG_ERROR( m_Logger, e.what().c_str() );
		throw;
	}

	return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION(ScheduledDeliveryMaintainUC, SelectCustomerRequested)
{
	METHODNAME_DEF(ScheduledDeliveryMaintainUC, SelectCustomerRequested)
	BLOG_TRACE_METHOD(m_Logger, fun);
	rSource;

	try
	{
		tender::useCase::IPickerUCPtr pharmacyChainPickerUC = m_UseCaseGetter->getPharmacyChainPickerUC();
		basar::db::aspect::AccessorPropertyTableRef customers;

		//TODO DZ: evtl. besser Lösung finden! 
		if(1 == m_Tender->getHeader().getInt32(properties::TENDER_TYPE)) //Serbia and Customers within the pharmacy chain! 
		{
			customers = getCustomersForPharmacyChain(pharmacyChainPickerUC);		
		}
		else //Bulgaria and CustomerList!
		{
			customers = m_Tender->getCustomerList()->get();
		}

        tender::useCase::IPickerUCPtr customerSelectionUC( 
			m_UseCaseGetter->getCustomerSelectionUC(customers, m_Tender->getHeader().getInt32(properties::CUSTOMER_NO), m_Tender->getHeader().getInt32(properties::PHARMACY_CHAIN_NO) ));

		basar::Int32 customerNo = getScheduledDeliveryMaintainVC()->getCustomer();

		for(basar::db::aspect::AccessorPropertyTable_YIterator yit = customers.begin(); !yit.isEnd(); ++yit)
		{
			if( customerNo == yit.getInt32(properties::CUSTOMER_NO) )
			{
				customerSelectionUC->setPicked(yit);
				break;
			}
		}

		customerSelectionUC->run();
        
        getScheduledDeliveryMaintainVC()->setCustomer(customerSelectionUC->getPicked());
	}
	catch(const basar::Exception& e)
	{
		BLOG_ERROR(m_Logger, e.what().c_str());
		throw;
	}

	return basar::appl::EventReturnStruct(basar::appl::HANDLER_OK);
}

basar::db::aspect::AccessorPropertyTableRef ScheduledDeliveryMaintainUC::getCustomersForPharmacyChain(tender::useCase::IPickerUCPtr pharmacyChainPickerUC)
{
	basar::db::aspect::AccessorPropertyTableRef foundCustomers;

	pharmacyChainPickerUC->getSearchYIterator().setInt32(properties::PHARMACY_CHAIN_NO, m_Tender->getHeader().getInt32(properties::PHARMACY_CHAIN_NO));

	pharmacyChainPickerUC->setSearchWithoutGUIMode(true);
	pharmacyChainPickerUC->run();

	foundCustomers = pharmacyChainPickerUC->getFound();

	return foundCustomers;
}

//--------------------------------------------------------------------------------------------------//

ScheduledDeliveryMaintainUC::YitList ScheduledDeliveryMaintainUC::getPositionsToChange()
{
	YitList changeList;
	basar::db::aspect::AccessorPropertyTableRef tabPos = getScheduledDelivery()->getPositionCollection()->get();
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos;
	
	//When the delivery date has changed, the expectedgoodsindate field (pubatchorderpos table) needs to be updated too! 
	if( basar::SS_UPDATE == getScheduledDelivery()->getHeader()->get().getState(properties::DELIVERY_DATE).getState() )
	{
		for(yitPos = tabPos.begin(); !yitPos.isEnd(); ++yitPos)
		{
			if( 0 == yitPos.getInt16( properties::PURCHASE_ORDER_PROPOSAL_FLAG ) )
			{
				continue;
			}

			changeList.push_back(yitPos);
		}

		return changeList;
	}

	for (yitPos = tabPos.begin(basar::SS_INSERT); !yitPos.isEnd(); ++yitPos)
	{
		if( 0 == yitPos.getInt16( properties::PURCHASE_ORDER_PROPOSAL_FLAG ) )
		{
			continue;
		}

		changeList.push_back(yitPos);
	}
	for (yitPos = tabPos.begin(basar::SS_UPDATE); !yitPos.isEnd(); ++yitPos)
	{
		if( 0 == yitPos.getInt16( properties::PURCHASE_ORDER_PROPOSAL_FLAG ) )
		{
			continue;
		}

		changeList.push_back(yitPos);
	}

	return changeList;
}

//--------------------------------------------------------------------------------------------------//

ScheduledDeliveryMaintainUC::YitList ScheduledDeliveryMaintainUC::getPositionsToDelete()
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, getPositionsToDelete )
	BLOG_TRACE_METHOD( m_Logger, fun );

	YitList delList;
	basar::db::aspect::AccessorPropertyTableRef tabPos = getScheduledDelivery()->getPositionCollection()->get();
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos;

	for (yitPos = tabPos.begin(basar::SS_DELETE); !yitPos.isEnd(); ++yitPos)
	{
		delList.push_back(yitPos.cloneToUnboundedPropertyRow());
	}

	for (yitPos = tabPos.begin(basar::SS_UPDATE); !yitPos.isEnd(); ++yitPos)
	{
		if( 0 == yitPos.getInt16( properties::PURCHASE_ORDER_PROPOSAL_FLAG) )
		{
			delList.push_back(yitPos.cloneToUnboundedPropertyRow());
		}
	}
	return delList;
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::preparePurchaseOrderProposalSearchIterator(const basar::db::aspect::AccessorPropertyTable_YIterator yitPos)
{
	m_PurchaseOrderProposal->resetSearchYit();
	//m_PurchaseOrderProposal->getSearchYit().setInt16(properties::BRANCH_NO,			yitPos.getInt16( properties::BRANCH_NO_POP ));
	m_PurchaseOrderProposal->getSearchYit().setInt32(properties::ARTICLE_NO,		yitPos.getInt32(properties::ARTICLE_NO));
	m_PurchaseOrderProposal->getSearchYit().setInt32(properties::ORDER_NO_BATCH,	getScheduledDelivery()->getHeader()->getScheduledDeliveryId());
	m_PurchaseOrderProposal->getSearchYit().setInt16(properties::ORDER_TYPE_BATCH,	constants::ORDER_TYPE_BATCH); // TODO
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::setPurchaseOrderProposalValues(const basar::db::aspect::AccessorPropertyTable_YIterator yitPos)
{
	m_PurchaseOrderProposal->setBranchNo(yitPos.getInt16( properties::BRANCH_NO_POP ));
	m_PurchaseOrderProposal->setArticleNo(yitPos.getInt32(properties::ARTICLE_NO));
	m_PurchaseOrderProposal->setOrderNoBatch(getScheduledDelivery()->getHeader()->getScheduledDeliveryId());
	m_PurchaseOrderProposal->setOrderTypeBatch(constants::ORDER_TYPE_BATCH); // TODO
	m_PurchaseOrderProposal->setOrderProposalQuantity(yitPos.getInt32(properties::QUANTITY));
	m_PurchaseOrderProposal->setExpectedGoodsinDate(getScheduledDelivery()->getHeader()->getDeliveryDate());
	m_PurchaseOrderProposal->setSupplierNo( 0 ); //Supplier is determined trough the purchase batch!
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::transferChangedPositionsToPurchase(const YitList& changedList)
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, transferChangedPositionsToPurchase )
	BLOG_TRACE_METHOD( m_Logger, fun );

	YitList::const_iterator it = changedList.begin();

	while (it != changedList.end())
	{
		preparePurchaseOrderProposalSearchIterator(*it);
		m_PurchaseOrderProposal->findByKey(m_PurchaseOrderProposal->getSearchYit());
		if (false == m_PurchaseOrderProposal->isEmpty() && m_PurchaseOrderProposal->getOrderProposalNo() != 0)
		{
			// entry already existing and check BVO-Nr!=0 --> no update possible!!
		}
		else
		{
			setPurchaseOrderProposalValues(*it);
			m_PurchaseOrderProposal->save();
		}
		++it;
	}
}

//--------------------------------------------------------------------------------------------------//

void ScheduledDeliveryMaintainUC::transferDeletedPositionsToPurchase(const YitList& deletedList)
{
	METHODNAME_DEF( ScheduledDeliveryMaintainUC, transferDeletedPositionsToPurchase )
	BLOG_TRACE_METHOD( m_Logger, fun );

	YitList::const_iterator it = deletedList.begin();
	while (it != deletedList.end())
	{
		preparePurchaseOrderProposalSearchIterator(*it);
		m_PurchaseOrderProposal->findByKey(m_PurchaseOrderProposal->getSearchYit());
		if (false == m_PurchaseOrderProposal->isEmpty())
		{
			if (m_PurchaseOrderProposal->getOrderProposalNo() != 0)
			{
				// entry already existing and check BVO-Nr!=0 --> no delete possible!!
			}
			else
			{
				m_PurchaseOrderProposal->markForDelete();
				m_PurchaseOrderProposal->save();
			}
		}
		++it;
	}
}

//--------------------------------------------------------------------------------------------------//

} // namespace useCase
} // namespace tender

//--------------------------------------------------------------------------------------------------//
