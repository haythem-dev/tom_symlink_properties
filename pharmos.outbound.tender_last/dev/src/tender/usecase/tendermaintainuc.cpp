#include "tendermaintainuc.h"
#include "tendermaintainvc.h"
#include "iusecasegetter.h"
#include "omgorderucptr.h"
#include "omgorderuc.h"
#include "itenderstateuc.h"
#include "statevalidationenum.h"
#include "libbasarproperty_propertydescription.h"

#include <loggerpool/tenderloggerpool.h>
#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>
#include <libbasardbaspect_exceptions.h>
#include <libbasarlogin.h>
#include <libtender/domainmodule/tender/itender.h>
#include <libtender/domainmodule/tender/itenderpositioncollection.h>
#include <libtender/domainmodule/tender/tenderstatecollection.h>
#include <libtender/domainmodule/tender/tendertypecollection.h>
#include <libtender/domainmodule/article/businesstypemapper.h>
#include <libtender/domainmodule/constants.h>
#include <libtender/domainmodule/libtender_properties_definitions.h>
#include <libtender/domainmodule/article/iarticlepricedm.h>
#include <libtender/domainmodule/customerlist/icustomerlistdm.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverycollectiondm.h>
#include <libtender/domainmodule/scheduleddelivery/ischedule.h>
#include <libtender/domainmodule/scheduleddelivery/schedule.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryheader.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverypositioncollection.h>
#include <libtender/domainmodule/purchaseorderproposal/ipurchaseorderproposal.h>
#include <domainmodule/tender/itendercollectiondm.h>
#include <usecase/ipickeruc.h>


namespace tender
{
namespace useCase
{

BEGIN_HANDLERS_REGISTRATION( TenderMaintainUC )
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, SaveTenderRequested					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, SaveTenderPositionRequested			)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, SaveAndNewTenderPositionRequested		)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, ModifyTenderPositionRequested			)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CancelModifyTenderPositionRequested	)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, DeleteTenderPositionRequested			)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, SearchArticleRequested					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CheckArticleRequested					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, SearchCustomerRequested				)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CheckCustomerRequested					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, SearchPharmacyGroupRequested           )
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CheckPharmacyGroupRequested            )
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, SearchPartnerRequested					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CheckPartnerRequested					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, SearchPharmacyChainRequested			)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CheckPharmacyChainRequested			)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, AddDeliveryRequested					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CreateCSCOrdersRequested               )
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, ModifyDeliveryRequested				)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, DeleteDeliveryRequested				)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, DeliverySelectionChangeRequested		)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, ScheduleRequested						)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, ChangeStateToClosedRequested			)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, ChangeStateFromClosedRequested			)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CheckIfPositionsWereModifiedRequested	)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CloneDeliveryRequested					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CheckIfTenderWasModifiedRequested		)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CheckIfContractPeriodIsValid			)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, ContractInfoChanged					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CustomerListRequested					)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, CheckCustomerListRequested				)
    SYSTEM_EVENT_HANDLER_REGISTRATION( TenderMaintainUC, ProtocolsRequested						)
END_HANDLERS_REGISTRATION()

