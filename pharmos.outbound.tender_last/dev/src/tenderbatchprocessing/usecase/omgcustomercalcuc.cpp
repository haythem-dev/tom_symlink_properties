//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "omgcustomercalcuc.h"
#include <loggerpool/loggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>
#include <libbasardbaspect_exceptions.h>

#include "domainmodule/omgcustomercalc/omgcustomercalcdm.h"
#include "domainmodule/omgcustomercalc/iomgcustomercalc.h"

#include <domainmodule/tender/itenderprogress.h>
#include <domainmodule/tender/tenderprogressdm.h>

#include <domainmodule/tender/itendercollectiondm.h>
#include <domainmodule/tender/tendercollectiondm.h>

#include <domainmodule/tender/itender.h>
#include <domainmodule/tender/tenderdm.h>
#include <domainmodule/libtender_properties_definitions.h>

#include <domainmodule/tender/itenderpositioncollectionptr.h>
#include <domainmodule/tender/tenderpositioncollection.h>
#include <domainmodule/tender/tenderstateenum.h>
//-------------------------------------------------------------------------------------------------//
// namespace sections
//-------------------------------------------------------------------------------------------------//
namespace tenderbatchprocessing
{
namespace useCase
{

    OMGCustomerCalcUC::OMGCustomerCalcUC()
    : m_Logger( tenderbatchprocessing::LoggerPool::getLoggerUseCases() )
    {
        METHODNAME_DEF( OMGCustomerCalcUC, OMGCustomerCalcUC )
        BLOG_TRACE_METHOD( m_Logger, fun );


    }

    OMGCustomerCalcUC::~OMGCustomerCalcUC()
    {
        METHODNAME_DEF( OMGCustomerCalcUC, ~OMGCustomerCalcUC )
        BLOG_TRACE_METHOD( m_Logger, fun );
    }

    void OMGCustomerCalcUC::injectOMGCustomerCalcDM(tenderbatchprocessing::domMod::omgcustomercalc::IOMGCustomerCalcPtr omgCustomerCalcDM)
    {
        METHODNAME_DEF( OMGCustomerCalcUC, injectOMGCustomerCalcDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_OMGCustomerCalcDM = omgCustomerCalcDM;
    }

    void OMGCustomerCalcUC::injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgressDM )
    {
        METHODNAME_DEF( OMGCustomerCalcUC, injectTenderProgressDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_TenderProgressDM = tenderProgressDM;
    }

    void OMGCustomerCalcUC::injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM )
    {
        METHODNAME_DEF( OMGCustomerCalcUC, injectTenderCollectionDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_TenderCollectionDM = tenderCollectionDM;
    }

    void OMGCustomerCalcUC::setTender( libtender::domMod::tender::ITenderPtr tender )
    {
        METHODNAME_DEF( OMGCustomerCalcUC, setTender )
        BLOG_TRACE_METHOD( m_Logger, fun );
        m_Tender = tender;
    }

    void OMGCustomerCalcUC::run()
    {
        METHODNAME_DEF( OMGCustomerCalcUC, run )
        BLOG_TRACE_METHOD( getLogger(), fun );
        try
        {
            m_TenderEndDate = m_Tender->getHeader().getDate( properties::DATE_TO ) ;

            basar::db::aspect::AccessorPropertyTable_YIterator searchYit = m_TenderProgressDM->getSearchYit();
            searchYit.setInt32( properties::TENDER_NO, m_Tender->getHeader().getInt32(properties::TENDER_NO ) );
            m_TenderProgressDM->findTenderProgressForOMGCalcultion( searchYit );

            basar::db::aspect::AccessorPropertyTable_YIterator yitEnd = m_TenderProgressDM->get().begin();
            basar::db::aspect::AccessorPropertyTable_YIterator yitStart;
            while ( !yitEnd.isEnd() )
            {
                yitStart = yitEnd;
                yitEnd = getEndOfArticleGroup( yitStart );
                processArticle( yitStart, yitEnd );
            }
        }
        catch( const basar::Exception & e )
        {
            BLOG_ERROR( m_Logger, e.what().c_str() );
        }
    }

    basar::db::aspect::AccessorPropertyTable_YIterator OMGCustomerCalcUC::getEndOfArticleGroup( basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgStart )
    {
        METHODNAME_DEF( OMGCustomerCalcUC, getEndOfArticleGroup )
        BLOG_TRACE_METHOD( getLogger(), fun );

        basar::Int32 tenderProgArtNo = yitTenderProgStart.getInt32( properties::ARTICLE_NO );
        basar::db::aspect::AccessorPropertyTable_YIterator yitEnd = yitTenderProgStart;
        while ( !yitEnd.isEnd() && ( yitEnd.getInt32( properties::ARTICLE_NO ) == tenderProgArtNo ) )
        {
            ++yitEnd;
        }
        return yitEnd;
    }

    void OMGCustomerCalcUC::processArticle( basar::db::aspect::AccessorPropertyTable_YIterator yitArticleStartPos, basar::db::aspect::AccessorPropertyTable_YIterator yitArticleEndPos )
    {
        METHODNAME_DEF( OMGCustomerCalcUC, processArticle )
        BLOG_TRACE_METHOD( getLogger(), fun );

        checkDeliveredQuantity( yitArticleStartPos, yitArticleEndPos );
        //validateTenderDiscount( yitArticleStartPos, yitArticleEndPos );
        //saveCalculation();
    }

    void OMGCustomerCalcUC::validateTenderDiscount( basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleStart, basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleEnd )
    {
         METHODNAME_DEF( OMGCustomerCalcUC, validateTenderDiscount )
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
            basar::Int32 deliveredQty = yitCurrent.getInt32( properties::DELIVERY_QTY ); 
            basar::Decimal percentageDiff = basar::Decimal(0.0);
            basar::Decimal priceDiff = basar::Decimal(0.0);
            if( m_TenderDiscPct > tenderProgDiscPct )
            {

                percentageDiff = m_TenderDiscPct -tenderProgDiscPct;
                priceDiff = basePrice * (percentageDiff/100) * deliveredQty;
            }
            else
            {
                percentageDiff = tenderProgDiscPct - m_TenderDiscPct;
                priceDiff = basePrice * (percentageDiff/100) * deliveredQty;
            }
            createPosition( yitCurrent.getInt32( properties::TENDER_NO ), yitCurrent.getInt32( properties::CUSTOMER_NO ),
                                    yitCurrent.getInt32( properties::ARTICLE_NO ), priceDiff );
        }
    }

