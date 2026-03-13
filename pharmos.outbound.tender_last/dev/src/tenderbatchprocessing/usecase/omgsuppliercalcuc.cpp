//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "omgsuppliercalcuc.h"
#include <loggerpool/loggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>
#include <libbasardbaspect_exceptions.h>

#include "domainmodule/omgsuppliercalc/omgsuppliercalcdm.h"
#include "domainmodule/omgsuppliercalc/iomgsuppliercalc.h"

#include <domainmodule/tender/itenderprogress.h>
#include <domainmodule/tender/tenderprogressdm.h>

#include <domainmodule/tender/itendercollectiondm.h>
#include <domainmodule/tender/tendercollectiondm.h>

#include <domainmodule/tender/itender.h>
#include <domainmodule/tender/tenderdm.h>
#include <domainmodule/libtender_properties_definitions.h>

#include <domainmodule/tender/itenderpositioncollectionptr.h>
#include <domainmodule/tender/tenderpositioncollection.h>

//-------------------------------------------------------------------------------------------------//
// namespace sections
//-------------------------------------------------------------------------------------------------//
namespace tenderbatchprocessing
{
namespace useCase
{

    OMGSupplierCalcUC::OMGSupplierCalcUC()
    : m_Logger( tenderbatchprocessing::LoggerPool::getLoggerUseCases() )
    {
        METHODNAME_DEF( OMGSupplierCalcUC, OMGSupplierCalcUC )
        BLOG_TRACE_METHOD( m_Logger, fun );

        //registerEventHandlers( true );
    }

    OMGSupplierCalcUC::~OMGSupplierCalcUC()
    {
        METHODNAME_DEF( OMGSupplierCalcUC, ~OMGSupplierCalcUC )
        BLOG_TRACE_METHOD( m_Logger, fun );

        //registerEventHandlers( false );
    }

