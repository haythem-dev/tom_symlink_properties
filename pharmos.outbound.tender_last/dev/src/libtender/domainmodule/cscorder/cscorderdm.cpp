#include "cscorderdm.h"
#include "cscorderposition.h"
#include "cscclienthelper.h"
#include <libutil/misc/metaclassmacro_definitions.h> // libutil
#include <libutil/misc/log_macro_definitions.h>      // libutil
#include <loggerpool/tenderloggerpool.h>
#include "infrastructure/cscclient/icscclientconnection.h"
#include "infrastructure/cscclient/icscclientconnectionptr.h"

#include <kscclient/articleinfostruct.h>
#include <kscclient/articleinfostructcode.h>
#include <kscclient/orderinfo.h>
//#include <callbackinquiry.h>
//#include <newcallbackcritpos.h>
//#include <newcallback.h>
//#include <callbackreasoninquiry.h>
//#include <callbackdivisioninquiry.h>
#include <kscclient/callksc.h>

#include <domainmodule/libtender_properties_definitions.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverycollectiondm.h>
#include <domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryheaderptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryheader.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverypositioncollectionptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverypositioncollection.h>
#include "domainmodule/cscorder/icscorder.h"
#include "domainmodule/cscorder/icscorderposition.h"
#include <infrastructure/cscclient/icscclientparameterproxy.h>
#include <infrastructure/cscclient/icscclientparameterproxyptr.h>

#include <libbasardbaspect.h>

#ifdef WIN32
#pragma warning (push)
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4127) // conditional expression is constant
#pragma warning (disable : 4505) // unreferenced local function has been removed
#pragma warning (disable : 4706)
#endif
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TEncodedProtocol.h>
#include <thrift/protocol/BoostEncoder.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/TApplicationException.h>
#include <gen-cpp/CscServiceBatch.h>
#include <gen-cpp/cscbase_constants.h>
#ifdef WIN32
#pragma warning (pop)
#endif


using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace csc::base;
using namespace csc::service;


//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace cscorder {

//-----------------------------------------------------------------------------------------

CscOrderDM::CscOrderDM(const log4cplus::Logger& logger) :
	m_Logger(logger)
{
	init();
}

CscOrderDM::~CscOrderDM()
{
	// TODO	wo / wann disconnecten
	//m_Connection->disconnect();
}

void CscOrderDM::injectCscClientConnection( infrastructure::cscclient::ICscClientConnectionPtr conn )
{
	m_Connection = conn;
	// TODO	wo / wann connecten
	//m_Connection->connect();
}

void CscOrderDM::init()
{
	m_BranchNo = 0;
	m_CustomerNo = 0;
	m_OrderNo = 0;
	m_PharmacyName.clear();
	m_DeliveryDate = basar::Date();
	m_DeliveryTime = basar::Time();
	m_TourDate = basar::Date();
	m_ValueOfOrder = basar::Decimal(0.0);
	m_LinesOfOrder = 0;
	m_LackReason = 0;
	m_OrderType.clear();
	m_Text.clear();

	m_OrderPositions.clear();
}

basar::Int32 CscOrderDM::openOrderVz(const basar::Int16 branchNo, const basar::Int32 customerNo, const basar::VarString& orderType,  const basar::VarString& subOrderType)
{
	METHODNAME_DEF( CscOrderDM, openOrderVz )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Int32	ret;
	orderinfoS		info;

	init();
	memset(&info, 0, sizeof(orderinfoS));

	m_BranchNo = branchNo;
	m_CustomerNo = customerNo;

	std::stringstream ss;
	ss <<  "openOrderVz(" << branchNo << ", " << customerNo << ", \"" << orderType << "\", \"" << subOrderType << "\")";
	BLOG_DEBUG(getLogger(), ss.str());

	ret = ::OpenOrderVz(
		branchNo,
		customerNo,
		const_cast<char*>(orderType.c_str()),
		const_cast<char*>(subOrderType.c_str()),
		&info);

	m_OrderNo		= info.ORDERNO;
	m_PharmacyName	= CscClientHelper::getStringFromStruct(info.PHARMACYNAME, 30);
	m_LackReason	= info.LACKREASON;
	m_Text			= CscClientHelper::getStringFromStruct(info.TEXT, 240);

	return ret;
}