    void OMGCustomerCalcUC::checkDeliveredQuantity( basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleStart, basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleEnd )
    {
        METHODNAME_DEF( OMGCustomerCalcUC, checkDeliveredQuantity )
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
                ss<< "Delivered quntity exceeds the contract quantity. Total exceeding quantity = " << qtyExceeded;
                BLOG_ERROR( m_Logger, ss.str() );
            }
        }
        catch( const basar::Exception & e )
        {
            BLOG_ERROR( m_Logger, e.what().c_str() );
        }
    }

    basar::Int32 OMGCustomerCalcUC::getContractQuantity( basar::Int32 articleNo)
    {
        METHODNAME_DEF( OMGCustomerCalcUC, getContractQuantity )
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
                    //m_TenderDiscPct = yitTenPos.getDecimal( properties::ADD_DISCOUNT_PCT );
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

    void OMGCustomerCalcUC::saveCalculation()
    {
        METHODNAME_DEF( OMGCustomerCalcUC, saveCalculation )
        BLOG_TRACE_METHOD( getLogger(), fun );

        //m_OMGCustomerCalcDM->save();
    }

    void OMGCustomerCalcUC::createPosition(basar::Int32 tenderNo, basar::Int32 customerNo, basar::Int32 articleNO, basar::Decimal priceDiff, bool transFlag )
    {
        METHODNAME_DEF( OMGCustomerCalcUC, createPosition )
        BLOG_TRACE_METHOD( getLogger(), fun );

        basar::db::aspect::AccessorPropertyTable_YIterator yitOMGCal = m_OMGCustomerCalcDM->get().insert(basar::FOR_INSERT);

        yitOMGCal.setInt32(properties::TENDER_NO,tenderNo );
        yitOMGCal.setInt32(properties::CUSTOMER_NO,customerNo );
        yitOMGCal.setInt32(properties::ARTICLE_NO,articleNO );
        yitOMGCal.setDecimal(properties::PRICE_DIFFERENCE,priceDiff );
        yitOMGCal.setInt16(properties::TRANSFERED_FLAG,transFlag );
        m_OMGCustomerCalcDM->save( yitOMGCal );
    }

     const log4cplus::Logger& OMGCustomerCalcUC::getLogger() const
    {
        METHODNAME_DEF( OMGCustomerCalcUC, getLogger )
        BLOG_TRACE_METHOD( m_Logger, fun );

        return m_Logger;
    }

 } // namespace usecase
} // namespace tender