    void OMGSupplierCalcUC::injectOMGSupplierCalcDM(tenderbatchprocessing::domMod::omgsuppliercalc::IOMGSupplierCalcPtr omgSupplierCalcDM)
    {
        METHODNAME_DEF( OMGSupplierCalcUC, injectOMGSupplierCalcDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_OMGSupplierCalcDM = omgSupplierCalcDM;
    }

    void OMGSupplierCalcUC::injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgressDM )
    {
        METHODNAME_DEF( OMGSupplierCalcUC, injectTenderProgressDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_TenderProgressDM = tenderProgressDM;
    }

    void OMGSupplierCalcUC::injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM )
    {
        METHODNAME_DEF( OMGSupplierCalcUC, injectTenderCollectionDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_TenderCollectionDM = tenderCollectionDM;
    }

    void OMGSupplierCalcUC::setTender( libtender::domMod::tender::ITenderPtr tender )
    {
        METHODNAME_DEF( OMGSupplierCalcUC, setTender )
        BLOG_TRACE_METHOD( m_Logger, fun );
        m_Tender = tender;
    }

    void OMGSupplierCalcUC::run()
    {
        METHODNAME_DEF( OMGSupplierCalcUC, run )
        BLOG_TRACE_METHOD( getLogger(), fun );
        try
        {
            basar::db::aspect::AccessorPropertyTable_YIterator searchYit = m_TenderProgressDM->getSearchYit();
            searchYit.setInt32( properties::TENDER_NO, m_Tender->getHeader().getInt32(properties::TENDER_NO ) );
            m_TenderProgressDM->findTenderProgressForOMGCalcultion( searchYit );

            basar::db::aspect::AccessorPropertyTable_YIterator yitEnd = m_TenderProgressDM->get().begin();
            basar::db::aspect::AccessorPropertyTable_YIterator yitStart;
            while ( !yitEnd.isEnd() )
            {
                yitStart = yitEnd;
                yitEnd = getEndOfArticleGroup( yitStart );
                processArticle( yitStart, yitEnd ); // pass the start of the artice and End pos. of the same article
            } //again loop to get next article
        }
        catch( const basar::Exception & e )
        {
            BLOG_ERROR( m_Logger, e.what().c_str() );
        }
    }

    basar::db::aspect::AccessorPropertyTable_YIterator OMGSupplierCalcUC::getEndOfArticleGroup( basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgStart )
    {
        METHODNAME_DEF( OMGSupplierCalcUC, getEndOfArticleGroup )
        BLOG_TRACE_METHOD( getLogger(), fun );

        // returns the yit containing the next article number
        basar::Int32 tenderProgArtNo = yitTenderProgStart.getInt32( properties::ARTICLE_NO );
        basar::db::aspect::AccessorPropertyTable_YIterator yitEnd = yitTenderProgStart;
        while ( !yitEnd.isEnd() && ( yitEnd.getInt32( properties::ARTICLE_NO ) == tenderProgArtNo ) )
        {
            ++yitEnd;
        }
        return yitEnd;
    }

// pass the start of the artice and End pos. of the same article
    void OMGSupplierCalcUC::processArticle( basar::db::aspect::AccessorPropertyTable_YIterator yitArticleStartPos, basar::db::aspect::AccessorPropertyTable_YIterator yitArticleEndPos )
    {
        METHODNAME_DEF( OMGSupplierCalcUC, processArticle )
        BLOG_TRACE_METHOD( getLogger(), fun );

        checkDeliveredQuantity( yitArticleStartPos, yitArticleEndPos );
       // validateTenderDiscount( yitArticleStartPos, yitArticleEndPos );
       // saveCalculation();
    }

    void OMGSupplierCalcUC::validateTenderDiscount( basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleStart, basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleEnd )
    {
         METHODNAME_DEF( OMGSupplierCalcUC, validateTenderDiscount )
        BLOG_TRACE_METHOD( getLogger(), fun );

        basar::db::aspect::AccessorPropertyTable_YIterator yitCurrent;  
        for( yitCurrent = yitTenderProgArticleStart; yitCurrent != yitTenderProgArticleEnd; ++yitCurrent )
        {
            basar::Decimal tenderProgDiscPct = yitCurrent.getDecimal( properties::DISCOUNT_PCT );
            if(  tenderProgDiscPct == m_TenderDiscPct)
            {
                continue;
            }

            basar::Decimal basePrice = yitCurrent.getDecimal( properties::GEP );
           // basar::Int32 deliveredQty = yitCurrent.getInt32( properties::DELIVERY_QTY ); 
            basar::Decimal percentageDiff = basar::Decimal(0.0);
            //basar::Decimal priceDiff = basar::Decimal(0.0);
            if( m_TenderDiscPct > tenderProgDiscPct )
            {

                percentageDiff = m_TenderDiscPct -tenderProgDiscPct;
                //priceDiff = basePrice * (percentageDiff/100) * deliveredQty;
            }
            else
            {
                percentageDiff = tenderProgDiscPct - m_TenderDiscPct;
                //priceDiff = basePrice * (percentageDiff/100) * deliveredQty;
            }
           
            createSupplierCalcPosition( yitCurrent, percentageDiff );
        }
    }

    void OMGSupplierCalcUC::createSupplierCalcPosition(basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProg, basar::Decimal discountDiff, bool transFlag )
    {
         METHODNAME_DEF( OMGSupplierCalcUC, createSupplierCalcPosition )
         BLOG_TRACE_METHOD( getLogger(), fun );

        basar::db::aspect::AccessorPropertyTable_YIterator yitOMGCal = m_OMGSupplierCalcDM->get().insert(basar::FOR_INSERT);
        yitOMGCal.setInt32(properties::TENDER_NO, yitTenderProg.getInt32( properties::TENDER_NO ) );
        yitOMGCal.setInt16(properties::BRANCH_NO, yitTenderProg.getInt16( properties::BRANCH_NO ) );
        yitOMGCal.setInt32(properties::ORDER_NO, yitTenderProg.getInt32( properties::ORDER_NO ) );
        yitOMGCal.setDate(properties::ORDER_DATE, yitTenderProg.getDate( properties::ORDER_DATE ) );
        yitOMGCal.setInt32(properties::ARTICLE_NO, yitTenderProg.getInt32( properties::ARTICLE_NO ) );
        yitOMGCal.setInt32(properties::CUSTOMER_NO, yitTenderProg.getInt32( properties::CUSTOMER_NO ) );

        yitOMGCal.setInt32(properties::INVOICE_NO, yitTenderProg.getInt32( properties::INVOICE_NO ) );
        //yitOMGCal.setDate(properties::INVOICE_DATE, yitTenderProg.getDate( properties::INVOICE_DATE ) );
        //yitOMGCal.setInt32(properties::INVOICE_TIME, yitTenderProg.getInt32( properties::INVOICE_TIME ) );

        yitOMGCal.setInt32(properties::DELIVERY_QTY, yitTenderProg.getInt32( properties::DELIVERY_QTY ) );
        yitOMGCal.setDecimal( properties::BASE_PRICE, yitTenderProg.getDecimal( properties::AEP ) );
        yitOMGCal.setDecimal( properties::DISCOUNT_PCT, yitTenderProg.getDecimal( properties::DISCOUNT_PCT ) );
        yitOMGCal.setInt16(properties::TRANSFERED_FLAG, transFlag );
        m_OMGSupplierCalcDM->save( yitOMGCal );
    }

    void OMGSupplierCalcUC::checkDeliveredQuantity( basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleStart, basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleEnd )
    {
        METHODNAME_DEF( OMGSupplierCalcUC, checkDeliveredQuantity )
        BLOG_TRACE_METHOD( getLogger(), fun );
        try
        {
            basar::Int32 contractQty = getContractQuantity( yitTenderProgArticleStart.getInt32( properties::ARTICLE_NO ) );
            basar::Int32 sumDeliveredQty = 0;
            basar::db::aspect::AccessorPropertyTable_YIterator yitCurrent;
            for( yitCurrent = yitTenderProgArticleStart; yitCurrent != yitTenderProgArticleEnd; ++yitCurrent )
            {
                sumDeliveredQty += yitCurrent.getInt32( properties::QUANTITY );
            }

            if( ( sumDeliveredQty > contractQty ) )
            {
                basar::Int32 qtyExceeded = sumDeliveredQty - contractQty;
                std::stringstream ss;
                ss<< "Delivered quantity exceeds the contract quantity. Total exceeding quantity = " << qtyExceeded;
                BLOG_ERROR( m_Logger, ss.str() );
            }
        }
        catch( const basar::Exception & e )
        {
            BLOG_ERROR( m_Logger, e.what().c_str() );
        }
    }

    basar::Int32 OMGSupplierCalcUC::getContractQuantity( basar::Int32 articleNo)
    {
        METHODNAME_DEF( OMGSupplierCalcUC, getContractQuantity )
        BLOG_TRACE_METHOD( getLogger(), fun );

        basar::Int32 contractQty = 0;
        try
        {
            libtender::domMod::tender::ITenderPositionCollectionPtr tenderPos = m_Tender->getPositionCollection();
            basar::db::aspect::AccessorPropertyTable_YIterator yitTenPos = tenderPos->get().begin();
            for( ; !yitTenPos.isEnd(); ++yitTenPos )
            {
                if ( yitTenPos.getInt32( properties::ARTICLE_NO ) == articleNo )
                {
                    contractQty = yitTenPos.getInt32( properties::CONTRACT_QTY );
                   // m_TenderDiscPct = yitTenPos.getDecimal( properties::ADD_DISCOUNT_PCT );
                    break;
                }
            }
        }
        catch( const basar::Exception & e )
        {
            BLOG_ERROR( m_Logger, e.what().c_str() );
        }

        return contractQty;
    }

    void OMGSupplierCalcUC::saveCalculation()
    {
        METHODNAME_DEF( OMGSupplierCalcUC, saveCalculation )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }


    const log4cplus::Logger OMGSupplierCalcUC::getLogger() const
    {
        METHODNAME_DEF( OMGSupplierCalcUC, getLogger )
        BLOG_TRACE_METHOD( m_Logger, fun );

        return m_Logger;
    }

 } // namespace usecase
} // namespace tender