basar::Int32 CscOrderDM::openOrderReferenceVzType( const basar::Int16 branchNo, const basar::Int32 customerNo,
												   const basar::VarString & orderType,
												   const basar::VarString & pickingType, const basar::VarString & bookingType,
												   const basar::VarString & reference /* = basar::VarString() */,
												   const basar::VarString & subOrderType /* = basar::VarString() */ )
{
	METHODNAME_DEF( CscOrderDM, openOrderReferenceVzType )
	BLOG_TRACE_METHOD( getLogger(), fun );

	orderinfoS info;

	init();
	memset( &info, 0, sizeof( orderinfoS ) );

	m_BranchNo = branchNo;
	m_CustomerNo = customerNo;

	std::stringstream ss;
	ss << "openOrderReferenceVzType( " << branchNo << ", " << customerNo << ", " << orderType << ", ";
	ss << pickingType << ", " << bookingType << ", " << reference << ", " << subOrderType << " )";
	BLOG_DEBUG( getLogger(), ss.str() );

	basar::Int32 ret = ::OpenOrderReferenceVzType( branchNo, customerNo, const_cast<char *>( reference.c_str() ), 
		const_cast<char *>( orderType.c_str() ), const_cast<char *>( pickingType.c_str() ), 
		const_cast<char *>( bookingType.c_str() ), const_cast<char *>( subOrderType.c_str() ), &info );

	m_OrderNo = info.ORDERNO;
	m_PharmacyName = CscClientHelper::getStringFromStruct( info.PHARMACYNAME, 30 );
	m_LackReason = info.LACKREASON;
	m_Text = CscClientHelper::getStringFromStruct( info.TEXT, 240 );

	return ret;
}

basar::Int32 CscOrderDM::cancelOrder()
{
    METHODNAME_DEF( CscOrderDM, cancelOrder )
    BLOG_TRACE_METHOD( getLogger(), fun );

    basar::Int32    ret;
    orderinfoS      info;

    memset(&info, 0, sizeof(orderinfoS));

    std::stringstream ss;
    ss <<  "cancelOrder()";
    BLOG_DEBUG(getLogger(), ss.str());

    ret = ::CancelOrder( &info );

    m_BranchNo = info.DELIVERYBRANCHNO;
    m_OrderNo = info.ORDERNO;
    m_DeliveryDate = basar::Date(info.DELIVERYDATE);
    m_DeliveryTime = basar::Time(info.TIMEDELIVERY);
    m_TourId = CscClientHelper::getStringFromStruct(info.TOURID, 6);
    m_TourDate = basar::Date(info.TOURDATE);
    m_ValueOfOrder = basar::Decimal(info.VALUEOFORDER);
    m_LinesOfOrder = info.LINESOFORDER;

    return ret;
}

basar::Int32 CscOrderDM::closeOrder()
{
	METHODNAME_DEF( CscOrderDM, closeOrder )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Int32	ret;
	orderinfoS		info;
	char			strText[81] = "";

	memset(&info, 0, sizeof(orderinfoS));

	std::stringstream ss;
	ss <<  "closeOrder()";
	BLOG_DEBUG(getLogger(), ss.str());

	ret = ::CloseOrder(strText, &info);

	m_BranchNo = info.DELIVERYBRANCHNO; // maybe changed
	m_OrderNo = info.ORDERNO; // maybe changed
	m_DeliveryDate = basar::Date(info.DELIVERYDATE);
	m_DeliveryTime = basar::Time(info.TIMEDELIVERY);
	m_TourId = CscClientHelper::getStringFromStruct(info.TOURID, 6);
	m_TourDate = basar::Date(info.TOURDATE);
	m_ValueOfOrder = basar::Decimal(info.VALUEOFORDER);
	m_LinesOfOrder = info.LINESOFORDER;
//d->TEXTLINES            = ps->Order()->AnzAlphaText();  //nase von ohne bestand auf alpha, bitte prüfen
//d->LACKLINES            = ps->Order()->AnzPosDafueFehler();
//d->WERTNETTO_FV			= ps->Order()->GetValueNetCash(0);
//d->WERTTAX_FV			= ps->Order()->GetValueSalesTax(0);
//d->WERTNETTO_KK			= ps->Order()->GetValueNetCash(1);
//d->WERTTAX_KK			= ps->Order()->GetValueSalesTax(1);
//d->WERTNETTO_EH			= ps->Order()->GetValueNetCash(2);
//d->VALRESTCL			= ps->Order()->Customer()->RestCreditLimit();
	return ret;
}

