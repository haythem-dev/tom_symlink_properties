#include "cscordermaintainuc.h"
#include "cscordermaintainvc.h"

#include <libtender/domainmodule/constants.h>
#include <libtender/domainmodule/tender/tendertypeenum.h>

#include <libtender/domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryheader.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverypositioncollection.h>
#include <libtender/domainmodule/tender/itenderprogress.h>
#include <libtender/domainmodule/customer/icustomercollectiondm.h>
#include <libtender/domainmodule/cscordercustomer/icscordercustomercollectiondm.h>

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

BEGIN_HANDLERS_REGISTRATION( CSCOrderMaintainUC )
    SYSTEM_EVENT_HANDLER_REGISTRATION( CSCOrderMaintainUC, CreateCSCOrderRequested              )
    SYSTEM_EVENT_HANDLER_REGISTRATION( CSCOrderMaintainUC, SelectPositionRequested              )
    //SYSTEM_EVENT_HANDLER_REGISTRATION( CSCOrderMaintainUC, GetAvailableQuantitiesRequested      )
    SYSTEM_EVENT_HANDLER_REGISTRATION( CSCOrderMaintainUC, ChangeQuantityRequested              )
END_HANDLERS_REGISTRATION()

//--------------------------------------------------------------------------------------------------//

CSCOrderMaintainUC::CSCOrderMaintainUC() : libutil::useCase::IRunnable(),
    m_Logger( libtender::LoggerPool::getLoggerUseCases() )
{
    METHODNAME_DEF( CSCOrderMaintainUC, CSCOrderMaintainUC )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    registerEventHandlers( true );
}

//--------------------------------------------------------------------------------------------------//

CSCOrderMaintainUC::~CSCOrderMaintainUC()
{
    METHODNAME_DEF( CSCOrderMaintainUC, ~CSCOrderMaintainUC )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    registerEventHandlers( false );
}

//--------------------------------------------------------------------------------------------------//

void CSCOrderMaintainUC::injectCSCOrderMaintainVC( tender::viewConn::ICSCOrderMaintainVCPtr cscOrderMaintainVC )
{
    METHODNAME_DEF( CSCOrderMaintainUC, injectCSCOrderMaintainVC )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    m_CSCOrderMaintainVC = cscOrderMaintainVC;
}

//--------------------------------------------------------------------------------------------------//

void CSCOrderMaintainUC::injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgressDM )
{
    METHODNAME_DEF( CSCOrderMaintainUC, injectPurchaseOrderProposalDM )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    m_TenderProgress = tenderProgressDM;
}

//--------------------------------------------------------------------------------------------------//

