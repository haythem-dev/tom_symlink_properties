#include "usecase/mainuc.h"
#include "loggerpool/loggerpool.h"
#include "icmdlineparametergetter.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <domainmodule/tender/itendercollectiondm.h>
#include <domainmodule/tender/tenderstateenum.h>
#include <domainmodule/tender/itender.h>
#include <domainmodule/tender/itenderheader.h>
#include <domainmodule/tender/itenderpositioncollection.h>
#include <infrastructure/mail/imailalert.h>
#include <infrastructure/countrycodemapper.h>

#include <libutil/util.h>
#include <libbasardbaspect.h>
#include <libbasarcmnutil_email.h>


namespace tenderbatchprocessing {
namespace useCase {

MainUC::MainUC() : libutil::useCase::IRunnable(),
	m_Logger(LoggerPool::getLoggerUseCases())
{
	METHODNAME_DEF( MainUC, MainUC )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_TenderEndDateOffset = 1;
}

MainUC::~MainUC()
{
	METHODNAME_DEF( MainUC, ~MainUC )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

void MainUC::injectCmdLineParameterGetter( infrastructure::ICmdLineParameterGetterPtr cmdLineParameterGetter )
{
	METHODNAME_DEF( MainUC, injectCmdLineParameterGetter )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_CmdLineParameterGetter = cmdLineParameterGetter;
}

void MainUC::injectMailAlert( infrastructure::mail::IMailAlertPtr mailAlert )
{
	METHODNAME_DEF( MainUC, injectMailAlert )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_MailAlert = mailAlert;
}

void MainUC::injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM )
{
    METHODNAME_DEF( TenderOverviewUC, injectTenderCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_TenderCollection = tenderCollectionDM;
}

void MainUC::injectSuccessorTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM )
{
    METHODNAME_DEF( TenderOverviewUC, injectSuccessorTenderCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_SuccessorTenderCollection = tenderCollectionDM;
}

void MainUC::setOMGDuration(basar::Int32 duration)
{
	m_OMGDuration = duration;
}

void MainUC::setTenderEndDateOffset(basar::Int32 offset)
{
	m_TenderEndDateOffset = offset;
}

infrastructure::ICmdLineParameterGetterPtr MainUC::getCmdLineParameterGetter()
{
	CHECK_INSTANCE_EXCEPTION( m_CmdLineParameterGetter.get() );
	return m_CmdLineParameterGetter;
}

infrastructure::mail::IMailAlertPtr MainUC::getMailAlert()
{
	CHECK_INSTANCE_EXCEPTION( m_MailAlert.get() );
	return m_MailAlert;
}

basar::Date MainUC::determineStartDate()
{
	basar::Date dateFrom = getCmdLineParameterGetter()->getDateFrom();

	if (false == dateFrom.isValid())
	{
		dateFrom = basar::cmnutil::Date::getCurrent();
	}
	return dateFrom;
}

basar::Date MainUC::determineEndDate()
{
	basar::Date dateTo = getCmdLineParameterGetter()->getDateTo();
	if (false == dateTo.isValid())
	{
		// otherwise relative from startdate
		dateTo = determineStartDate();
		if (getCmdLineParameterGetter()->getNextDays() > 0)
		{
			dateTo.addDays(getCmdLineParameterGetter()->getNextDays());
		}
		else
		{
			dateTo.addDays(1); // default: today and tomorrow
		}
	}
	return dateTo;
}

libtender::domMod::tender::ITenderCollectionDMPtr MainUC::getTenderCollectionDM()
{
    METHODNAME_DEF( MainUC, getTenderCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    CHECK_INSTANCE_EXCEPTION( m_TenderCollection.get() );
    return m_TenderCollection;
}

libtender::domMod::tender::ITenderCollectionDMPtr MainUC::getSuccessorTenderCollectionDM()
{
    METHODNAME_DEF( MainUC, getTenderCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    CHECK_INSTANCE_EXCEPTION( m_SuccessorTenderCollection.get() );
    return m_SuccessorTenderCollection;
}

void MainUC::run()
{
    METHODNAME_DEF( MainUC, run )
    BLOG_TRACE_METHOD( m_Logger, fun );

    try
    {
		libtender::infrastructure::CountryIDEnum countryID = libtender::infrastructure::CountryCodeMapper::getCountryID(getCmdLineParameterGetter()->getCountry());

		basar::db::aspect::AccessorPropertyTable_YIterator searchYit = getTenderCollectionDM()->getSearchYit();
        searchYit.setDate(properties::DATE_TO, determineEndDate());
        getTenderCollectionDM()->findExpiredTenders(searchYit);

		basar::db::aspect::AccessorPropertyTableRef tenderHeads = getTenderCollectionDM()->get();
		basar::db::aspect::AccessorPropertyTable_YIterator tenderHeadYit = tenderHeads.begin();
        
		for(; !tenderHeadYit.isEnd(); ++tenderHeadYit)
        {
            libtender::domMod::tender::ITenderPtr tender = getTenderCollectionDM()->getTender(tenderHeadYit);

			libtender::domMod::tender::ITenderPositionCollectionPtr positions = tender->getPositionCollection();
			positions->setState(libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED);
			tender->setTenderState(libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED);

			if(libtender::infrastructure::CID_CH == countryID)
			{
				basar::db::aspect::AccessorPropertyTable_YIterator tenderPosYit = positions->get().begin();
				for( ; !tenderPosYit.isEnd(); ++tenderPosYit )
				{
					getSuccessorTenderCollectionDM()->resetSearchYit();
					basar::db::aspect::AccessorPropertyTable_YIterator findValidTender = getSuccessorTenderCollectionDM()->getSearchYit();
					findValidTender.setString(properties::PHARMACY_GROUPID, tender->getHeader().getString(properties::PHARMACY_GROUPID));
					findValidTender.setInt32(properties::ARTICLE_NO, tenderPosYit.getInt32(properties::ARTICLE_NO));
					findValidTender.setInt32(properties::TENDER_NO, tender->getHeader().getInt32(properties::TENDER_NO));
					findValidTender.setInt16(properties::TENDER_STATE, libtender::domMod::tender::TS_UNCOMPLETED);
					findValidTender.setDate(properties::DATE_TO, determineEndDate());
					getSuccessorTenderCollectionDM()->findValidTenders(findValidTender);
				
					if( false == getSuccessorTenderCollectionDM()->isEmpty() ) // Checking Successor Tender 
					{
						libtender::domMod::tender::ITenderPtr successorTender = getSuccessorTenderCollectionDM()->getTender(getSuccessorTenderCollectionDM()->get().begin());

						successorTender->getHeader().setInt16(properties::TENDER_STATE, libtender::domMod::tender::TS_OK);
						successorTender->getHeader().setDate(properties::DATE_FROM, basar::cmnutil::Date::getCurrent());
						successorTender->getHeader().setDate(properties::DATE_TO, calculateTenderEndDate());

						successorTender->getPositionCollection()->setState(libtender::domMod::tender::TS_OK);

						successorTender->save();
					}
				}
			}

			tender->save();
		}
	}
    catch(const basar::Exception & e)
    {
        BLOG_ERROR(m_Logger, e.what().c_str());
    }
}

basar::Date	MainUC::calculateTenderEndDate()
{
	METHODNAME_DEF( MainUC, calculateTenderEndDate )
    BLOG_TRACE_METHOD( m_Logger, fun );

	using basar::cmnutil::Date;

    Date dateTo = Date::getCurrent();
	dateTo.addMonths(m_OMGDuration);
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

} // namespace usecase
} // namespace tenderbatchprocessing