basar::Int32 CscOrderDM::setOrderType(const basar::VarString& orderType, const basar::VarString& pickingType, const basar::VarString& bookingType)
{
	METHODNAME_DEF( CscOrderDM, closeOrder )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Int32	ret;
	orderinfoS		info;

	memset(&info, 0, sizeof(orderinfoS));

	ret = ::SetOrderType(
		const_cast<char*>(orderType.c_str()),
		const_cast<char*>(pickingType.c_str()),
		const_cast<char*>(bookingType.c_str()),
		&info);
    m_Text = CscClientHelper::getStringFromStruct(info.TEXT, 240);
	m_OrderType = m_Text; //save order type for text future use
	return ret;
}

basar::Int32 CscOrderDM::setDeliveryDate(const basar::Date deliveryDate)
{
	METHODNAME_DEF( CscOrderDM, setDeliveryDate )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Int32	ret;
	orderinfoS		info;

	memset(&info, 0, sizeof(orderinfoS));

	ret = ::SetDeliveryDate(deliveryDate.getDate(), &info);

	m_DeliveryDate = basar::Date(info.DELIVERYDATE);
	m_TourDate = basar::Date(info.TOURDATE);

	return ret;
}

basar::Int32 CscOrderDM::setTenderNo(const basar::Int32 tenderNo)
{
	METHODNAME_DEF( CscOrderDM, setTenderNo )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Int32	ret;
	orderinfoS		info;

	memset(&info, 0, sizeof(orderinfoS));

	ret = ::SetTenderNo(tenderNo, &info);

	return ret;
}

basar::Int32 CscOrderDM::setOrderText(const basar::VarString& orderText)
{
	METHODNAME_DEF( CscOrderDM, setOrderText )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::Int32	ret;
	orderinfoS		info;

	memset(&info, 0, sizeof(orderinfoS));

	ret = ::SetOrderText(orderText.c_str(), &info);

	return ret;
}

ICscOrderPositionPtr CscOrderDM::createEmptyPosition(const basar::VarString& articleCode, const basar::Int32 qty)
{
	METHODNAME_DEF( CscOrderDM, createEmptyPosition )
	BLOG_TRACE_METHOD( getLogger(), fun );

	ICscOrderPositionPtr pos = ICscOrderPositionPtr(new CscOrderPosition(articleCode, qty, getLogger()));
	m_OrderPositions.push_back(pos);
	return pos;
}

ICscOrderPositionPtr CscOrderDM::addOrderItem(const basar::VarString& articleCode, const basar::Int32 qty)
{
	METHODNAME_DEF( CscOrderDM, addOrderItem )
	BLOG_TRACE_METHOD( getLogger(), fun );

	orderinfoS		info;

	memset(&info, 0, sizeof(orderinfoS));

	std::stringstream ss;
	ss <<  "addOrderItem(\"" << articleCode << "\", " << qty << ")";
	BLOG_DEBUG(getLogger(), ss.str());

	::AddOrderItemCode(
		qty,
		const_cast<char*>(articleCode.c_str()),
		0, /* auxiliary */
		0, /* restdelivery */
		0, /* dispose */
		1, /* part delivery */
		0, /* internal OrderNo */
		0, /* interne IDF-Nummer */
		0, /* Ursprungs Auftragsnummer */
		0, /* Ursprungs Positionsnummer */
		0, /* Preistyp */
		const_cast<char*>(""),  /* Bermerkungstext */
		&info);

	ICscOrderPositionPtr pos = ICscOrderPositionPtr(new CscOrderPosition(info, getLogger()));
	m_OrderPositions.push_back(pos);
	return pos;
}