void CSCOrderMaintainUC::injectCustomerCollectionDM( libtender::domMod::customer::ICustomerCollectionDMPtr customercoll )
{
    METHODNAME_DEF( CSCOrderMaintainUC, injectCustomerCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_CustomerCollection = customercoll;
}

//--------------------------------------------------------------------------------------------------//
 void CSCOrderMaintainUC::injectCscOrderCustomerCollectionDM( libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr  cscOrderCollectionDM)
 {
    METHODNAME_DEF( CSCOrderMaintainUC, injectCSCorderUC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_CscOrderCustomerCollection = cscOrderCollectionDM;
 }
//--------------------------------------------------------------------------------------------------//

void CSCOrderMaintainUC::injectCSCorderUC( libutil::useCase::IRunnablePtr cscOrderUC)
{
    METHODNAME_DEF( CSCOrderMaintainUC, injectCSCorderUC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_CSCOrderUC = cscOrderUC;
}

//--------------------------------------------------------------------------------------------------//

void CSCOrderMaintainUC::setScheduledDelivery( libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery )
{
    METHODNAME_DEF( CSCOrderMaintainUC, setSchedule )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    m_ScheduledDelivery = scheduledDelivery;
}

//--------------------------------------------------------------------------------------------------//

void CSCOrderMaintainUC::setTender( libtender::domMod::tender::ITenderPtr tender )
{
    METHODNAME_DEF( CSCOrderMaintainUC, setTender )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    m_Tender = tender;
}

//--------------------------------------------------------------------------------------------------//

void CSCOrderMaintainUC::run()
{
    METHODNAME_DEF( CSCOrderMaintainUC, run )
    BLOG_TRACE_METHOD( m_Logger, fun );

    if( NULL != m_Tender.get() )
    {
        getCSCOrderMaintainVC()->matchFromTender( m_Tender );
    }
    
    //if( NULL != m_ScheduledDelivery.get() )
    //{
    //    getCSCOrderMaintainVC()->matchFromScheduledDelivery( m_ScheduledDelivery );
    //}
    getCSCOrderMaintainVC()->show();
	
	// reset temporary value
    basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_Tender->getPositionCollection()->get().begin();
    for( ; false == yitPos.isEnd(); ++yitPos )
    {
		yitPos.setPropertyState( properties::QUANTITY_ALLOCATED, basar::SS_UNSET );
	}
}

//--------------------------------------------------------------------------------------------------//

libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr CSCOrderMaintainUC::getScheduledDelivery()
{
    METHODNAME_DEF( CSCOrderMaintainUC, getScheduledDelivery )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    return m_ScheduledDelivery;
}

//--------------------------------------------------------------------------------------------------//

libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr CSCOrderMaintainUC::getCscOrderCustomerCollection()
{
    METHODNAME_DEF( CSCOrderMaintainUC, getCscOrderCustomerCollection )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    return m_CscOrderCustomerCollection;
}

//--------------------------------------------------------------------------------------------------//

libutil::useCase::IRunnablePtr CSCOrderMaintainUC::getCSCOrderUC()
{
    METHODNAME_DEF( CSCOrderMaintainUC, getCSCOrderUC )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    return m_CSCOrderUC;
}

//--------------------------------------------------------------------------------------------------//

libtender::domMod::tender::ITenderPtr CSCOrderMaintainUC::getTender()
{
    METHODNAME_DEF( CSCOrderMaintainUC, getTender )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    return m_Tender;
}

//--------------------------------------------------------------------------------------------------//

const log4cplus::Logger & CSCOrderMaintainUC::getLogger() const
{
    METHODNAME_DEF( CSCOrderMaintainUC, getLogger )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    return m_Logger;
}

//--------------------------------------------------------------------------------------------------//

tender::viewConn::ICSCOrderMaintainVCPtr CSCOrderMaintainUC::getCSCOrderMaintainVC()
{
    METHODNAME_DEF( CSCOrderMaintainUC, getCSCOrderMaintainVC )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    CHECK_INSTANCE_EXCEPTION( m_CSCOrderMaintainVC.get() );
    return m_CSCOrderMaintainVC;
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( CSCOrderMaintainUC, CreateCSCOrderRequested )
{
    METHODNAME_DEF( CSCOrderMaintainUC, CreateCSCOrderRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;
    
    try
    {
        getCSCOrderUC()->run();
        getCSCOrderMaintainVC()->close();
    }
    catch( const basar::Exception& e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }
    
    return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( CSCOrderMaintainUC, SelectPositionRequested )
{
    METHODNAME_DEF( CSCOrderMaintainUC, SelectPositionRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        basar::db::aspect::AccessorPropertyTable_YIterator currentArticle = getCSCOrderMaintainVC()->getCurrentTenderPosition( m_Tender->getPositionCollection()->get() );

        if (currentArticle.isNull() || currentArticle.isEnd() || 0 == currentArticle.getInt32( properties::REMAINING_QUANTITY ) )
        {
            getCSCOrderMaintainVC()->displayPositionHasNoAvailableQuantity();
            return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
        }

        getCscOrderCustomerCollection()->get().clear();
        m_CustomerCollection->resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator findCustomer = m_CustomerCollection->getSearchYit();
        findCustomer.setString( properties::PHARMACY_GROUPID,  m_Tender->getHeader().getString( properties::PHARMACY_GROUPID ) );
        m_CustomerCollection->findCustomerByPharmacyGroupId(findCustomer);

        basar::db::aspect::AccessorPropertyTable_YIterator cusColl = m_CustomerCollection->get().begin();
        for( ; !cusColl.isEnd(); ++cusColl )
        {
            basar::db::aspect::AccessorPropertyTable_YIterator addCscOrderCusCol =  getCscOrderCustomerCollection()->get().insert( basar::FOR_CLEAN );

            addCscOrderCusCol.setInt32 ( properties::TENDER_ID, currentArticle.getInt32( properties::TENDER_NO    ) );
            addCscOrderCusCol.setString (properties::ARTICLE_CODE, currentArticle.getString ( properties::ARTICLE_CODE ) );
            addCscOrderCusCol.setInt32 ( properties::QUANTITY, 0);
            addCscOrderCusCol.setInt32 ( properties::CUSTOMER_NO, cusColl.getInt32( properties::CUSTOMER_NO ) );
            addCscOrderCusCol.setString (properties::PHARMACY_NAME, cusColl.getString ( properties::PHARMACY_NAME ) );
            addCscOrderCusCol.setString (properties::PHARMACY_LOCATION, cusColl.getString ( properties::PHARMACY_LOCATION ) );
            addCscOrderCusCol.setInt16 ( properties::BRANCH_NO, cusColl.getInt16( properties::BRANCH_NO) );

        }
        getCSCOrderMaintainVC()->matchFromOrderPositions( getCscOrderCustomerCollection()->get() );

    }
    catch( const basar::Exception& e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

void CSCOrderMaintainUC::getAvailableQuantities()
//SYSTEM_EVENT_HANDLER_DEFINITION( CSCOrderMaintainUC, GetAvailableQuantitiesRequested )
{
    METHODNAME_DEF( CSCOrderMaintainUC, GetAvailableQuantitiesRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    //rSource;
    
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

            //OwnQuota quantity----------------------------
            basar::Int32 ownQuotaQty = 0;
            if( article.getInt16( properties::OWNQUOTA_FLAG ) )
            {
                ownQuotaQty =  article.getInt32( properties::OWNQUOTA_QTY );
            }
            //contract quantity--------------------------
            basar::Int32 tenderQty = article.getInt32( properties::CONTRACT_QTY );
            //contract quantity--------------------------
            
            //available quantity-------------------------
            basar::Int32 availableQty = tenderQty - ( pastQty + ownQuotaQty );
            //available quantity-------------------------
            
            article.setInt32( properties::QUANTITY_AVAILABLE, availableQty );
            article.setInt32( properties::QUANTITY_ALLOCATED, 0 );


			basar::Int32 remQty =  article.getInt32( properties::REMAINING_QUANTITY );
			if( remQty == availableQty )
			{
				remQty--;
			}
        }
    }
    catch( const basar::Exception& e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }
    
    //return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
}

//--------------------------------------------------------------------------------------------------//

SYSTEM_EVENT_HANDLER_DEFINITION( CSCOrderMaintainUC, ChangeQuantityRequested )
{
    METHODNAME_DEF( CSCOrderMaintainUC, ChangeQuantityRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        basar::db::aspect::AccessorPropertyTableRef tenderPositionPropTab = m_Tender->getPositionCollection()->get();
        basar::db::aspect::AccessorPropertyTable_YIterator allocatedPosition = getCSCOrderMaintainVC()->getCurrentOrderPosition( getCscOrderCustomerCollection()->get() );
        
        basar::Int32 totalSelectedQty = 0;
        basar::Int32 selectedQty = allocatedPosition.getInt32( properties::QUANTITY );

        basar::db::aspect::AccessorPropertyTable_YIterator orderPosition = getCscOrderCustomerCollection()->get().begin( basar::SS_INSERT | basar::SS_UPDATE );
        for( ; !orderPosition.isEnd(); ++orderPosition )
        {
            totalSelectedQty += orderPosition.getInt32( properties::QUANTITY );
        }

        basar::db::aspect::AccessorPropertyTable_YIterator tenderPosition = tenderPositionPropTab.begin();
        for( ; !tenderPosition.isEnd(); ++tenderPosition )
        {
            if( allocatedPosition.getString( properties::ARTICLE_CODE ) == tenderPosition.getString( properties::ARTICLE_CODE ) )
            {
                if( totalSelectedQty > tenderPosition.getInt32( properties::REMAINING_QUANTITY ) )
                {
                    basar::Int32 requiredQty = tenderPosition.getInt32( properties::REMAINING_QUANTITY ) - (totalSelectedQty - selectedQty ) ;
                    allocatedPosition.setInt32( properties::QUANTITY, requiredQty);
                    getCSCOrderMaintainVC()->matchFromOrderPositions( getCscOrderCustomerCollection()->get() );
                    getCSCOrderMaintainVC()->displayQuantityExceedance( tenderPosition.getInt32( properties::REMAINING_QUANTITY ) );
                }

				totalSelectedQty = 0;
				basar::db::aspect::AccessorPropertyTable_YIterator orderPos = getCscOrderCustomerCollection()->get().begin( basar::SS_INSERT | basar::SS_UPDATE );
				for( ; false == orderPos.isEnd(); ++orderPos )
				{
					totalSelectedQty += orderPos.getInt32( properties::QUANTITY );
				}

				tenderPosition.setInt32( properties::QUANTITY_ALLOCATED, totalSelectedQty );
				getCSCOrderMaintainVC()->matchFromTender( m_Tender );
                break;
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

} // namespace useCase
} // namespace tender

//--------------------------------------------------------------------------------------------------//
