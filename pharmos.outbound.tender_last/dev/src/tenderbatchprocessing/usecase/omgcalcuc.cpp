//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "omgcalcuc.h"
#include <loggerpool/loggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>
#include <libbasardbaspect_exceptions.h>

#include "domainmodule/omgcalc/omgcalcdm.h"
#include "domainmodule/omgcalc/iomgcalc.h"

#include <domainmodule/tender/itenderprogress.h>
#include <domainmodule/tender/tenderprogressdm.h>

#include <domainmodule/tender/itendercollectiondm.h>
#include <domainmodule/tender/tendercollectiondm.h>

//-------------------------------------------------------------------------------------------------//
// namespace sections
//-------------------------------------------------------------------------------------------------//
namespace tenderbatchprocessing
{
namespace useCase
{
    //BEGIN_HANDLERS_REGISTRATION( OMGCalcUC )
    //    SYSTEM_EVENT_HANDLER_REGISTRATION( OMGCalcUC, InsertOMGCalcRequested )
    //END_HANDLERS_REGISTRATION()

    OMGCalcUC::OMGCalcUC()
    : m_Logger( tenderbatchprocessing::LoggerPool::getLoggerUseCases() )
    {
        METHODNAME_DEF( OMGCalcUC, OMGCalcUC )
        BLOG_TRACE_METHOD( m_Logger, fun );

        //registerEventHandlers( true );
    }

    OMGCalcUC::~OMGCalcUC()
    {
        METHODNAME_DEF( OMGCalcUC, ~OMGCalcUC )
        BLOG_TRACE_METHOD( m_Logger, fun );

        //registerEventHandlers( false );
    }

    void OMGCalcUC::injectOMGCalcDM(tenderbatchprocessing::domMod::omgcalc::IOMGCalcPtr omgCalcDM)
    {
        METHODNAME_DEF( OMGCalcUC, injectOMGCalcDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_OMGCalcDM = omgCalcDM;
    }

    void OMGCalcUC::injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgressDM )
    {
        METHODNAME_DEF( OMGCalcUC, injectTenderProgressDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_TenderProgressDM = tenderProgressDM;
    }

    void OMGCalcUC::injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM )
    {
        METHODNAME_DEF( OMGCalcUC, injectTenderCollectionDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_TenderCollectionDM = tenderCollectionDM;
    }

    void OMGCalcUC::run()
    {
        METHODNAME_DEF( OMGCalcUC, run )
        BLOG_TRACE_METHOD( getLogger(), fun );

// TODO SK: implement
/*
	m_TenderProgressDM->findTenderProgressForOMGCalcultion( m_Tender->getTenderNo() )
	yitEnd = m_TenderProgressDM->get().begin();
	while yitEnd != end()
		yitStart = yitEnd;
		yitEnd = getEndOfArticleGroup( yitStart );
		processArticle( yitStart, yitEnd );
*/


    }

/*
// TODO SK: implement
yit OMGCalcUC::getEndOfArticleGroup( yitStart )
{
	// returns the yit containing the next article number
	yitEnd = yitStart;
	while yitEnd != end() and yitEnd.getInt32( ARTICLE_NO ) == yitStart.getInt32( ARTICLE_NO )
		++yitEnd

	return yitEnd
}

void OMGCalcUC::processArticle( yitStart, yitEnd )
{
	calculate( yitStart, yitEnd )
	saveCalculation()
}

void OMGCalcUC::calculate( yitStart, yitEnd )
{
	for yitCurrent = yitStart; yitCurrent != YitEnd; ++yitCurrent
		summarize delivered quantity
		if sum exceeds quantity from tender position
			qty = get exceeded qty from yitCurrent
			processExceedance( yitCurrent, qty )
			++yitCurrent
			for ; yitCurrent != YitEnd; ++yitCurrent
				processExceedance( yitCurrent, qty )

tender qty 20

customerno, 
5, price
10
10   sum = 25, 5 -> invoice for the customer
4	-> invoice for the customer
6
10
20

}

void OMGCalcUC::processExceedance( yitCurrent, qty )
{
	if tender->negativeContingentAllowed
		if findSuccessorTender( customer, yitCurrent.getArticleNo() )
			get difference in discountPercentage
			priceDifference = calc credit note or invoice for customer
			update tenderprogress with new tender number
			m_OMGCalcDM->createPosition( customerNo, articleNo, priceDifference )
			return


yitCurrent - old tender 20%
case 1: SuccessorTender -new tender 10% -> invoice for 10%
case 2: new tender 25% -> credit note for 5%



	price = calc invoice for customer
	m_OMGCalcDM->createPosition( customerNo, articleNo, price )
}

*/

    const log4cplus::Logger OMGCalcUC::getLogger() const
    {
        METHODNAME_DEF( OMGCalcUC, getLogger )
        BLOG_TRACE_METHOD( m_Logger, fun );

        return m_Logger;
    }


    //SYSTEM_EVENT_HANDLER_DEFINITION( OMGCalcUC, InsertOMGCalcRequested )
    //{
    //    METHODNAME_DEF( TenderInvoiceUC, InsertOMGCalcRequested )
    //    BLOG_TRACE_METHOD( m_Logger, fun );
    //    rSource;
    //    try
    //    {
    //    }
    //    catch( const basar::Exception & e )
    //    {
    //        BLOG_ERROR( m_Logger, e.what().c_str() );
    //        throw;
    //    }
    //    return  basar::appl::EventReturnType( basar::appl::HANDLER_OK );;
    //}
 } // namespace usecase
} // namespace tender