TenderMaintainUC::TenderMaintainUC()
: m_Logger( libtender::LoggerPool::getLoggerUseCases() ),
  m_ModifyExistingPosition( false ),
  m_CloneDeliveries( false )
{
    METHODNAME_DEF( TenderMaintainUC, TenderMaintainUC )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

TenderMaintainUC::~TenderMaintainUC()
{
    METHODNAME_DEF( TenderMaintainUC, ~TenderMaintainUC )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

void TenderMaintainUC::injectTenderMaintainVC ( tender::viewConn::TenderMaintainVCPtr tenderMaintainVC )
{
    METHODNAME_DEF( TenderMaintainUC, injectTenderMaintainVC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_TenderMaintainVC = tenderMaintainVC;
}

void TenderMaintainUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
{
    METHODNAME_DEF( TenderMaintainUC, injectUseCaseGetter )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_UseCaseGetter = useCaseGetter;
}

void TenderMaintainUC::injectPurchaseOrderProposalDM( libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr purchaseOrderProposal )
{
    METHODNAME_DEF( TenderMaintainUC, injectPurchaseOrderProposalDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_PurchaseOrderProposal = purchaseOrderProposal;
}

void TenderMaintainUC::injectOMGOrderUC(tender::useCase::OMGOrderUCPtr omgucPtr)
{
    METHODNAME_DEF( TenderMaintainUC, injectOMGOrderUC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_OMGOrderUC = omgucPtr;
}

void TenderMaintainUC::injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM )
{
    METHODNAME_DEF( TenderMaintainUC, injectTenderCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_TenderCollection = tenderCollectionDM;
}

void TenderMaintainUC::injectTenderStateUC( tender::useCase::ITenderStateUCPtr tenderStateUC )
{
    METHODNAME_DEF( TenderMaintainUC, injectTenderStateUC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_TenderStateUC = tenderStateUC;
}

libtender::domMod::tender::ITenderCollectionDMPtr TenderMaintainUC::getTenderCollectionDM()
{
    METHODNAME_DEF( TenderMaintainUC, getTenderCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    CHECK_INSTANCE_EXCEPTION( m_TenderCollection.get() );
    return m_TenderCollection;
}

void TenderMaintainUC::injectArticlePriceDM( libtender::domMod::article::IArticlePricePtr articlePrice )
{
    METHODNAME_DEF( TenderMaintainUC, injectArticlePriceDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_ArticlePrice = articlePrice;
}

void TenderMaintainUC::setArea(basar::Int16 area)
{
    m_Area = area;
}

void TenderMaintainUC::setTender(libtender::domMod::tender::ITenderPtr tender)
{
    METHODNAME_DEF( TenderMaintainUC, setTender )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_Tender = tender;
}

void TenderMaintainUC::setSchedule( libtender::domMod::scheduledDelivery::ISchedulePtr schedule )
{
    METHODNAME_DEF( TenderMaintainUC, setSchedule )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_Schedule = schedule;
}

void TenderMaintainUC::setCloneDeliveriesFlag(bool cloneDeliveries)
{
    m_CloneDeliveries = cloneDeliveries;
    getTenderMaintainVC()->setCloneDeliveriesFlag(m_CloneDeliveries);
}

tender::viewConn::TenderMaintainVCPtr TenderMaintainUC::getTenderMaintainVC()
{
    METHODNAME_DEF( TenderMaintainUC, getTenderMaintainVC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    CHECK_INSTANCE_EXCEPTION( m_TenderMaintainVC.get() );
    return m_TenderMaintainVC;
}

tender::componentManager::IUseCaseGetterPtr TenderMaintainUC::getUseCaseGetter()
{
    METHODNAME_DEF( TenderMaintainUC, getUseCaseGetter )
    BLOG_TRACE_METHOD( m_Logger, fun );

    CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
    return m_UseCaseGetter;
}

const log4cplus::Logger& TenderMaintainUC::getLogger() const
{
    METHODNAME_DEF( TenderMaintainUC, getLogger )
    BLOG_TRACE_METHOD( m_Logger, fun );

    return m_Logger;
}

void TenderMaintainUC::init()
{
    METHODNAME_DEF( TenderMaintainUC, init )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

void TenderMaintainUC::reset()
{
    METHODNAME_DEF( TenderMaintainUC, reset )
    BLOG_TRACE_METHOD( m_Logger, fun );

    init();
}

void TenderMaintainUC::run()
{
    METHODNAME_DEF( TenderMaintainUC, run )
    BLOG_TRACE_METHOD( m_Logger, fun );

    registerEventHandlers( true );
    init();
    getTenderMaintainVC()->reset();

    if( NULL != m_Tender.get() )
    {
        getTenderMaintainVC()->matchFromTender( m_Tender );
        m_Tender->getCustomerList()->findByTenderNo( m_Tender->getHeader() );
    }

    if( !m_Schedule->getScheduledDeliveryCollection()->get().isEmpty() )
    {
        m_CloneDeliveries = true;
        getTenderMaintainVC()->setCloneDeliveriesFlag(m_CloneDeliveries);
    }
    
    getTenderMaintainVC()->show();
    registerEventHandlers( false );
}

bool TenderMaintainUC::isQuantityUndershot( basar::db::aspect::AccessorPropertyTable_YIterator positionToBeModified )
{
    METHODNAME_DEF( TenderMaintainUC, isQuantityUndershot )
    BLOG_TRACE_METHOD( m_Logger, fun );

    int quantity = 0;
    getTenderMaintainVC()->matchToTenderPosition( positionToBeModified );

    if( m_Tender->getHeader().isContainedAndSet( properties::TENDER_NO ) )
    {
        m_Schedule->getScheduledDeliveryCollection()->findByPattern( m_Tender->getHeader() );
    }
    
    //TODO: move to ScheduledDeliveryCollectionDM
    basar::db::aspect::AccessorPropertyTable_YIterator deliveryYit = m_Schedule->getScheduledDeliveryCollection()->get().begin();

    if( deliveryYit.isEnd() )
    {
        return false;
    }

    for( ; !deliveryYit.isEnd(); ++deliveryYit )
    {
        libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr delivery =  m_Schedule->getScheduledDeliveryCollection()->getScheduledDelivery( deliveryYit );
        basar::db::aspect::AccessorPropertyTable_YIterator position = delivery->getPositionCollection()->get().begin();
        
        for( ; !position.isEnd(); ++position )
        {
            if( positionToBeModified.getString( properties::ARTICLE_CODE ) == position.getString( properties::ARTICLE_CODE ) )
            {
                quantity = quantity + position.getInt32( properties::QUANTITY );
            }
        }
    }
    
    if( quantity > positionToBeModified.getInt32( properties::CONTRACT_QTY ) )
    {
        return true;
    }

    return false;
}

basar::appl::EventReturnType TenderMaintainUC::AddPosition(basar::db::aspect::AccessorPropertyTable_YIterator position)
{
    METHODNAME_DEF(TenderMaintainUC, AddPosition)
    BLOG_TRACE_METHOD(getLogger(), fun);
    
    std::string articleCode = getTenderMaintainVC()->getArticleToBeAdded();

    for(; !position.isEnd(); ++position)
    {
        if(articleCode == position.getString(properties::ARTICLE_CODE))
        {
            if(basar::SS_DELETE != position.getState(properties::ARTICLE_CODE).getState())
            {
                getTenderMaintainVC()->displayArticleAlreadySelected();
                return basar::appl::EventReturnType(basar::appl::HANDLER_ERROR);
            }
            
            getTenderMaintainVC()->matchToTenderPosition(position);
            
            if( position.isContainedAndSet(properties::TENDER_NO))
            {
                position.setPropertyState(properties::TENDER_NO,	basar::SS_UPDATE);
            }

            position.setPropertyState(properties::ARTICLE_NO,		basar::SS_UPDATE);
            position.setPropertyState(properties::ARTICLE_CODE,		basar::SS_UPDATE);
            position.setPropertyState(properties::ARTICLE_NAME,		basar::SS_UPDATE);
            position.setPropertyState(properties::CONTRACT_QTY,		basar::SS_UPDATE);
            position.setPropertyState(properties::CONTRACT_PRICE,	basar::SS_UPDATE);
            position.setPropertyState(properties::ADD_DISCOUNT_PCT,	basar::SS_UPDATE);
            position.setPropertyState(properties::NAME_OF_USER,		basar::SS_UPDATE);

            if( position.isContainedAndSet( properties::DISCOUNT_CALC_FROM) )
            {
                position.setPropertyState( properties::DISCOUNT_CALC_FROM, basar::SS_UPDATE );
            }

            if (position.isContainedAndSet(properties::DISCOUNT_APPLY_TO))
            {
                position.setPropertyState(properties::DISCOUNT_APPLY_TO, basar::SS_UPDATE);
            }

            if (position.isContainedAndSet(properties::OWNQUOTA_FLAG))
            {
                position.setPropertyState(properties::OWNQUOTA_FLAG, basar::SS_UPDATE);
            }

            if( position.isContainedAndSet( properties::OWNQUOTA_QTY ) )
            {
                position.setPropertyState( properties::OWNQUOTA_QTY,   basar::SS_UPDATE );
            }

            if( position.isContainedAndSet( properties::RECALL_FLAG ) )
            {
                position.setPropertyState( properties::RECALL_FLAG,   basar::SS_UPDATE );
            }

            if( position.isContainedAndSet(properties::QUANTITY_AVAILABLE))
            {
                position.setPropertyState(properties::QUANTITY_AVAILABLE, basar::SS_UPDATE);	
            }

            getTenderMaintainVC()->matchFromTender( m_Tender );
            m_ModifyExistingPosition = false;

            return basar::appl::EventReturnType(basar::appl::HANDLER_OK);
        }		
    }

    basar::db::aspect::AccessorPropertyTable_YIterator tenderPosYit =   m_Tender->addEmptyPosition();
    if( m_Tender->getHeader().isContainedAndSet( properties::RECALL_FLAG ) )
    {
        tenderPosYit.setInt16(properties::RECALL_FLAG,m_Tender->getHeader().getInt16(properties::RECALL_FLAG));
    }

    getTenderMaintainVC()->matchToTenderPosition( tenderPosYit );
    getPricesForArticle( tenderPosYit );
    getTenderMaintainVC()->matchFromTender( m_Tender );
    getTenderMaintainVC()->matchFromTenderPositions( m_Tender->getPositionCollection()->get() );
    m_ModifyExistingPosition = false;

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

basar::appl::EventReturnType TenderMaintainUC::ModifyPosition(basar::db::aspect::AccessorPropertyTable_YIterator positionToModify)
{
    METHODNAME_DEF(TenderMaintainUC, ModifyPosition)
    BLOG_TRACE_METHOD(getLogger(), fun);

    int oldQuantity = positionToModify.getInt32(properties::CONTRACT_QTY);
    if( isQuantityUndershot(positionToModify))
    {
        getTenderMaintainVC()->displayConractQuantityUndershot(positionToModify.getInt32(properties::CONTRACT_QTY));
    
        positionToModify.setInt32(properties::CONTRACT_QTY, oldQuantity);
        return basar::appl::EventReturnType(basar::appl::HANDLER_ERROR);
    }

    getPricesForArticle(positionToModify);

    getTenderMaintainVC()->matchToTenderPosition(positionToModify);
    getTenderMaintainVC()->matchFromTender( m_Tender );
    getTenderMaintainVC()->matchFromTenderPositions(m_Tender->getPositionCollection()->get());

    m_ModifyExistingPosition = false;

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

void TenderMaintainUC::getPricesForArticle(basar::db::aspect::AccessorPropertyTable_YIterator yitArticle)
{
    METHODNAME_DEF( TenderMaintainUC, getPricesForArticle )
    BLOG_TRACE_METHOD( getLogger(), fun );

    basar::db::aspect::AccessorPropertyTable_YIterator articlePrices;

    //Get AEP and (if contract price is set) price incl. VAT
    m_ArticlePrice->findByArticleCode(yitArticle);

    //Set AEP and price incl. VAT 
    articlePrices = m_ArticlePrice->get().begin();

    yitArticle.setDecimal( properties::AEP,			   articlePrices.getDecimal( properties::AEP			) );
    yitArticle.setDecimal( properties::PRICE_INCL_VAT, articlePrices.getDecimal( properties::PRICE_INCL_VAT ) );
}

void TenderMaintainUC::saveClonedDeliveries()
{
    METHODNAME_DEF( TenderMaintainUC, saveClonedDeliveries )
    BLOG_TRACE_METHOD( m_Logger, fun );

    basar::db::aspect::AccessorPropertyTable_YIterator deliveryHead = m_Schedule->getScheduledDeliveryCollection()->get().begin();

    for( ; !deliveryHead.isEnd(); ++deliveryHead )
    {
        libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr delivery = m_Schedule->getScheduledDeliveryCollection()->getScheduledDelivery( deliveryHead );

        delivery->getHeader()->get().setInt32( properties::TENDER_ID, m_Tender->getHeader().getInt32( properties::TENDER_NO ) );
        delivery->getHeader()->get().setInt16( properties::BRANCH_NO, m_Tender->getHeader().getInt16( properties::BRANCH_NO ) );

        delivery->getHeader()->get().setPropertyState( properties::TENDER_ID,						 basar::SS_INSERT );
        delivery->getHeader()->get().setPropertyState( properties::DELIVERY_DATE,					 basar::SS_INSERT );
        delivery->getHeader()->get().setPropertyState( properties::CUSTOMER_NO,						 basar::SS_INSERT );

        delivery->getHeader()->get().setPropertyState( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID, basar::SS_UNSET );
        delivery->getHeader()->get().setPropertyState( properties::CSC_ORDER_NO,					 basar::SS_UNSET );
        delivery->getHeader()->get().setPropertyState( properties::CSC_ORDER_DATE,				     basar::SS_UNSET );
        delivery->getHeader()->get().setPropertyState( properties::LAST_CHANGED_DATETIME,			 basar::SS_UNSET );
        delivery->getHeader()->get().setPropertyState( properties::CREATION_DATETIME,				 basar::SS_UNSET );
        
        basar::db::aspect::AccessorPropertyTable_YIterator article = delivery->getPositionCollection()->get().begin();
        
        for( ; !article.isEnd(); ++article )
        {
            article.setPropertyState( properties::QUANTITY_DELIVERED,				basar::SS_UNSET  );
            article.setPropertyState( properties::LAST_CHANGED_DATETIME,			basar::SS_UNSET  );
            article.setPropertyState( properties::CREATION_DATETIME,				basar::SS_UNSET  );
            article.setPropertyState( properties::ORDER_PROPOSAL_NO,				basar::SS_UNSET  );

            article.setPropertyState( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID, basar::SS_INSERT );
            article.setPropertyState( properties::ARTICLE_CODE,						basar::SS_INSERT );
            article.setPropertyState( properties::QUANTITY,							basar::SS_INSERT );
            article.setPropertyState( properties::PURCHASE_ORDER_PROPOSAL_FLAG,		basar::SS_INSERT );

        }

        YitList posList = getDeliveryPositionsToSave(delivery);
        delivery->save();
        transferDeliveryPositionsToPurchase(posList, delivery);
    }
}

TenderMaintainUC::YitList TenderMaintainUC::getDeliveryPositionsToSave(libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr delivery)
{
    YitList changeList;
    basar::db::aspect::AccessorPropertyTableRef tabPos = delivery->getPositionCollection()->get();

    basar::db::aspect::AccessorPropertyTable_YIterator yitPos;

    for (yitPos = tabPos.begin(); !yitPos.isEnd(); ++yitPos)
    {
        if( 0 == yitPos.getInt16( properties::PURCHASE_ORDER_PROPOSAL_FLAG ) )
        {
            continue;
        }

        changeList.push_back(yitPos);
    }

    return changeList;
}

void TenderMaintainUC::transferDeliveryPositionsToPurchase(const YitList& changedList, libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr delivery)
{
    METHODNAME_DEF( TenderMaintainUC, transferDeliveryPositionsToPurchase )
    BLOG_TRACE_METHOD( m_Logger, fun );

    basar::db::aspect::AccessorPropertyTable_YIterator yitPos;
    YitList::const_iterator iterator = changedList.begin();

    while(iterator != changedList.end())
    {
        yitPos = *iterator;

        m_PurchaseOrderProposal->setBranchNo(yitPos.getInt16( properties::BRANCH_NO_POP ));
        m_PurchaseOrderProposal->setArticleNo(yitPos.getInt32(properties::ARTICLE_NO));
        m_PurchaseOrderProposal->setOrderNoBatch(delivery->getHeader()->getScheduledDeliveryId());
        m_PurchaseOrderProposal->setOrderTypeBatch(constants::ORDER_TYPE_BATCH);
        m_PurchaseOrderProposal->setOrderProposalQuantity(yitPos.getInt32(properties::QUANTITY));
        m_PurchaseOrderProposal->setExpectedGoodsinDate(delivery->getHeader()->getDeliveryDate());
        m_PurchaseOrderProposal->setSupplierNo(0); //Supplier is determined trough the purchase batch!

        m_PurchaseOrderProposal->save();
        m_PurchaseOrderProposal->clear();

        ++iterator;
    }
}

void TenderMaintainUC::deletePurchaseOrderProposals(libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr delivery)
{
    METHODNAME_DEF(TenderMaintainUC, deletePurchaseOrderProposals)
    BLOG_TRACE_METHOD(getLogger(), fun);

    basar::db::aspect::AccessorPropertyTable_YIterator articleYit;
    articleYit = delivery->getPositionCollection()->get().begin();

    while( !articleYit.isEnd() ) 
    {
        m_PurchaseOrderProposal->resetSearchYit();
        m_PurchaseOrderProposal->getSearchYit().setInt32(properties::ORDER_NO_BATCH,	delivery->getHeader()->get().getInt32(properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID));
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
}

bool TenderMaintainUC::needsCustomerSelection()
{
    METHODNAME_DEF(TenderMaintainUC, needsCustomerSelection)
    BLOG_TRACE_METHOD(getLogger(), fun);

    if( (constants::REGION_BG && 2 == m_Tender->getHeader().getInt32(properties::TENDER_TYPE)) || 
        (constants::REGION_RS && 0 != m_Tender->getHeader().getInt32(properties::PHARMACY_CHAIN_NO)) )
    {
        return true;
    }

    return false;
}

bool TenderMaintainUC::hasCustomerListChanged(basar::db::aspect::AccessorPropertyTableRef customers)
{
    basar::db::aspect::AccessorPropertyTable_YIterator customer = customers.begin();
    
    using namespace basar::property;
    
    PropertyStateSet stateSet;
    stateSet.insert(basar::SS_INSERT);
    
    while(!customer.isEnd())
    {
        PropertyStateSet statesetemp = customer.getState();
    
        if(customer.getState().contains(stateSet))
        {
            return true;
        }
        
        ++customer;
    }

    return false;
}

void TenderMaintainUC::setTenderState( basar::Int16 tenderState)
{
    if( m_Tender->getHeader().isContainedAndSet(properties::TENDER_NO) ) // setting tender state only existing tender not for new tender.
    {
        getTenderMaintainVC()->setTenderState( (libtender::domMod::tender::TenderStateEnum)tenderState );
        m_Tender->getHeader().setInt16(properties::TENDER_STATE, tenderState );
    }
}

bool TenderMaintainUC::validateTenderState( const libtender::domMod::tender::TenderStateEnum oldTenderState )
{
    METHODNAME_DEF( TenderMaintainUC, validateTenderState )
    BLOG_TRACE_METHOD( m_Logger, fun );
    try
    {
        basar::db::aspect::AccessorPropertyTable_YIterator yitTenderPos= m_Tender->getPositionCollection()->get().begin();

		if(yitTenderPos.isEnd())
        {
            return false;
        }

        m_TenderStateUC->setTender( m_Tender );
        StateValidationEnum stateEnum = m_TenderStateUC->validateTenderState( static_cast<basar::Int16>( oldTenderState ) );
        
		if(stateEnum != tender::useCase::NONE)
        {
            getTenderMaintainVC()->displayTenderStateWarning( m_TenderStateUC->getValidationErrorMessage( stateEnum ) );
            return false;
        }
    }
    catch( basar::Exception& e )
    {
        throw e;
    }

    return true;
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, SaveTenderRequested )
{
    METHODNAME_DEF( TenderMaintainUC, SaveTenderRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        basar::Int16 tenderState = 0;

        if(constants::REGION_CH == m_Area)
        {  
            if(m_Tender->getHeader().isContainedAndSet(properties::TENDER_STATE))
            {
                tenderState = m_Tender->getHeader().getInt16(properties::TENDER_STATE);
            }
        }
       
        getTenderMaintainVC()->matchToTender(m_Tender);

        if(constants::REGION_CH == m_Area)
        {
            if(libtender::domMod::tender::TS_MANUALLY_CLOSED == m_Tender->getTenderState()
               && tenderState != m_Tender->getTenderState())
            {
                m_Tender->getHeader().setDate(properties::DATE_TO, basar::Date::getCurrent());
            }

			if(libtender::domMod::tender::TS_OK == m_Tender->getTenderState()) 
			{
				m_TenderStateUC->setTender(m_Tender);
				m_Tender->getHeader().setDate(properties::DATE_TO, m_TenderStateUC->getTenderEndDate(m_Tender->getHeader().getDate(properties::DATE_FROM)));
			}
        }

        if( !getTenderMaintainVC()->isTenderHeadFilled() )
        {
            getTenderMaintainVC()->displayTenderHeaderMustBeFilled();
            return basar::appl::EventReturnType( basar::appl::HANDLER_INFO );
        }

        if( m_Tender->getHeader().isContainedAndSet( properties::PARTNER_NO ) && m_Tender->getHeader().getInt32( properties::PARTNER_NO ) != 0 ) 
        {
            if( !handleCheckPartnerRequested( rSource, yitFromEvSrc ).isHandlerOK() )
            {	
                getTenderMaintainVC()->displayPartnerNotValid();
                return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
            }
        }

        if( m_Tender->getHeader().isContainedAndSet( properties::CUSTOMER_NO ) && m_Tender->getHeader().getInt32( properties::CUSTOMER_NO ) != 0 )
        {
            if( !handleCheckCustomerRequested( rSource, yitFromEvSrc ).isHandlerOK() ) 
            {
                getTenderMaintainVC()->displayCustomerNotValid();
                return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
            }
        }
        else if( m_Tender->getHeader().isContainedAndSet( properties::PHARMACY_CHAIN_NO ) &&
                 0 !=m_Tender->getHeader().getInt32( properties::PHARMACY_CHAIN_NO ) )
        {
            //Checks also if the CustomerGroupID is valid for this pharmacy chain! 
            if( !handleCheckPharmacyChainRequested( rSource, yitFromEvSrc ).isHandlerOK() )
            {
                return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
            }
        }

        if( m_Tender->getHeader().getInt32( properties::TENDER_TYPE ) == libtender::domMod::tender::TT_STATE )
        {
            if( !handleCheckCustomerListRequested( rSource, yitFromEvSrc ).isHandlerOK() )
            {
                getTenderMaintainVC()->displayCustomerListInvalid();
                return basar::appl::EventReturnType( basar::appl::HANDLER_INFO );
            }
        }
        
        if( !basar::appl::SystemEventManager::getInstance().fire( "CheckIfContractPeriodIsValid" ).isHandlerOK() )
        {
            return basar::appl::EventReturnType( basar::appl::HANDLER_INFO );
        }
        
        if( constants::REGION_CH == m_Area )
        {
            if( m_Tender->getHeader().isContainedAndSet( properties::PHARMACY_GROUPID ) && (false == m_Tender->getHeader().getString( properties::PHARMACY_GROUPID ).empty()) )
            {
                if( !handleCheckPharmacyGroupRequested( rSource, yitFromEvSrc ).isHandlerOK() ) 
                {
                    getTenderMaintainVC()->displayPharmacyGroupNotValid();
                    return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
                }
            }

            if( validateTenderState( static_cast<libtender::domMod::tender::TenderStateEnum>( tenderState ) ) == false )
            {
                setTenderState( tenderState );
                return basar::appl::EventReturnType( basar::appl::HANDLER_INFO );
            }

            basar::db::aspect::AccessorPropertyTable_YIterator position = m_Tender->getPositionCollection()->get().begin();
            basar::Int32 contractQty = position.getInt32( properties::CONTRACT_QTY );
            basar::Int32 ownQuotaQty = position.getInt32( properties::OWNQUOTA_QTY );
            
			if( contractQty < ownQuotaQty )
            {
                getTenderMaintainVC()->showErrorMessage( QCoreApplication::translate( "TenderMaintainUC", "Own quota quantity can't exceed contract quantity!" ).toLocal8Bit().constData() );
                return basar::appl::EventReturnType( basar::appl::HANDLER_ERROR );
            }
            
			if(1 == position.getInt16(properties::OWNQUOTA_FLAG) && contractQty == ownQuotaQty)
            {
                position.setInt16( properties::TENDER_STATE, libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED );
                m_Tender->getHeader().setInt16( properties::TENDER_STATE, libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED );
                getTenderMaintainVC()->showInfoMessage( QCoreApplication::translate( "TenderMaintainUC", "Own quota quantity equals contract quantity, Tender will be closed automatically." ).toLocal8Bit().constData() );
            }
        }

        if(m_CloneDeliveries)
        {
            m_Tender->save();
            saveClonedDeliveries();
            m_CloneDeliveries = false;
            getTenderMaintainVC()->setCloneDeliveriesFlag(m_CloneDeliveries);
        } 
        else
        {
            m_Tender->save();
        }

        getTenderMaintainVC()->matchFromTender(m_Tender);
        getTenderMaintainVC()->matchFromTenderPositions(m_Tender->getPositionCollection()->get());

    }
    catch(const basar::Exception & e)
    {
        BLOG_ERROR(m_Logger, e.what().c_str());
        getTenderMaintainVC()->showErrorMessage(QCoreApplication::translate("TenderMaintainUC", "Error saving Tender!").toLocal8Bit().constData());
        throw;
    }

    return basar::appl::EventReturnType(basar::appl::HANDLER_OK);
}

SYSTEM_EVENT_HANDLER_DEFINITION(TenderMaintainUC, SaveTenderPositionRequested)
{
    METHODNAME_DEF(TenderMaintainUC, SaveTenderPositionRequested)
    BLOG_TRACE_METHOD(getLogger(), fun);
    rSource;

    try
    {
        if(!handleCheckArticleRequested(rSource, yitFromEvSrc).isHandlerOK()) 
        {
            getTenderMaintainVC()->displayArticleNotExisting();
            return basar::appl::EventReturnType(basar::appl::HANDLER_NOT_FOUND);
        }
        
        getTenderMaintainVC()->matchToTender(m_Tender);

        if(m_ModifyExistingPosition)
        {
            basar::db::aspect::AccessorPropertyTable_YIterator currentPosition = getTenderMaintainVC()->getCurrentTenderPosition();
            return ModifyPosition(currentPosition);
        }
        else
        {
            basar::db::aspect::AccessorPropertyTable_YIterator position = m_Tender->getPositionCollection()->get().begin();
            return AddPosition(position);
        }
    }
    catch(const basar::Exception & e)
    {
        BLOG_ERROR(m_Logger, e.what().c_str());
        throw;
    }
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, SaveAndNewTenderPositionRequested )
{
    METHODNAME_DEF( TenderMaintainUC, SaveAndNewTenderPositionRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {	
        if( !handleSaveTenderPositionRequested( rSource, yitFromEvSrc ).isHandlerOK() ) 
        {
            return basar::appl::EventReturnType( basar::appl::HANDLER_INFO );
        }

        getTenderMaintainVC()->resetTenderPosition();
        getTenderMaintainVC()->enableTenderPositionEditing(false);
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, ModifyTenderPositionRequested )
{
    METHODNAME_DEF( TenderMaintainUC, ModifyTenderPositionRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        getTenderMaintainVC()->matchFromTenderPosition( getTenderMaintainVC()->getCurrentTenderPosition() );
        getTenderMaintainVC()->enableTenderPositionEditing(true);
        m_ModifyExistingPosition = true;
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CancelModifyTenderPositionRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CancelModifyTenderPositionRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        getTenderMaintainVC()->resetTenderPosition();
        m_ModifyExistingPosition = false;
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, DeleteTenderPositionRequested )
{
    METHODNAME_DEF( TenderMaintainUC, DeleteTenderPositionRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        basar::db::aspect::AccessorPropertyTable_YIterator positionToBeDeleted = getTenderMaintainVC()->getCurrentTenderPosition();

        if( constants::REGION_CH == m_Area )
        {
            libtender::domMod::tender::TenderStateEnum oldTenderState = m_Tender->getTenderState();
            m_Tender->setTenderState( libtender::domMod::tender::TS_MANUALLY_CLOSED );
            validateTenderState( oldTenderState );
            m_Tender->getHeader().setDate( properties::DATE_TO, basar::Date::getCurrent() );
            m_Tender->save();
            getTenderMaintainVC()->matchFromTender( m_Tender );
            return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
        }

        //TODO: move to ScheduledDeliveryCollectionDM
        if( m_Tender->getHeader().isContainedAndSet( properties::TENDER_NO ) )
        {
            m_Schedule->getScheduledDeliveryCollection()->findByPattern( m_Tender->getHeader() );
        }

        basar::db::aspect::AccessorPropertyTableRef		   deliveryCollection = m_Schedule->getScheduledDeliveryCollection()->get();
        basar::db::aspect::AccessorPropertyTable_YIterator deliveryYit = deliveryCollection.begin();

        for( ; !deliveryYit.isEnd(); ++deliveryYit )
        {
            libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr delivery = 
                m_Schedule->getScheduledDeliveryCollection()->getScheduledDelivery( deliveryYit );

            basar::db::aspect::AccessorPropertyTableRef		   positionCollection = delivery->getPositionCollection()->get();
            basar::db::aspect::AccessorPropertyTable_YIterator position = positionCollection.begin();

            for( ; !position.isEnd(); ++position )
            {
                if( positionToBeDeleted.getString( properties::ARTICLE_CODE ) == position.getString( properties::ARTICLE_CODE ) )
                {
                    if( !m_Schedule->getScheduledDeliveryCollection()->get().isEmpty() )
                    {
                        getTenderMaintainVC()->displayArticleExistsInDelivery();
                        return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
                    }
                }

            }
        }
        
        m_Tender->deletePosition( positionToBeDeleted );
        getTenderMaintainVC()->matchFromTender( m_Tender );
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, SearchArticleRequested )
{
    METHODNAME_DEF( TenderMaintainUC, SearchArticleRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        tender::useCase::IPickerUCPtr articlePickerUC( getUseCaseGetter()->getArticlePickerUC() );
        getTenderMaintainVC()->matchToArticleSearch( articlePickerUC->getSearchYIterator() );
        articlePickerUC->setSearchCriteria( articlePickerUC->getSearchYIterator() );
        articlePickerUC->run();

        if( !articlePickerUC->getPicked().isNull() )
        {
            getTenderMaintainVC()->matchFromArticle( articlePickerUC->getPicked() );
        }
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckArticleRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CheckArticleRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        tender::useCase::IPickerUCPtr articlePickerUC( getUseCaseGetter()->getArticlePickerUC() );
        getTenderMaintainVC()->matchToArticleSearch( articlePickerUC->getSearchYIterator() );
        articlePickerUC->setSearchCriteria( articlePickerUC->getSearchYIterator() );
        articlePickerUC->setCheckMode(true);
        articlePickerUC->run();

        basar::db::aspect::AccessorPropertyTable_YIterator yitArticle =	articlePickerUC->getPicked();

        if (false == yitArticle.isNull() && false == yitArticle.isEnd())
        {
            getTenderMaintainVC()->matchFromArticle( yitArticle );
        }
        else
        {
            getTenderMaintainVC()->resetArticle();
            articlePickerUC->setCheckMode(false);
            return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
        }

        articlePickerUC->setCheckMode(false);
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, SearchCustomerRequested )
{
    METHODNAME_DEF( TenderMaintainUC, SearchCustomerRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        tender::useCase::IPickerUCPtr customerPickerUC( getUseCaseGetter()->getCustomerPickerUC() );
        getTenderMaintainVC()->matchToCustomerSearch( customerPickerUC->getSearchYIterator() );
        customerPickerUC->setSearchCriteria( customerPickerUC->getSearchYIterator() );
        customerPickerUC->run();

        if( !customerPickerUC->getPicked().isNull() )
        {
            getTenderMaintainVC()->matchFromCustomer( customerPickerUC->getPicked() );
            handleCheckCustomerRequested(rSource, yitFromEvSrc);
        }
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckCustomerRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CheckCustomerRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        tender::useCase::IPickerUCPtr customerPickerUC( getUseCaseGetter()->getCustomerPickerUC() );
        getTenderMaintainVC()->matchToCustomerSearch( customerPickerUC->getSearchYIterator() );
        customerPickerUC->setCheckMode(true);
        customerPickerUC->run();

        basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer = customerPickerUC->getPicked();
        if( false == yitCustomer.isNull() && false == yitCustomer.isEnd() )
        {
            if (yitCustomer.getInt16(properties::ORIGINAL_BRANCH_NO) > 0)
            {
                yitCustomer.setInt16(properties::BRANCH_NO, yitCustomer.getInt16(properties::ORIGINAL_BRANCH_NO));
            }

            getTenderMaintainVC()->matchFromCustomer( yitCustomer );
        }
        else
        {
            getTenderMaintainVC()->resetCustomer();
            customerPickerUC->setCheckMode(false);
            return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
        }

        customerPickerUC->setCheckMode(false);
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, SearchPharmacyGroupRequested )
{
    METHODNAME_DEF( TenderMaintainUC, SearchPharmacyGroupRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;
    
    try
    {
        tender::useCase::IPickerUCPtr pharmacyGroupPickerUC( getUseCaseGetter()->getPharmacyGroupPickerUC() );
        getTenderMaintainVC()->matchToPharmacyGroupSearch( pharmacyGroupPickerUC->getSearchYIterator() );
        pharmacyGroupPickerUC->setSearchCriteria( pharmacyGroupPickerUC->getSearchYIterator() );
        pharmacyGroupPickerUC->run();
        
        if( !pharmacyGroupPickerUC->getPicked().isNull() )
        {
            getTenderMaintainVC()->matchFromPharmacyGroup( pharmacyGroupPickerUC->getPicked() );
            handleCheckCustomerRequested(rSource, yitFromEvSrc);
        }
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }
    
    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckPharmacyGroupRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CheckPharmacyGroupRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;
    
    try
    {
        tender::useCase::IPickerUCPtr pharmacyGroupPickerUC( getUseCaseGetter()->getPharmacyGroupPickerUC() );
        getTenderMaintainVC()->matchToPharmacyGroupSearch( pharmacyGroupPickerUC->getSearchYIterator() );
        pharmacyGroupPickerUC->setCheckMode(true);
        pharmacyGroupPickerUC->run();
        
        basar::db::aspect::AccessorPropertyTable_YIterator yitpharmacyGroup = pharmacyGroupPickerUC->getPicked();
        if( false == yitpharmacyGroup.isNull() && false == yitpharmacyGroup.isEnd() )
        {
            getTenderMaintainVC()->matchFromPharmacyGroup( yitpharmacyGroup );
        }
        else
        {
            getTenderMaintainVC()->resetPharmacyGroup();
            pharmacyGroupPickerUC->setCheckMode(false);
            return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
        }
        
        pharmacyGroupPickerUC->setCheckMode(false);
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }
    
    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, SearchPartnerRequested )
{
    METHODNAME_DEF( TenderMaintainUC, SearchPartnerRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        tender::useCase::IPickerUCPtr partnerPickerUC( getUseCaseGetter()->getPartnerPickerUC() );
        getTenderMaintainVC()->matchToPartnerSearch( partnerPickerUC->getSearchYIterator() );
        partnerPickerUC->setSearchCriteria( partnerPickerUC->getSearchYIterator() );
        partnerPickerUC->run();

        if( !partnerPickerUC->getPicked().isNull() )
        {
            getTenderMaintainVC()->matchFromPartner( partnerPickerUC->getPicked() );
        }
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckPartnerRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CheckPartnerRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        tender::useCase::IPickerUCPtr partnerPickerUC( getUseCaseGetter()->getPartnerPickerUC() );
        getTenderMaintainVC()->matchToPartnerSearch( partnerPickerUC->getSearchYIterator() );

        partnerPickerUC->setCheckMode(true);
        partnerPickerUC->run();
        basar::db::aspect::AccessorPropertyTable_YIterator yitPartner = partnerPickerUC->getPicked();

        if (false == yitPartner.isNull() && false == yitPartner.isEnd())
        {
            getTenderMaintainVC()->matchFromPartner( yitPartner );
        }
        else
        {
            partnerPickerUC->setCheckMode(false);
            return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
        }

        partnerPickerUC->setCheckMode(false);
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, SearchPharmacyChainRequested )
{
    METHODNAME_DEF( TenderMaintainUC, SearchPharmacyChainRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        tender::useCase::IPickerUCPtr pharmacyChainPickerUC( getUseCaseGetter()->getPharmacyChainPickerUC() );
        getTenderMaintainVC()->matchToPharmacyChainSearch( pharmacyChainPickerUC->getSearchYIterator() );
        pharmacyChainPickerUC->setSearchCriteria( pharmacyChainPickerUC->getSearchYIterator() );
        pharmacyChainPickerUC->run();

        if( !pharmacyChainPickerUC->getPicked().isNull() )
        {
            getTenderMaintainVC()->matchFromPharmacyChain( pharmacyChainPickerUC->getPicked() );
        }
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckPharmacyChainRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CheckPharmacyChainRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        tender::useCase::IPickerUCPtr pharmacyChainPickerUC( getUseCaseGetter()->getPharmacyChainPickerUC() );
        getTenderMaintainVC()->matchToPharmacyChainSearch( pharmacyChainPickerUC->getSearchYIterator() );

        pharmacyChainPickerUC->setCheckMode(true);
        pharmacyChainPickerUC->run();
        basar::db::aspect::AccessorPropertyTable_YIterator yitPharmacyChain = pharmacyChainPickerUC->getPicked();

        if( pharmacyChainPickerUC->isPharmacyChainValid() )
        {
            if( pharmacyChainPickerUC->isCustomerGroupIDValid() )
            {
                getTenderMaintainVC()->matchFromPharmacyChain( yitPharmacyChain );
            }
            else
            {
                getTenderMaintainVC()->displayCustomerGroupIDNotValid();
                return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
            }
        }
        else
        {
            getTenderMaintainVC()->displayPharmacyChainNotValid();
            return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
        }

        pharmacyChainPickerUC->setCheckMode(false);
        return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckCustomerGroupIDRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CheckCustomerGroupIDRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        tender::useCase::IPickerUCPtr pharmacyChainPickerUC( getUseCaseGetter()->getPharmacyChainPickerUC() );
        getTenderMaintainVC()->matchToPharmacyChainSearch( pharmacyChainPickerUC->getSearchYIterator() );

        pharmacyChainPickerUC->setCheckMode(true);
        pharmacyChainPickerUC->run();
        basar::db::aspect::AccessorPropertyTable_YIterator yitPharmacyChain = pharmacyChainPickerUC->getPicked();

        if (false == yitPharmacyChain.isNull() && false == yitPharmacyChain.isEnd())
        {
            getTenderMaintainVC()->matchFromPharmacyChain( yitPharmacyChain );
        }
        else
        {
            pharmacyChainPickerUC->setCheckMode(false);
            return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
        }

        pharmacyChainPickerUC->setCheckMode(false);
    }
    catch( const basar::Exception & e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CreateCSCOrdersRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CreateCSCOrdersRequested )
    BLOG_TRACE_METHOD( getLogger(), fun );
    rSource;

    try
    {
        getTenderMaintainVC()->matchToTender( m_Tender );

        libutil::useCase::IRunnablePtr orderMaintainUC = getUseCaseGetter()->getCSCOrderMaintainUC( m_Tender, libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr() );
        orderMaintainUC->run();

        basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_Tender->getHeader().cloneToUnboundedPropertyRow();
        m_Tender->findByKey( yitSearch );
        getTenderMaintainVC()->matchFromTender( m_Tender );
    }
    catch( const basar::Exception& e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnStruct();
}

SYSTEM_EVENT_HANDLER_DEFINITION(TenderMaintainUC, AddDeliveryRequested)
{
    METHODNAME_DEF(TenderMaintainUC, AddDeliveryRequested)
    BLOG_TRACE_METHOD(getLogger(), fun);
    rSource;

    try
    { 
        //Make sure that a customer is selected!
        if(!getTenderMaintainVC()->isCustomerSelected())
        {
            getTenderMaintainVC()->displayCustomerNotSelected();
            return basar::appl::EventReturnStruct();
        }

        //Make sure that the customer list is saved before adding a new delivery! 
        if(!m_Tender->getCustomerList()->get().isNull() && !m_Tender->getCustomerList()->get().isEmpty())
        {
            if(hasCustomerListChanged(m_Tender->getCustomerList()->get()))
            {
                getTenderMaintainVC()->displayCustomerListNotSaved();
                return basar::appl::EventReturnStruct();
            }
        }

        basar::db::aspect::AccessorPropertyTable_YIterator yitScheduledDelivery;
        libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery = m_Schedule->getScheduledDeliveryCollection()->getScheduledDelivery(yitScheduledDelivery);

        getTenderMaintainVC()->matchToTender(m_Tender);

        libutil::useCase::IRunnablePtr scheduledDeliveryMaintainUC = 
            getUseCaseGetter()->getScheduledDeliveryMaintainUC(m_Tender, scheduledDelivery, m_Schedule->getScheduledDeliveryCollection(), needsCustomerSelection());

        scheduledDeliveryMaintainUC->run();
        
        m_Schedule->getScheduledDeliveryCollection()->findByPattern(m_Tender->getHeader());
        getTenderMaintainVC()->matchFromScheduledDeliveryCollection(m_Schedule->getScheduledDeliveryCollection());

        getTenderMaintainVC()->selectDelivery(scheduledDelivery->getHeader()->get());
    }
    catch(const basar::Exception& e)
    {
        BLOG_ERROR(m_Logger, e.what().c_str());
        throw;
    }

    return basar::appl::EventReturnStruct();
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, ModifyDeliveryRequested )
{
    METHODNAME_DEF( TenderMaintainUC, ModifyDeliveryRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        basar::db::aspect::AccessorPropertyTable_YIterator yitScheduledDelivery = getTenderMaintainVC()->getCurrentScheduledDelivery();
        libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery = m_Schedule->getScheduledDeliveryCollection()->getScheduledDelivery( yitScheduledDelivery );
        getTenderMaintainVC()->matchToTender( m_Tender );

        if (scheduledDelivery->isPurchaseOrderProposalExisting())
        {
            getTenderMaintainVC()->displayPurchaseOrderProposalExisting();
        }

        libutil::useCase::IRunnablePtr scheduledDeliveryMaintainUC = 
            getUseCaseGetter()->getScheduledDeliveryMaintainUC( m_Tender, scheduledDelivery, m_Schedule->getScheduledDeliveryCollection(), needsCustomerSelection() );

        scheduledDeliveryMaintainUC->run();

        m_Schedule->getScheduledDeliveryCollection()->findByPattern( m_Tender->getHeader() );
        getTenderMaintainVC()->matchFromScheduledDeliveryCollection( m_Schedule->getScheduledDeliveryCollection() );

		getTenderMaintainVC()->selectDelivery(scheduledDelivery->getHeader()->get());
    }
    catch( const basar::Exception& e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnStruct();
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, DeleteDeliveryRequested )
{
    METHODNAME_DEF(TenderMaintainUC, DeleteDeliveryRequested)
    BLOG_TRACE_METHOD(m_Logger, fun);
    rSource;

    try
    {
        //TODO: move to ScheduledDeliveryCollectionDM
        libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery = 
			m_Schedule->getScheduledDeliveryCollection()->getScheduledDelivery( getTenderMaintainVC()->getCurrentScheduledDelivery() );
        
        basar::db::aspect::AccessorPropertyTable_YIterator scheduledDeliveryHeader = scheduledDelivery->getHeader()->get();
        scheduledDeliveryHeader.setPropertyState( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID, basar::SS_UPDATE );

        basar::db::aspect::AccessorPropertyTable_YIterator position = scheduledDelivery->getPositionCollection()->get().begin();

        for( ; !position.isEnd(); ++position )
        {
            scheduledDelivery->getPositionCollection()->get().markForDelete( position );
        }

        deletePurchaseOrderProposals(scheduledDelivery);
        scheduledDelivery->save();

        m_Schedule->getScheduledDeliveryCollection()->findByPattern( m_Tender->getHeader() );
        getTenderMaintainVC()->matchFromScheduledDeliveryCollection( m_Schedule->getScheduledDeliveryCollection() );
    }
    catch(const basar::Exception& e)
    {
        BLOG_ERROR(m_Logger, e.what().c_str());
        throw;
    }

    return basar::appl::EventReturnStruct();
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, DeliverySelectionChangeRequested )
{
    METHODNAME_DEF( TenderMaintainUC, DeliverySelectionChangeRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery = 
            m_Schedule->getScheduledDeliveryCollection()->getScheduledDelivery( getTenderMaintainVC()->getCurrentScheduledDelivery() );

        getTenderMaintainVC()->matchFromScheduledDeliveryPositionCollection( scheduledDelivery->getPositionCollection()->get() );
    }
    catch( const basar::Exception& e )
    {
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::EventReturnStruct();
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, ScheduleRequested )
{
    METHODNAME_DEF( TenderMaintainUC, ScheduleRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    if(m_CloneDeliveries)
    {
        getTenderMaintainVC()->matchFromScheduledDeliveryCollection(m_Schedule->getScheduledDeliveryCollection());
        return basar::appl::EventReturnStruct();
    }

    if( NULL != m_Tender.get() && NULL != m_Schedule.get() )
    {
        m_Schedule->getScheduledDeliveryCollection()->findByPattern( m_Tender->getHeader() );
        getTenderMaintainVC()->matchFromScheduledDeliveryCollection( m_Schedule->getScheduledDeliveryCollection() );
    }

    return basar::appl::EventReturnStruct();
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, ChangeStateToClosedRequested )
{
    METHODNAME_DEF( TenderMaintainUC, ChangeStateToClosedRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    getTenderMaintainVC()->disableAllExceptState( true );

    basar::db::aspect::AccessorPropertyTable_YIterator deliveryYit = m_Schedule->getScheduledDeliveryCollection()->get().begin();

    for( ; !deliveryYit.isEnd(); ++deliveryYit )
    {
        if( deliveryYit.getDate( properties::DELIVERY_DATE ).getDate() > basar::Date::getCurrent().getDate() )
        {
            getTenderMaintainVC()->displayOpenDeliveriesWarning();
            return basar::appl::EventReturnStruct();
        }
    }

    return basar::appl::EventReturnStruct();
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, ChangeStateFromClosedRequested )
{
    METHODNAME_DEF( TenderMaintainUC, ChangeStateFromClosedRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    getTenderMaintainVC()->disableAllExceptState( false );
    getTenderMaintainVC()->tblDeliverySelectionChanged();

    return basar::appl::EventReturnStruct();
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckIfPositionsWereModifiedRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CheckIfPositionsWereModifiedRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    if( !m_Tender->getHeader().isContainedAndSet( properties::TENDER_NO ) )
    {
        return basar::appl::HANDLER_NOT_FOUND;
    }

    basar::db::aspect::AccessorPropertyTable_YIterator position = m_Tender->getPositionCollection()->get().begin();

    for( ; !position.isEnd(); ++position )
    {
        if( basar::db::aspect::PropertyState::PropertyState( basar::SS_INSERT ) == position.getState( properties::TENDER_NO )	       ||
            basar::db::aspect::PropertyState::PropertyState( basar::SS_UPDATE ) == position.getState( properties::ARTICLE_CODE )       ||
            basar::db::aspect::PropertyState::PropertyState( basar::SS_UPDATE ) == position.getState( properties::CONTRACT_QTY )       ||
            basar::db::aspect::PropertyState::PropertyState( basar::SS_UPDATE ) == position.getState( properties::CONTRACT_PRICE )     ||
            basar::db::aspect::PropertyState::PropertyState( basar::SS_UPDATE ) == position.getState( properties::ADD_DISCOUNT_PCT )   ||
            basar::db::aspect::PropertyState::PropertyState( basar::SS_UPDATE ) == position.getState( properties::DISCOUNT_CALC_FROM ) ||
            basar::db::aspect::PropertyState::PropertyState( basar::SS_UPDATE ) == position.getState( properties::DISCOUNT_APPLY_TO ) )
        {
            return basar::appl::HANDLER_ERROR;
        }
    }

    return basar::appl::HANDLER_OK;
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CloneDeliveryRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CloneDeliveryRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr deliveryToBeCloned =
        m_Schedule->getScheduledDeliveryCollection()->getScheduledDelivery( getTenderMaintainVC()->getCurrentScheduledDelivery() );

    deliveryToBeCloned->getHeader()->get().setPropertyState( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID, basar::SS_UNSET );
    deliveryToBeCloned->getHeader()->get().setPropertyState( properties::DELIVERY_DATE,					   basar::SS_UNSET );
    deliveryToBeCloned->getHeader()->get().setPropertyState( properties::CSC_ORDER_NO,					   basar::SS_UNSET );
    deliveryToBeCloned->getHeader()->get().setPropertyState( properties::CSC_ORDER_DATE,				   basar::SS_UNSET );
    deliveryToBeCloned->getHeader()->get().setPropertyState( properties::LAST_CHANGED_DATETIME,			   basar::SS_UNSET );
    deliveryToBeCloned->getHeader()->get().setPropertyState( properties::CREATION_DATETIME,				   basar::SS_UNSET );

    deliveryToBeCloned->getHeader()->get().setPropertyState( properties::TENDER_ID,	basar::SS_INSERT );
    

    basar::db::aspect::AccessorPropertyTable_YIterator article =  deliveryToBeCloned->getPositionCollection()->get().begin();

    for( ; !article.isEnd(); ++article )
    {
        article.setPropertyState( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID, basar::SS_UNSET );
        article.setPropertyState( properties::QUANTITY_DELIVERED, basar::SS_UNSET );
        article.setPropertyState( properties::LAST_CHANGED_DATETIME, basar::SS_UNSET );
        article.setPropertyState( properties::CREATION_DATETIME, basar::SS_UNSET );
        article.setPropertyState( properties::ORDER_PROPOSAL_NO, basar::SS_UNSET );

        article.setPropertyState( properties::ARTICLE_CODE, basar::SS_INSERT );
        article.setPropertyState( properties::QUANTITY, basar::SS_INSERT );
    }

    if( deliveryToBeCloned->isPurchaseOrderProposalExisting())
    {
        getTenderMaintainVC()->displayPurchaseOrderProposalExisting();
    }

    libutil::useCase::IRunnablePtr scheduledDeliveryMaintainUC = 
        getUseCaseGetter()->getScheduledDeliveryMaintainUC( m_Tender, deliveryToBeCloned, m_Schedule->getScheduledDeliveryCollection(), needsCustomerSelection() );
    
    scheduledDeliveryMaintainUC->run();

    m_Schedule->getScheduledDeliveryCollection()->findByPattern( m_Tender->getHeader() );
    getTenderMaintainVC()->matchFromScheduledDeliveryCollection( m_Schedule->getScheduledDeliveryCollection() );

    getTenderMaintainVC()->selectDelivery( deliveryToBeCloned->getHeader()->get() );

    return basar::appl::HANDLER_OK;
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckIfTenderWasModifiedRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CheckIfTenderWasModifiedRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    getTenderMaintainVC()->matchToTender( m_Tender );

    if( m_Tender->getHeader().arePropertiesSetAndAllInState( properties::TENDER_NO, basar::SS_INSERT ) )
    {
        return basar::appl::HANDLER_OK;
    }

    if( m_Tender->getHeader().isContainedAndSet( properties::TENDER_NO ) &&
       (m_Tender->getHeader().getState().contains( basar::SS_INSERT )    ||
        m_Tender->getHeader().getState().contains( basar::SS_UPDATE )    ||
        m_Tender->getHeader().getState().contains( basar::SS_DELETE ))   )
    {
        return basar::appl::HANDLER_ERROR;
    }

    basar::db::aspect::AccessorPropertyTable_YIterator position = m_Tender->getPositionCollection()->get().begin();

    for( ; !position.isEnd(); ++position )
    {
        if( position.isContainedAndSet( properties::TENDER_NO ) &&
           (position.getState().contains( basar::SS_INSERT )    ||
            position.getState().contains( basar::SS_UPDATE )    ||
            position.getState().contains( basar::SS_DELETE ))   )
        {
            return basar::appl::HANDLER_ERROR;
        }
    }
    
    return basar::appl::HANDLER_OK;
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckIfContractPeriodIsValid )
{
    METHODNAME_DEF( TenderMaintainUC, CheckIfContractPeriodIsValid )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
	{
		if( !m_Tender->getHeader().isContainedAndSet( properties::TENDER_NO ) )
		{
			return basar::appl::HANDLER_OK;
		}

        m_Schedule->getScheduledDeliveryCollection()->findByPattern( m_Tender->getHeader() );

        basar::db::aspect::AccessorPropertyTableRef deliveries = m_Schedule->getScheduledDeliveryCollection()->get();
        basar::db::aspect::AccessorPropertyTable_YIterator delivery = deliveries.begin();
        
        basar::Int32 dateto = m_Tender->getHeader().getDate( properties::DATE_TO ).getDate();
        basar::Int32 datefrom = m_Tender->getHeader().getDate( properties::DATE_FROM ).getDate();
        
        while( !delivery.isEnd() )
        {
            if( dateto < delivery.getDate( properties::DELIVERY_DATE ).getDate() ||
                datefrom > delivery.getDate( properties::DELIVERY_DATE).getDate() )
            {
                getTenderMaintainVC()->displayDeliveryOutOfContractPeriod();
                return basar::appl::EventReturnType( basar::appl::HANDLER_INFO );
            }

            ++delivery;
        }
    }
    catch( const basar::Exception & e )
    {	
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::HANDLER_OK;
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, ContractInfoChanged )
{
    METHODNAME_DEF( TenderMaintainUC, ContractInfoChanged )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        getTenderMaintainVC()->checkContractInfo();
    }
    catch( const basar::Exception & e )
    {	
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::HANDLER_OK;
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CustomerListRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CustomerListRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        if( m_Tender->getCustomerList()->get().isEmpty() || m_Tender->getCustomerList()->get().isNull() )
        {
            m_Tender->getCustomerList()->findByTenderNo( m_Tender->getHeader() );
        }

        libutil::useCase::IRunnablePtr customerListMaintainUC = getUseCaseGetter()->getCustomerListMaintainUC( m_Tender->getCustomerList() );
        customerListMaintainUC->run();
    }
    catch( const basar::Exception & e )
    {	
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::HANDLER_OK;
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, CheckCustomerListRequested )
{
    METHODNAME_DEF( TenderMaintainUC, CustomerListRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        basar::db::aspect::AccessorPropertyTableRef customerList = m_Tender->getCustomerList()->get();
        basar::db::aspect::AccessorPropertyTable_YIterator customer = customerList.begin();

        while( !customer.isEnd() )
        {
            if( customer.getState() == basar::SS_INSERT ||
                customer.getState() == basar::SS_UPDATE ||
                customer.getState() == basar::SS_CLEAN )
            {
                return basar::appl::HANDLER_OK;
            }

            ++customer;
        }

        return basar::appl::HANDLER_ERROR;
    }
    catch( const basar::Exception & e )
    {	
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderMaintainUC, ProtocolsRequested )
{
    METHODNAME_DEF( TenderMaintainUC, ProtocolsRequested )
    BLOG_TRACE_METHOD( m_Logger, fun );
    rSource;

    try
    {
        if( !m_Tender->getHeader().isContainedAndSet( properties::TENDER_NO ) )
        {
            getTenderMaintainVC()->displayTenderMustExist();
            return basar::appl::HANDLER_OK;
        }

        libutil::useCase::IRunnablePtr tenderProtocolUC = getUseCaseGetter()->getTenderProtocolUC( m_Tender, m_Area );
        tenderProtocolUC->run();
    }
    catch( const basar::Exception & e )
    {	
        BLOG_ERROR( m_Logger, e.what().c_str() );
        throw;
    }

    return basar::appl::HANDLER_OK;
}

} // namespace usecase
} // namespace tender