void CscOrderDM::save(bool importing)
{
	METHODNAME_DEF( CscOrderDM, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

	// openorder

	std::list<ICscOrderPositionPtr>::const_iterator iter = m_OrderPositions.begin();
	while (iter != m_OrderPositions.end())
	{
		(*iter)->save(importing);
		++iter;
	}
	// closeorder
}

const log4cplus::Logger& CscOrderDM::getLogger() const
{
	return m_Logger;
}

basar::Int32 CscOrderDM::getOrderNo()
{
	return m_OrderNo;
}

basar::Date CscOrderDM::getDeliveryDate()
{
	return m_DeliveryDate;
}

basar::Date CscOrderDM::getTourDate()
{
	return m_TourDate;
}

basar::Int32 CscOrderDM::getLackReason()
{
	return m_LackReason;
}

basar::VarString CscOrderDM::getText()
{
	return m_Text;
}

basar::Int32 CscOrderDM::getCustomerNo()
{
	return m_CustomerNo;
}

csc::order::PlaceOrderResponse CscOrderDM::createCscOrder(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery)
{
	METHODNAME_DEF(CscOrderDM, createCscOrder)
	BLOG_TRACE_METHOD(getLogger(), fun);

	libtender::infrastructure::cscclient::ICscClientParameterProxyPtr paramProxy = m_Connection->getParamProxy();

	boost::shared_ptr<TTransport>				socket(new TSocket(paramProxy->getCscServer(), paramProxy->getCscPort()));
	boost::shared_ptr<TTransport>				transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol>				binaryProtocol(new TBinaryProtocol(transport));
	boost::shared_ptr<TEncoder>					encoder(new BoostEncoder(BoostEncoder::CodePage::WEST));
	boost::shared_ptr<TProtocol>				encodedProtocol(new TEncodedProtocol(binaryProtocol, encoder));
	boost::shared_ptr<CscServiceBatchClient>	client(new CscServiceBatchClient(encodedProtocol));

	BLOG_INFO(m_Logger, "Creating CSC order....");

	libtender::domMod::scheduledDelivery::IScheduledDeliveryHeaderPtr head = scheduledDelivery->getHeader();
	libtender::domMod::scheduledDelivery::IScheduledDeliveryPositionCollectionPtr positions = scheduledDelivery->getPositionCollection();
	m_OrderType = paramProxy->getCscOrderType();

	csc::order::PlaceOrderRequest request;
	csc::order::PlaceOrderResponse response;

	// openorder
	csc::order::OpenOrderRequest openrequest;

	csc::base::SessionRequest	sessionRequest;
	csc::base::SessionResponse	sessionResponse;

	try
	{
		transport->open();
		sessionRequest.__set_DeviceName(paramProxy->getCscDeviceName());
		sessionRequest.__set_Uid(0);
		client->openSession(sessionResponse, sessionRequest);
	}
	catch (basar::Exception& e)
	{
		e; //avoid warning
		std::stringstream ss;
		ss << "Exception in " << fun;
		BLOG_DEBUG(getLogger(), ss.str());
	}

	
	openrequest.__set_TenderNo(head->getTenderId());
	openrequest.__set_BranchNo(head->getBranchNo());
	openrequest.__set_CustomerNo(head->getCustomerNo());
	openrequest.__set_OrderType(m_OrderType);
	openrequest.__set_BookingType("0");
	openrequest.__set_ShipmentType("0");	
	request.__set_OpenRequest(openrequest);

	
	// add positions
	csc::order::OrderPositionRequestList poslist;

	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = positions->get().begin();
	while (!yitPos.isEnd())
	{
		csc::order::OrderPositionRequest requestPos;

		requestPos.__set_PosType(csc::types::PositionTypeEnum::TypeProductStocked);
		requestPos.__set_ArticleCode(yitPos.getString(properties::ARTICLE_CODE));
		requestPos.__set_QuantityOrdered(yitPos.getInt32(properties::QUANTITY));
		requestPos.__set_AcceptInterBranchDelivery(true);
		requestPos.__set_AcceptPartialDelivery(true);
		poslist.push_back(requestPos);
		++yitPos;
	}

	request.__set_Positions(poslist);

	// close order
	csc::order::CloseOrderRequest closerequest;

	closerequest.__set_CloseMode(csc::order::OrderCloseMode::CLOSE_MODE_CLOSE);
	request.__set_CloseRequest(closerequest);

	client->placeNewOrder(response, request);

	client->closeSession(sessionResponse);
	transport->close();

	//set csc order no and date in scheduled delivery header
	head->setCscOrderNo(response.OpenOrder.OrderNo);
	head->setCscOrderDate(head->getDeliveryDate());

	//set quantity deliverd in scheduled delivery positions
	yitPos = positions->get().begin();
	for (; false == yitPos.isEnd(); ++yitPos)
	{
		csc::order::OrderPositionResponseList::const_iterator it = response.Positions.begin();
		
		for (; it !=  response.Positions.end(); ++it)
		{
			//get the scheduled delivery position with the same article code
			//articledelivered is set by cscservice, articleordered only if substitution is booked
			if (it->ArticleDelivered.ArticleCode == yitPos.getString(properties::ARTICLE_CODE))
			{
				yitPos.setInt32(properties::QUANTITY_DELIVERED, it->QuantityDelivered);
				break;
			}
			else if (it->ArticleOrdered.ArticleCode == yitPos.getString(properties::ARTICLE_CODE))
			{
				yitPos.setInt32(properties::QUANTITY_DELIVERED, it->QuantityOrdered);
				break;
			}
		}
	}

	scheduledDelivery->save();

	return response;
}

void CscOrderDM::createCscOrder(basar::db::aspect::AccessorPropertyTable_YIterator cscOrderCustCollYit, basar::Int32 customerNo, basar::Int16 branchNo, basar::VarString orderType, basar::Int32& orderNo)
{
	METHODNAME_DEF(CscOrderDM, createCscOrder)
	BLOG_TRACE_METHOD(getLogger(), fun);

	libtender::infrastructure::cscclient::ICscClientParameterProxyPtr paramProxy = m_Connection->getParamProxy();

	boost::shared_ptr<TTransport>				socket(new TSocket(paramProxy->getCscServer(), paramProxy->getCscPort()));
	boost::shared_ptr<TTransport>				transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol>				binaryProtocol(new TBinaryProtocol(transport));
	boost::shared_ptr<TEncoder>					encoder(new BoostEncoder(BoostEncoder::CodePage::WEST));
	boost::shared_ptr<TProtocol>				encodedProtocol(new TEncodedProtocol(binaryProtocol, encoder));
	boost::shared_ptr<CscServiceBatchClient>	client(new CscServiceBatchClient(encodedProtocol));

	m_OrderType = paramProxy->getCscOrderType();


	csc::order::PlaceOrderRequest request;
	csc::order::PlaceOrderResponse response;

	// openorder
	csc::order::OpenOrderRequest openrequest;

	csc::base::SessionRequest	sessionRequest;
	csc::base::SessionResponse	sessionResponse;

	try
	{
		transport->open();
		sessionRequest.__set_DeviceName(paramProxy->getCscDeviceName());
		sessionRequest.__set_Uid(0);
		client->openSession(sessionResponse, sessionRequest);
	}
	catch (basar::Exception& e)
	{
		e; //avoid warning
		std::stringstream ss;
		ss << "Exception in " << fun;
		BLOG_DEBUG(getLogger(), ss.str());		
	}

	m_CustomerNo = customerNo;
	m_BranchNo = branchNo;
	m_OrderType = orderType;

	openrequest.__set_BranchNo(branchNo);
	openrequest.__set_CustomerNo(customerNo);
	openrequest.__set_OrderType(orderType);
	openrequest.__set_BookingType(paramProxy->getCscOrderBookingType());
	openrequest.__set_ShipmentType(paramProxy->getCscOrderPickingType());
	request.__set_OpenRequest(openrequest);
		
	// add positions
	csc::order::OrderPositionRequestList poslist;

	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = cscOrderCustCollYit;
	csc::order::OrderPositionRequest requestPos;

	requestPos.__set_PosType(csc::types::PositionTypeEnum::TypeProductStocked);
	requestPos.__set_ArticleCode(yitPos.getString(properties::ARTICLE_CODE));
	requestPos.__set_QuantityOrdered(yitPos.getInt32(properties::QUANTITY));
	requestPos.__set_AcceptInterBranchDelivery(true);
	requestPos.__set_AcceptPartialDelivery(true);
	poslist.push_back(requestPos);
		
	request.__set_Positions(poslist);

	// close order
	csc::order::CloseOrderRequest closerequest;

	closerequest.__set_CloseMode(csc::order::OrderCloseMode::CLOSE_MODE_CLOSE);
	request.__set_CloseRequest(closerequest);

	client->placeNewOrder(response, request);
	
	orderNo = response.OpenOrder.OrderNo;

	client->closeSession(sessionResponse);
	transport->close();	

	//DZ: log the response from placeNewOrder
	LogPlaceOrderResponse(response);
}

void CscOrderDM::LogPlaceOrderResponse(csc::order::PlaceOrderResponse response)
{
	if (response.OpenOrder.RetValue.ErrorCode != csc::types::MessageCodeEnum::CMSG_OK)
	{
		std::stringstream ss;
		ss << "OpenOrder error for order no " << response.CloseOrder;
		ss << " BranchNo: " << response.OpenOrder.BranchNo << " CustomerNo: " << response.OpenOrder.CustomerNo;
		ss << " ErrorCode: " << response.OpenOrder.RetValue.ErrorCode << " ErrorMessage: " << response.OpenOrder.RetValue.ErrorMessage;
		BLOG_ERROR(m_Logger, ss.str());
	}

	if (response.CloseOrder.RetValue.ErrorCode != csc::types::MessageCodeEnum::CMSG_OK)
	{
		std::stringstream ss;
		ss << "CloseOrder error for order no " << response.CloseOrder.OrderNo;
		ss << " BranchNo: " << response.CloseOrder.BranchNo << " CustomerNo: " << response.CloseOrder.CustomerNo;
		ss << " ErrorCode: " << response.CloseOrder.RetValue.ErrorCode << " ErrorMessage: " << response.CloseOrder.RetValue.ErrorMessage;
		BLOG_ERROR(m_Logger, ss.str());
	}

	csc::order::OrderPositionResponseList::const_iterator it = response.Positions.begin();

	for (; it != response.Positions.end(); ++it)
	{
		if (it->RetValue.ErrorCode != csc::types::MessageCodeEnum::CMSG_OK)
		{
			std::stringstream ss;
			ss << "Position error for order no " << response.OpenOrder.OrderNo;
			ss << " BranchNo: " << response.CloseOrder.BranchNo << " CustomerNo: " << response.CloseOrder.CustomerNo;
			ss << " ArticleCode: " << it->ArticleDelivered.ArticleCode;
			ss << " QtyOrdered: " << it->QuantityOrdered << " QtyLack: " << it->QuantityLack << " QtyIBT: " << it->QuantityInterBranch;
			ss << " ErrorCode: " << it->RetValue.ErrorCode << " ErrorMessage: " << it->RetValue.ErrorMessage;
			BLOG_ERROR(m_Logger, ss.str());
		}
	}
}

//-----------------------------------------------------------------------------------------

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

//-----------------------------------------------------------------------------------------
