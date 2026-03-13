//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderoverviewuc.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/tender/itenderptr.h"
#include "domainmodule/tender/itender.h"
#include "domainmodule/scheduleddelivery/ischeduleptr.h"
#include "iusecasegetter.h"
#include "itenderstateuc.h"

#include "ipickeruc.h"
#include "statevalidationenum.h"
#include <domainmodule/tender/itendercollectiondm.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverycollectiondm.h>
#include <domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryheader.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverypositioncollection.h>
#include <libtender/domainmodule/tender/itenderprogress.h>
#include <libtender/domainmodule/tender/itenderpositioncollection.h>
#include <libtender/domainmodule/tender/tenderstateenum.h>
#include <libtender/domainmodule/purchaseorderproposal/ipurchaseorderproposal.h>
#include "tenderoverviewvc.h"

#include <libtender/domainmodule/constants.h>

#include <loggerpool/tenderloggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>

#include <libbasardbaspect_exceptions.h>
#include <libbasarlogin.h>

//-------------------------------------------------------------------------------------------------//
// namespace sections
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace useCase
    {
        BEGIN_HANDLERS_REGISTRATION( TenderOverviewUC )
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, SearchTenderRequested			)
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, EditTenderRequested			)
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, DeleteTenderRequested			)
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, CloneTenderRequested			)
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, NewTenderRequested				)
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, SearchArticleRequested			)
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, SearchCustomerRequested		)
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, SearchPartnerRequested			)
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, SearchSupplierRequested        )
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, SearchPharmacyChainRequested	)
            SYSTEM_EVENT_HANDLER_REGISTRATION( TenderOverviewUC, SearchPharmacyGroupRequested   )
        END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // TenderOverviewUC::TenderOverviewUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TenderOverviewUC::TenderOverviewUC()
        : m_Logger( libtender::LoggerPool::getLoggerUseCases() )
        {
            METHODNAME_DEF( TenderOverviewUC, TenderOverviewUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            registerEventHandlers( true );
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // TenderOverviewUC::~TenderOverviewUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TenderOverviewUC::~TenderOverviewUC()
        {
            METHODNAME_DEF( TenderOverviewUC, ~TenderOverviewUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            registerEventHandlers( false );
        }

        void TenderOverviewUC::injectTenderOverviewVC( tender::viewConn::TenderOverviewVCPtr tenderOverviewVC)
        {
            METHODNAME_DEF( TenderOverviewUC, injectTenderOverviewVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_TenderOverviewVC = tenderOverviewVC;
        }

        void TenderOverviewUC::injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM )
        {
            METHODNAME_DEF( TenderOverviewUC, injectTenderCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_TenderCollection = tenderCollectionDM;
        }

        void TenderOverviewUC::injectScheduledDeliveryCollectionDM( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollectionDM )
        {
            METHODNAME_DEF( TenderOverviewUC, injectScheduledDeliveryCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_ScheduledDeliveryCollection = scheduledDeliveryCollectionDM;
        }

        void TenderOverviewUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
        {
            METHODNAME_DEF( TenderOverviewUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_UseCaseGetter = useCaseGetter;
        }

        void TenderOverviewUC::injectTenderStateUC( tender::useCase::ITenderStateUCPtr tenderStateUC )
        {
            METHODNAME_DEF( TenderOverviewUC, injectTenderStateUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_TenderStateUC = tenderStateUC;
        }

        void TenderOverviewUC::injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgress )
        {
            METHODNAME_DEF( TenderOverviewUC, injectTenderProgressDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_TenderProgress = tenderProgress;
        }

        void TenderOverviewUC::injectPurchaseOrderProposalDM( libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr purchaseOrderProposal )
        {
            METHODNAME_DEF( TenderOverviewUC, injectTenderProgressDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_PurchaseOrderProposal = purchaseOrderProposal;
        }

        void TenderOverviewUC::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
        {
            METHODNAME_DEF( TenderOverviewUC, injectTransactionFactory )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_TransactionFactory = transactionFactory;
        }

        void TenderOverviewUC::setArea( basar::Int16 area )
        {
            METHODNAME_DEF( TenderOverviewUC, setArea )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_Area = area;
        }

        tender::componentManager::IUseCaseGetterPtr TenderOverviewUC::getUseCaseGetter()
        {
            METHODNAME_DEF( TenderOverviewUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
            return m_UseCaseGetter;
        }

        tender::viewConn::TenderOverviewVCPtr TenderOverviewUC::getTenderOverviewVC()
        {
            METHODNAME_DEF( TenderOverviewUC, getTenderOverviewVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_TenderOverviewVC.get() );
            return m_TenderOverviewVC;
        }

        libtender::domMod::tender::ITenderCollectionDMPtr TenderOverviewUC::getTenderCollectionDM()
        {
            METHODNAME_DEF( TenderOverviewUC, getTenderCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_TenderCollection.get() );
            return m_TenderCollection;
        }

        libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr TenderOverviewUC::getScheduledDeliveryCollectionDM()
        {
            METHODNAME_DEF( TenderOverviewUC, getScheduledDeliveryCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_ScheduledDeliveryCollection.get() );
            return m_ScheduledDeliveryCollection;
        }

        const log4cplus::Logger& TenderOverviewUC::getLogger() const
        {
            METHODNAME_DEF( TenderOverviewUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

        void TenderOverviewUC::init()
        {
            METHODNAME_DEF( TenderOverviewUC, init )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }

        void TenderOverviewUC::reset()
        {
            METHODNAME_DEF( TenderOverviewUC, reset )
            BLOG_TRACE_METHOD( m_Logger, fun );

            init();
        }

        bool TenderOverviewUC::isPurchaseOrderExisting(basar::db::aspect::AccessorPropertyTableRef deliveryCollection)
        {
            METHODNAME_DEF( TenderOverviewUC, isPurchaseOrderExisting )
            BLOG_TRACE_METHOD( m_Logger, fun );

            libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr delivery;
            basar::db::aspect::AccessorPropertyTable_YIterator deliveryYit;
            basar::db::aspect::AccessorPropertyTable_YIterator articleYit;

            deliveryYit = deliveryCollection.begin();

            while( !deliveryYit.isEnd() )
            {
                delivery = getScheduledDeliveryCollectionDM()->getScheduledDelivery( deliveryYit );
                articleYit = delivery->getPositionCollection()->get().begin();

                while( !articleYit.isEnd() ) 
                {
                    m_PurchaseOrderProposal->resetSearchYit();
                    m_PurchaseOrderProposal->getSearchYit().setInt32(properties::ORDER_NO_BATCH,	deliveryYit.getInt32(properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID));
                    m_PurchaseOrderProposal->getSearchYit().setInt16(properties::ORDER_TYPE_BATCH,	constants::ORDER_TYPE_BATCH);
                    m_PurchaseOrderProposal->getSearchYit().setInt32(properties::ARTICLE_NO,		articleYit .getInt32(properties::ARTICLE_NO));
                    m_PurchaseOrderProposal->findByKey(m_PurchaseOrderProposal->getSearchYit() );

                    if( false == m_PurchaseOrderProposal->isEmpty() && m_PurchaseOrderProposal->getOrderProposalNo() != 0 )
                    {
                        return true;
                    }
                    
                    ++articleYit;
                }

                ++deliveryYit;
            }

            return false;
        }

        void TenderOverviewUC::deletePurchaseOrderProposals(basar::db::aspect::AccessorPropertyTableRef deliveryCollection)
        {
            METHODNAME_DEF(TenderOverviewUC, deletePurchaseOrderProposals)
            BLOG_TRACE_METHOD(getLogger(), fun);

            libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr delivery;
            basar::db::aspect::AccessorPropertyTable_YIterator deliveryYit;
            basar::db::aspect::AccessorPropertyTable_YIterator articleYit;

            deliveryYit = deliveryCollection.begin();

            while( !deliveryYit.isEnd() )
            {
                delivery = getScheduledDeliveryCollectionDM()->getScheduledDelivery( deliveryYit );
                articleYit = delivery->getPositionCollection()->get().begin();

                while( !articleYit.isEnd() ) 
                {
                    m_PurchaseOrderProposal->resetSearchYit();
                    m_PurchaseOrderProposal->getSearchYit().setInt32(properties::ORDER_NO_BATCH,	deliveryYit.getInt32(properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID));
                    m_PurchaseOrderProposal->getSearchYit().setInt16(properties::ORDER_TYPE_BATCH,	constants::ORDER_TYPE_BATCH);
                    m_PurchaseOrderProposal->getSearchYit().setInt32(properties::ARTICLE_NO,		articleYit .getInt32(properties::ARTICLE_NO));
                    m_PurchaseOrderProposal->findByKey( m_PurchaseOrderProposal->getSearchYit() );

                    if( false == m_PurchaseOrderProposal->isEmpty() && 0 == m_PurchaseOrderProposal->getOrderProposalNo() )
                    {
                        m_PurchaseOrderProposal->markForDelete();
                        m_PurchaseOrderProposal->save();
                    }

                    ++articleYit;
                }

                ++deliveryYit;
            }
        }

        void TenderOverviewUC::run()
        {
            METHODNAME_DEF( TenderOverviewUC, run )
            BLOG_TRACE_METHOD( m_Logger, fun );

            init();

            getTenderOverviewVC()->reset();
            getTenderOverviewVC()->show();
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, SearchPharmacyGroupRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, SearchPharmacyGroupRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            try
            {
                getTenderOverviewVC()->matchToSearchCriteria( getTenderCollectionDM()->getSearchYit() );

                tender::useCase::IPickerUCPtr pharmacyGroupPickerUC;
                pharmacyGroupPickerUC = getUseCaseGetter()->getPharmacyGroupPickerUC( getTenderCollectionDM()->getSearchYit() );
                pharmacyGroupPickerUC->run();
                getTenderOverviewVC()->setPharmacyGroupId( pharmacyGroupPickerUC->getPicked() );
                getTenderCollectionDM()->resetSearchYit();
            }
            catch( const basar::Exception & e )
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }
            return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, SearchTenderRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, SearchTenderRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

            try
            {
                getTenderCollectionDM()->resetSearchYit();
                getTenderOverviewVC()->matchToSearchCriteria( getTenderCollectionDM()->getSearchYit() );

                getTenderCollectionDM()->findByPattern( getTenderCollectionDM()->getSearchYit() );

                if( getTenderCollectionDM()->isEmpty() )
                {
                    result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::TenderOverviewUC", "No tender found!").toLocal8Bit().data();

                    getTenderOverviewVC()->displayNoTenderFound();
                }

                getTenderOverviewVC()->matchFromTenderCollection( getTenderCollectionDM()->get() );
            }
            catch( const basar::Exception & e )
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }

            return result;
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, EditTenderRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, EditTenderRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            try
            {
                basar::db::aspect::AccessorPropertyTable_YIterator yitTender = getTenderOverviewVC()->getCurrentTender();

                libtender::domMod::tender::ITenderPtr tender = getTenderCollectionDM()->getTender( yitTender );

                getUseCaseGetter()->getTenderMaintainUC(tender)->run();

                getTenderCollectionDM()->findByPattern( getTenderCollectionDM()->getSearchYit() );
                getTenderOverviewVC()->matchFromTenderCollection( getTenderCollectionDM()->get() );

                //getTenderOverviewVC()->selectTender( tender->getHeader() );
            }
            catch( const basar::Exception & e )
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }

            return basar::appl::EventReturnStruct();
        }

        bool TenderOverviewUC::validateTenderState( const basar::Int32 articleNo, const basar::Int32 tenderNo,
                                                    const basar::VarString & pharmGrpID, const basar::Date & endDate  )
        {
            METHODNAME_DEF( TenderOverviewUC, validateTenderState )
            BLOG_TRACE_METHOD( m_Logger, fun );
            try
            {
                StateValidationEnum stateEnum = m_TenderStateUC->validateTenderStateForDelete( articleNo, tenderNo, pharmGrpID, endDate );
                if(stateEnum != tender::useCase::NONE)
                {
                    getTenderOverviewVC()->displayTenderStateWarning( m_TenderStateUC->getValidationErrorMessage( stateEnum ) );
                    return false;
                }
            }
            catch( basar::Exception& e )
            {
                throw e;
            }
            
            return true;
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, DeleteTenderRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, DeleteTenderRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();
    
            try
            {
                basar::db::aspect::AccessorPropertyTable_YIterator yitTender = getTenderOverviewVC()->getCurrentTender();
                libtender::domMod::tender::ITenderPtr tender = getTenderCollectionDM()->getTender( yitTender );
                basar::Int32  tenderNo = tender->getHeader().getInt32( properties::TENDER_NO );

                if( constants::REGION_CH == m_Area )
                {
                    basar::Int32 articleNo = 0;
                    //basar::Int16 tenderState = tender->getHeader().getInt16( properties::TENDER_STATE );
                    basar::VarString PharmGrpID = tender->getHeader().getString( properties::PHARMACY_GROUPID );
                    basar::Date endDate = tender->getHeader().getDate( properties::DATE_TO );
                    basar::db::aspect::AccessorPropertyTable_YIterator yitTenderPos= tender->getPositionCollection()->get().begin( );
                    if( !yitTenderPos.isEnd() ) 
                    {
                        articleNo = yitTenderPos.getInt32( properties::ARTICLE_NO );
                    }
                    validateTenderState( articleNo, tenderNo, PharmGrpID, endDate );
                    tender->setTenderState( libtender::domMod::tender::TS_MANUALLY_CLOSED );
                    tender->getHeader().setDate( properties::DATE_TO, basar::Date::getCurrent() );
                    tender->save();
                    getTenderCollectionDM()->findByPattern(	getTenderCollectionDM()->getSearchYit() );
                    getTenderOverviewVC()->matchFromTenderCollection( getTenderCollectionDM()->get() );
                    return basar::appl::EventReturnStruct();
                }

                m_TenderProgress->resetSearchYit();
                m_TenderProgress->getSearchYit().setInt32( properties::TENDER_NO, tenderNo );
                m_TenderProgress->findArticleQuantityByPattern( m_TenderProgress->getSearchYit() );

                if( !m_TenderProgress->get().isEmpty() )
                {
                    getTenderOverviewVC()->displayDeleteNotPossible(
						QApplication::tr("Tender can not be deleted.\nAt least one CSC order has an article from this tender.").toLocal8Bit().constData());
                    return basar::appl::EventReturnStruct();
                }

                getScheduledDeliveryCollectionDM()->findByPattern( tender->getHeader() );
                
                basar::db::aspect::AccessorPropertyTableRef deliveryCollection = getScheduledDeliveryCollectionDM()->get();
                basar::db::aspect::AccessorPropertyTable_YIterator delivery = deliveryCollection.begin();

                while( !delivery.isEnd() )
                {
                    if( basar::cmnutil::Date::getCurrent().getDate() >= delivery.getDate( properties::DELIVERY_DATE ).getDate() )
                    {
						getTenderOverviewVC()->displayDeleteNotPossible(
							QApplication::tr("Tender can not be deleted.\nAt least one delivery has a delivery date which is in the past.").toLocal8Bit().constData());
                        return basar::appl::EventReturnStruct();
                    }
                    ++delivery;
                }

                //Check if a purchase oder proposal has been created (with a orderproposalno!) for a delivery.
                if( isPurchaseOrderExisting(deliveryCollection) )
                {
                    if( basar::gui::tie::ButtonNo == getTenderOverviewVC()->displayOrderProposalAlreadyCreated() )
                    {
                        return basar::appl::EventReturnStruct();
                    }
                }

                //Delete all existing records in pubatchorderpos table, which have no orderproposalno!
                deletePurchaseOrderProposals(deliveryCollection);
                
                transaction->begin();

                //DELETE DELIVERIES
                if( !deliveryCollection.isEmpty() )
                {
                    //TODO: move to ScheduledDeliveryCollectionDM					
                    basar::db::aspect::AccessorPropertyTable_YIterator yitDelivery = deliveryCollection.begin();

                    while( !yitDelivery.isEnd() )
                    {
                        libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr delivry = getScheduledDeliveryCollectionDM()->getScheduledDelivery( yitDelivery );
                        delivry->getHeader()->get().setPropertyState( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID, basar::SS_UPDATE );

                        basar::db::aspect::AccessorPropertyTable_YIterator deliveryPosition = delivry->getPositionCollection()->get().begin();

                        for( ; !deliveryPosition.isEnd(); ++deliveryPosition )
                        {
                            delivry->getPositionCollection()->get().markForDelete( deliveryPosition );
                        }

                        delivry->save();
                        ++yitDelivery;
                    }
                }

                //DELETE TENDER
                tender->getHeader().setPropertyState( properties::TENDER_NO.getName(),    basar::SS_DELETE );

                //basar::db::aspect::AccessorPropertyTableRef tenderPositions = tender->getPositionCollection()->get();
                basar::db::aspect::AccessorPropertyTable_YIterator position = tender->getPositionCollection()->get().begin();

                while( position != tender->getPositionCollection()->get().end() )
                {
                    position.setPropertyState( properties::TENDER_NO,  basar::SS_DELETE );
                    position.setPropertyState( properties::ARTICLE_NO, basar::SS_DELETE );

                    ++position;
                }

                tender->deleteTender();

                transaction->commit();

                getTenderCollectionDM()->findByPattern(	getTenderCollectionDM()->getSearchYit() );
                getTenderOverviewVC()->matchFromTenderCollection( getTenderCollectionDM()->get() );
            }
            catch( const basar::Exception & e )
            {
                transaction->rollback();

                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }

            return basar::appl::EventReturnStruct();
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, CloneTenderRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, CloneTenderRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            libtender::domMod::tender::ITenderPtr tenderToBeCloned = m_TenderCollection->getTender( getTenderOverviewVC()->getCurrentTender() );
            basar::db::aspect::AccessorPropertyTable_YIterator tenderHeader = tenderToBeCloned->getHeader();

            //GET ALL DELIVERIES WHICH ARE IN THE FUTURE OF THIS TENDER
            basar::Int16 year	= basar::Date::getCurrent().getYear();
            basar::Int16 month	= basar::Date::getCurrent().getMonth();
            basar::Int16 day	= basar::Date::getCurrent().getDay();

            //Check if today is the last day of the month and adjust the date to tomorrow
            if( day == basar::Date::getDaysOfMonth(year, month) )
            {
                month = month + 1;
                day = 1;
            }
            else
            {
                day = day + 1;
            }

			//Set dateto to current date, if it is in the past
			if(basar::Date::getCurrent() > tenderHeader.getDate(properties::DATE_FROM))
			{
				tenderHeader.setDate(properties::DATE_FROM, basar::Date::getCurrent());
			}

			//Just in case that the DATE_TO is in the past...
			//In Switzerland we are not able to adjust the DATE_TO manually, it will be calculated if the tender state goes from 0 -> 1 
			if(basar::Date::getCurrent() >= tenderHeader.getDate(properties::DATE_TO))
			{
				basar::Date newDateTo = basar::Date(basar::Date::getCurrent().getYear(), 12, 31);
				tenderHeader.setDate(properties::DATE_TO, newDateTo);

				getTenderOverviewVC()->displayEndDateSetToLastDateOfTheYear();
			}

            m_ScheduledDeliveryCollection->getSearchYit().setInt32( properties::TENDER_ID, tenderToBeCloned->getHeader().getInt32( properties::TENDER_NO ) );
            m_ScheduledDeliveryCollection->getSearchYit().setDate( properties::DELIVERY_DATE_FROM, basar::Date(year, month, day) );
            m_ScheduledDeliveryCollection->findByPattern( m_ScheduledDeliveryCollection->getSearchYit() );

            basar::db::aspect::AccessorPropertyTable_YIterator scheduleHead = m_ScheduledDeliveryCollection->get().begin();

            //TENDER HEADER
            tenderHeader.setPropertyState( properties::TENDER_NO,		  basar::SS_UNSET  );
            tenderHeader.setPropertyState( properties::TENDER_STATE,	  basar::SS_UNSET  );
            tenderHeader.setPropertyState( properties::CUSTOMER_NO,		  basar::SS_UNSET  );
            tenderHeader.setPropertyState( properties::ACC_CUSTOMER_NO,	  basar::SS_UNSET  );
            tenderHeader.setPropertyState( properties::PHARMACY_NAME,	  basar::SS_UNSET  );
            tenderHeader.setPropertyState( properties::PERSONAL_NR_VERTR, basar::SS_UNSET  );
            tenderHeader.setPropertyState( properties::BRANCH_NO,		  basar::SS_UNSET  );
            tenderHeader.setPropertyState( properties::ACC_BRANCH_NO,	  basar::SS_UNSET  );
            tenderHeader.setPropertyState( properties::PHARMACY_CHAIN_NO, basar::SS_UNSET  );
            tenderHeader.setPropertyState( properties::CUSTOMERGROUPID,	  basar::SS_UNSET  );

            tenderHeader.setPropertyState( properties::TENDER_TYPE,		  basar::SS_INSERT );
            tenderHeader.setPropertyState( properties::CONTRACT_NO,		  basar::SS_INSERT );
            tenderHeader.setPropertyState( properties::PARTNER_NO,		  basar::SS_INSERT );
            tenderHeader.setPropertyState( properties::DATE_FROM,		  basar::SS_INSERT );
            tenderHeader.setPropertyState( properties::DATE_TO,			  basar::SS_INSERT );
            tenderHeader.setPropertyState( properties::EXCEEDANCE_PCT,	  basar::SS_INSERT );

            //TENDER POSITIONS
            basar::db::aspect::AccessorPropertyTable_YIterator article = tenderToBeCloned->getPositionCollection()->get().begin();

            for( ; !article.isEnd(); ++article )
            {
                article.setPropertyState( properties::TENDER_NO,		basar::SS_UNSET  );
                                                                          
                article.setPropertyState( properties::ARTICLE_NO,		basar::SS_INSERT );
                article.setPropertyState( properties::ARTICLE_CODE,		basar::SS_INSERT );
                article.setPropertyState( properties::CONTRACT_QTY,		basar::SS_INSERT );
                article.setPropertyState( properties::CONTRACT_PRICE,   basar::SS_INSERT );
                article.setPropertyState( properties::ADD_DISCOUNT_PCT,	basar::SS_INSERT );

                if (constants::REGION_CH == m_Area)
                {
                    article.setPropertyState(properties::DISCOUNT_CALC_FROM, basar::SS_INSERT);
                    article.setPropertyState(properties::DISCOUNT_APPLY_TO,  basar::SS_INSERT);
                }
                else
                {
                    article.setInt16(properties::RECALL_FLAG, 0);
                    article.setInt16(properties::OWNQUOTA_FLAG, 0);
                    article.setInt32(properties::OWNQUOTA_QTY, 0);
                }
            }

            if( (constants::REGION_BG == m_Area && 2 == tenderHeader.getInt32(properties::TENDER_TYPE)) || 
                (constants::REGION_RS == m_Area ))//0 != tenderHeader.getInt32(properties::PHARMACY_CHAIN_NO)) )
            {
                getUseCaseGetter()->getTenderMaintainUC( tenderToBeCloned )->run();
            }
            else if( !scheduleHead.isEnd() ) 
            {
                switch( getTenderOverviewVC()->displayCloneDeliveriesQuestion() )
                {
                case basar::gui::tie::ButtonYes: 
                    getUseCaseGetter()->getTenderMaintainUC( tenderToBeCloned, m_ScheduledDeliveryCollection )->run();
                    break;
                case basar::gui::tie::ButtonNo:
                    getUseCaseGetter()->getTenderMaintainUC( tenderToBeCloned )->run();
                    break;
                default:
                    return basar::appl::EventReturnStruct();
                }
            }
            else
            {
                getUseCaseGetter()->getTenderMaintainUC( tenderToBeCloned )->run();
            }

            getTenderCollectionDM()->findByPattern( getTenderCollectionDM()->getSearchYit() );
            getTenderOverviewVC()->matchFromTenderCollection( getTenderCollectionDM()->get() );

            return basar::appl::EventReturnStruct();
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, NewTenderRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, NewTenderRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;
            
            try
            {
                basar::db::aspect::AccessorPropertyTable_YIterator yitTender;
                libtender::domMod::tender::ITenderPtr tender = getTenderCollectionDM()->getTender( yitTender );

                getUseCaseGetter()->getTenderMaintainUC( tender )->run();

                getTenderCollectionDM()->findByPattern(	getTenderCollectionDM()->getSearchYit() );
                getTenderOverviewVC()->matchFromTenderCollection( getTenderCollectionDM()->get() );

                //getTenderOverviewVC()->selectTender( tender->getHeader() );
            }
            catch( const basar::Exception & e )
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }
        
            return basar::appl::EventReturnStruct();
        }
    
        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, SearchArticleRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, SearchArticleNoRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            try
            {
                getTenderOverviewVC()->matchToSearchCriteria( getTenderCollectionDM()->getSearchYit() );
                
                tender::useCase::IPickerUCPtr articlePickerUC;

                articlePickerUC = getUseCaseGetter()->getArticlePickerUC( getTenderCollectionDM()->getSearchYit() );
                articlePickerUC->run();

                getTenderOverviewVC()->setArticleCode( articlePickerUC->getPicked() );

                getTenderCollectionDM()->resetSearchYit();
            }
            catch( const basar::Exception & e )
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }

            return basar::appl::EventReturnStruct();
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, SearchCustomerRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, SearchCustomerRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            try
            {
                getTenderOverviewVC()->matchToSearchCriteria( getTenderCollectionDM()->getSearchYit() );

                tender::useCase::IPickerUCPtr customerPickerUC;

                customerPickerUC = getUseCaseGetter()->getCustomerPickerUC( getTenderCollectionDM()->getSearchYit() );
                customerPickerUC->run();

                getTenderOverviewVC()->setCustomerNo( customerPickerUC->getPicked() );

                getTenderCollectionDM()->resetSearchYit();
            }
            catch( const basar::Exception & e)
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }

            return basar::appl::EventReturnStruct();
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, SearchPartnerRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, SearchPartnerRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            try
            {
                getTenderOverviewVC()->matchToSearchCriteria( getTenderCollectionDM()->getSearchYit() );

                tender::useCase::IPickerUCPtr partnerPickerUC;
                
                partnerPickerUC = getUseCaseGetter()->getPartnerPickerUC( getTenderCollectionDM()->getSearchYit() );
                partnerPickerUC->run();

                getTenderOverviewVC()->setPartnerNo( partnerPickerUC->getPicked() );

                getTenderCollectionDM()->resetSearchYit();
            }
            catch( const basar::Exception & e)
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }

            return basar::appl::EventReturnStruct();
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, SearchSupplierRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, SearchSupplierRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            try
            {
                getTenderOverviewVC()->matchToSearchCriteria( getTenderCollectionDM()->getSearchYit() );

                tender::useCase::IPickerUCPtr supplierPickerUC;
                supplierPickerUC = getUseCaseGetter()->getSupplierPickerUC( getTenderCollectionDM()->getSearchYit() );
                supplierPickerUC->run();
                getTenderOverviewVC()->setSupplierNo( supplierPickerUC->getPicked( ) );
                getTenderCollectionDM()->resetSearchYit();
            }
            catch( const basar::Exception & e)
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }

            return basar::appl::EventReturnStruct();
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( TenderOverviewUC, SearchPharmacyChainRequested )
        {
            METHODNAME_DEF( TenderOverviewUC, SearchPharmacyChainRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            try
            {
                getTenderOverviewVC()->matchToSearchCriteria( getTenderCollectionDM()->getSearchYit() );

                tender::useCase::IPickerUCPtr pharmacyChainPickerUC;
                
                pharmacyChainPickerUC = getUseCaseGetter()->getPharmacyChainPickerUC( getTenderCollectionDM()->getSearchYit() );
                pharmacyChainPickerUC->run();

                getTenderOverviewVC()->setPharmacyChainNo( pharmacyChainPickerUC->getPicked() );

                getTenderCollectionDM()->resetSearchYit();
            }
            catch( const basar::Exception & e)
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }

            return basar::appl::EventReturnStruct();
        }

    } // namespace usecase
} // namespace tender
