#include "usecase/mainuc.h"
#include "loggerpool/loggerpool.h"
#include "icmdlineparametergetter.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverycollectiondm.h>
#include <domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryheaderptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryheader.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverypositioncollectionptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverypositioncollection.h>
#include "domainmodule/cscorder/icscorder.h"
#include "domainmodule/cscorder/icscorderposition.h"

#include <infrastructure/mail/imailalert.h>
#include <libutil/util.h>
#include <libutil/exception.h>

#include <libbasardbaspect.h>
#include <libbasarcmnutil_email.h>

#ifdef WIN32
#pragma warning (push)
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4127) // conditional expression is constant
#pragma warning (disable : 4505) // unreferenced local function has been removed
#pragma warning (disable : 4706)
#endif
#include <gen-cpp/cscorder_types.h>
#ifdef WIN32
#pragma warning (pop)
#endif

using namespace csc::base;
using namespace csc::service;

//#define TENDER_KSC_SERVER

//-------------------------------------------------------------------------------------------------//

namespace tenderordercreator {
namespace useCase {

//-------------------------------------------------------------------------------------------------//

MainUC::MainUC() :
	m_Logger(LoggerPool::getLoggerUseCases())
{
	METHODNAME_DEF( MainUC, MainUC )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

//-------------------------------------------------------------------------------------------------//

MainUC::~MainUC()
{
	METHODNAME_DEF( MainUC, ~MainUC )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

//-------------------------------------------------------------------------------------------------//

void MainUC::injectScheduledDeliveryCollectionDM( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollectionDM )
{
	METHODNAME_DEF( MainUC, injectScheduledDeliveryCollectionDM )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_ScheduledDeliveryCollection = scheduledDeliveryCollectionDM;
}

//-------------------------------------------------------------------------------------------------//

void MainUC::injectCmdLineParameterGetter( infrastructure::ICmdLineParameterGetterPtr cmdLineParameterGetter )
{
	METHODNAME_DEF( MainUC, injectCmdLineParameterGetter )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_CmdLineParameterGetter = cmdLineParameterGetter;
}

//-------------------------------------------------------------------------------------------------//

void MainUC::injectMailAlert( infrastructure::mail::IMailAlertPtr mailAlert )
{
	METHODNAME_DEF( MainUC, injectMailAlert )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_MailAlert = mailAlert;
}

//-------------------------------------------------------------------------------------------------//

void MainUC::injectCscOrderDM( libtender::domMod::cscorder::ICscOrderPtr cscOrderDM )
{
	METHODNAME_DEF( MainUC, injectCscOrderDM )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_CscOrderDM = cscOrderDM;
}

void MainUC::injectOrderType( basar::VarString orderType )
{
	METHODNAME_DEF( MainUC, injectOrderType )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_OrderType = orderType;
}

//-------------------------------------------------------------------------------------------------//

infrastructure::ICmdLineParameterGetterPtr MainUC::getCmdLineParameterGetter()
{
	CHECK_INSTANCE_EXCEPTION( m_CmdLineParameterGetter.get() );
	return m_CmdLineParameterGetter;
}

//-------------------------------------------------------------------------------------------------//

infrastructure::mail::IMailAlertPtr MainUC::getMailAlert()
{
	CHECK_INSTANCE_EXCEPTION( m_MailAlert.get() );
	return m_MailAlert;
}

//-------------------------------------------------------------------------------------------------//

libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr MainUC::getScheduledDeliveryCollectionDM()
{
	CHECK_INSTANCE_EXCEPTION( m_ScheduledDeliveryCollection.get() );
	return m_ScheduledDeliveryCollection;
}

//-------------------------------------------------------------------------------------------------//

libtender::domMod::cscorder::ICscOrderPtr MainUC::getCscOrderDM()
{
	CHECK_INSTANCE_EXCEPTION( m_CscOrderDM.get() );
	return m_CscOrderDM;
}

//-------------------------------------------------------------------------------------------------//

basar::Date MainUC::determineStartDate()
{
	basar::Date dateFrom = getCmdLineParameterGetter()->getDateFrom();

	if (false == dateFrom.isValid())
	{
		dateFrom = basar::cmnutil::Date::getCurrent();
	}
	return dateFrom;
}

//-------------------------------------------------------------------------------------------------//

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

//-------------------------------------------------------------------------------------------------//

void MainUC::run()
{
	METHODNAME_DEF( MainUC, run )
	BLOG_TRACE_METHOD( m_Logger, fun );

	getScheduledDeliveryCollectionDM()->getSearchYit().setDate(properties::DELIVERY_DATE_FROM, determineStartDate());
	getScheduledDeliveryCollectionDM()->getSearchYit().setDate(properties::DELIVERY_DATE_TO, determineEndDate());
	getScheduledDeliveryCollectionDM()->getSearchYit().setInt32(properties::CSC_ORDER_NO, 0);
	getScheduledDeliveryCollectionDM()->findByPattern( getScheduledDeliveryCollectionDM()->getSearchYit() );

	basar::db::aspect::AccessorPropertyTable_YIterator yit;
	yit = getScheduledDeliveryCollectionDM()->get().begin();
	
	while( !yit.isEnd() )
	{
		libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery;
		scheduledDelivery = getScheduledDeliveryCollectionDM()->getScheduledDelivery(yit);
		createCscOrder(scheduledDelivery);
		++yit;
	}

	if( !m_OrderErrorList.empty() )
	{
		sendErrorMail(m_OrderErrorList);
	}
}

//-------------------------------------------------------------------------------------------------//
//#define TENDER_KSC_SERVER
#ifdef TENDER_KSC_SERVER

void MainUC::createCscOrder(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery)
{
	METHODNAME_DEF( MainUC, createCscOrder )
	BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::scheduledDelivery::IScheduledDeliveryHeaderPtr head = scheduledDelivery->getHeader();
	libtender::domMod::scheduledDelivery::IScheduledDeliveryPositionCollectionPtr positions = scheduledDelivery->getPositionCollection();
	basar::Int32 cscRetVal;
	basar::cmnutil::CollBString	posErrorList;

    LOG_YIT( head->get(), m_Logger, "");
    LOG_PROPTAB( positions->get(), m_Logger, "");

	cscRetVal = getCscOrderDM()->openOrderVz(head->getBranchNo(), head->getCustomerNo(), m_OrderType);
	if (cscRetVal != 0)
	{
		std::stringstream ss;
		ss << "CustNo=" << head->getCustomerNo() << "openOrder rc=" << cscRetVal << ", ReasonNo=" << getCscOrderDM()->getLackReason() << "Text=" << getCscOrderDM()->getText();
		BLOG_ERROR(m_Logger, ss.str());
		posErrorList.push_back(ss.str());
		sendErrorMail(scheduledDelivery, posErrorList);
		return;
	}
	cscRetVal = getCscOrderDM()->setDeliveryDate(head->getDeliveryDate());
	cscRetVal = getCscOrderDM()->setTenderNo(head->getTenderId());
	
	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = positions->get().begin();
	while (!yitPos.isEnd())
	{
		libtender::domMod::cscorder::ICscOrderPositionPtr cscPos;
		
		cscPos = createCscOrderPosition(yitPos);
		basar::Int32 qtyDelivered;

		qtyDelivered = cscPos->getDeliveryQuantity() + cscPos->getIBTQuantity();

		if( cscPos->getLackReason() > 0 )
		{
			std::stringstream ss;
			ss << cscPos->getOrderQuantity() << " x Article " << cscPos->getArticleCode() << ": delivered "
			   << cscPos->getDeliveryQuantity() + cscPos->getIBTQuantity() << " "
			   << cscPos->getLackReason() << " " << cscPos->getText();
			BLOG_ERROR(m_Logger, ss.str());
			 
			if( cscPos->getOrderQuantity() > qtyDelivered )
			{
				posErrorList.push_back(ss.str());
			}
		}
		
		yitPos.setInt32(properties::QUANTITY_DELIVERED, -1); // BAD EVIL HACK FROM MR.KAPUSTA
		yitPos.setInt32(properties::QUANTITY_DELIVERED, qtyDelivered);
		++yitPos;
	}

	getCscOrderDM()->closeOrder();

	head->setCscOrderNo(getCscOrderDM()->getOrderNo());
	head->setCscOrderDate(getCscOrderDM()->getTourDate());

	scheduledDelivery->save();

	if (false == posErrorList.empty())
	{
		prepareErrorMail(scheduledDelivery, posErrorList);
	}
}
#else //CSC Service
void MainUC::createCscOrder(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery)
{
	METHODNAME_DEF(MainUC, createCscOrder)
	BLOG_TRACE_METHOD(m_Logger, fun);

	csc::order::PlaceOrderResponse response = getCscOrderDM()->createCscOrder(scheduledDelivery);
	
	//TODO put into separate method
	if(response.OpenOrder.RetValue.ErrorCode != csc::types::MessageCodeEnum::CMSG_OK)
	{
		std::stringstream ss;
		ss << "OpenOrder error for tenderscheduleddeliveryid " << scheduledDelivery->getHeader()->getScheduledDeliveryId();
		ss << " BranchNo: " << scheduledDelivery->getHeader()->getBranchNo() << " CustomerNo: " << scheduledDelivery->getHeader()->getCustomerNo();
		ss << " ErrorCode: " << response.OpenOrder.RetValue.ErrorCode << " ErrorMessage: " << response.OpenOrder.RetValue.ErrorMessage ;
		BLOG_ERROR(m_Logger, ss.str());

        m_OrderErrorList.push_back(ss.str());
	}
	
	if (response.CloseOrder.RetValue.ErrorCode != csc::types::MessageCodeEnum::CMSG_OK)
	{
		std::stringstream ss;
		ss << "CloseOrder error for tenderscheduleddeliveryid " << scheduledDelivery->getHeader()->getScheduledDeliveryId();
		ss << " BranchNo: " << scheduledDelivery->getHeader()->getBranchNo() << " CustomerNo: " << scheduledDelivery->getHeader()->getCustomerNo();
		ss << " ErrorCode: " << response.CloseOrder.RetValue.ErrorCode << " ErrorMessage: " << response.CloseOrder.RetValue.ErrorMessage;
		BLOG_ERROR(m_Logger, ss.str());

		m_OrderErrorList.push_back(ss.str());
	}

	csc::order::OrderPositionResponseList::const_iterator it = response.Positions.begin();

	for (; it != response.Positions.end(); ++it)
	{
		if (it->RetValue.ErrorCode != csc::types::MessageCodeEnum::CMSG_OK)
		{
			std::stringstream ss;
			ss << "Position error for tenderscheduleddeliveryid " << scheduledDelivery->getHeader()->getScheduledDeliveryId();
			ss << " BranchNo: " << scheduledDelivery->getHeader()->getBranchNo() << " CustomerNo: " << scheduledDelivery->getHeader()->getCustomerNo();
			ss << " ArticleCode: " << it->ArticleDelivered.ArticleCode << " OrderNo: " << it->OrderNo;
			ss << " QtyOrdered: " << it->QuantityOrdered << " QtyLack: " << it->QuantityLack << " QtyIBT: " << it->QuantityInterBranch;
			ss << " ErrorCode: " << it->RetValue.ErrorCode << " ErrorMessage: " << it->RetValue.ErrorMessage;
			BLOG_ERROR(m_Logger, ss.str());

			m_OrderErrorList.push_back(ss.str());
		}
	}

	//send error mail
	if (!m_OrderErrorList.empty())
	{
		sendErrorMail(scheduledDelivery, m_OrderErrorList);
		m_OrderErrorList.clear();
	}
	
}
#endif //TENDER_KSC_SERVER

libtender::domMod::cscorder::ICscOrderPositionPtr MainUC::createCscOrderPosition(basar::db::aspect::AccessorPropertyTable_YIterator yitPos)
{
	basar::VarString articleCode	= yitPos.getString(properties::ARTICLE_CODE);
	basar::Int32 quantity			= yitPos.getInt32(properties::QUANTITY);
	libtender::domMod::cscorder::ICscOrderPositionPtr cscPos;

	//		cscPos = getCscOrderDM()->addOrderItem(articleCode, quantity);
	cscPos = getCscOrderDM()->createEmptyPosition(articleCode, quantity);
	cscPos->setIbtPossible(true);
	cscPos->setRestDeliveryPossible(false);
	cscPos->setDispoPossible(false);
	cscPos->setPartialDeliveryPossible(true);
	cscPos->save();

	return cscPos;
}

void MainUC::prepareErrorMail(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery, const basar::cmnutil::CollBString& errorList)
{
	std::stringstream body;
	
	body << "BranchNo: "		<< scheduledDelivery->getHeader()->getBranchNo()		<< std::endl;
	body << "CustomerNo: "		<< scheduledDelivery->getHeader()->getCustomerNo()		<< std::endl;
	body << "DeliveryDate: "	<< scheduledDelivery->getHeader()->getDeliveryDate()	<< std::endl;

	if(scheduledDelivery->getHeader()->getCscOrderNo() > 0)
	{
		body << "CSC OrderNo: " << scheduledDelivery->getHeader()->getCscOrderNo() << std::endl << std::endl;
	}

	basar::cmnutil::CollBString::const_iterator it = errorList.begin();
	while (it != errorList.end())
	{
		body << (*it) << std::endl;
		++it;
	}

	m_OrderErrorList.push_back(body.str());
}

const log4cplus::Logger& MainUC::getLogger() const
{
    return m_Logger;
}

void MainUC::sendErrorMail(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery, const basar::cmnutil::CollBString& errorList)
{

    METHODNAME_DEF( MainUC, sendErrorMail )
    BLOG_TRACE_METHOD( getLogger(), fun );

	std::stringstream body;
	body << "Error while creating order for scheduled delivery" << std::endl;
	body << "BranchNo: " << scheduledDelivery->getHeader()->getBranchNo() << std::endl;
	body << "CustomerNo: " << scheduledDelivery->getHeader()->getCustomerNo() << std::endl;
	body << "DeliveryDate: " << scheduledDelivery->getHeader()->getDeliveryDate() << std::endl;
	if(scheduledDelivery->getHeader()->getCscOrderNo() > 0)
	{
		body << "CSC OrderNo: " << scheduledDelivery->getHeader()->getCscOrderNo() << std::endl;
	}
	body << std::endl;
	basar::cmnutil::CollBString::const_iterator it = errorList.begin();
	while( it != errorList.end() )
	{
		body << (*it) << std::endl;
		++it;
	}

    try
    {
	    getMailAlert()->send( "Error Tenderordercreator", body.str() );
    }
    catch( basar::Exception& e )
    {
           libutil::exceptions::ExceptionHandler::processBasarException( e, getLogger(), fun, __FILE__, __LINE__ );
    }
}

void MainUC::sendErrorMail(const basar::cmnutil::CollBString& errorList)
{

    METHODNAME_DEF( MainUC, senderrorMail )
    BLOG_TRACE_METHOD( getLogger(), fun );

	std::stringstream body;
	body << "Error while creating orders..." << std::endl << std::endl;

	basar::cmnutil::CollBString::const_iterator it = errorList.begin();
	while( it != errorList.end() )
	{
		body << (*it) << std::endl;
		body << "---------------------------------------------------------------------" << std::endl;
		++it;
	}
    try
    {
	    getMailAlert()->send( "Error Tenderordercreator", body.str() );
    }
    catch( basar::Exception& e )
    {
           libutil::exceptions::ExceptionHandler::processBasarException( e, getLogger(), fun, __FILE__, __LINE__ );
    }

}

//-------------------------------------------------------------------------------------------------//

} // namespace usecase
} // namespace tenderordercreator

//-------------------------------------------------------------------------------------------------//
