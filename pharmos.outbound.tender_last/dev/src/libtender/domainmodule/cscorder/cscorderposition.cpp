#include "cscorderposition.h"
#include "cscclienthelper.h"

#include <libutil/misc/metaclassmacro_definitions.h> // libutil
#include <libutil/misc/log_macro_definitions.h>      // libutil
#include <loggerpool/tenderloggerpool.h>

#include <kscclient/articleinfostruct.h>
#include <kscclient/articleinfostructcode.h>
#include <kscclient/orderinfo.h>
//#include <callbackinquiry.h>
//#include <newcallbackcritpos.h>
//#include <newcallback.h>
//#include <callbackreasoninquiry.h>
//#include <callbackdivisioninquiry.h>
#include <kscclient/callksc.h>

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace cscorder {

//-----------------------------------------------------------------------------------------

CscOrderPosition::CscOrderPosition(const basar::VarString& articleCode, const basar::Int32 qty, const log4cplus::Logger& logger) :
	m_OrderInfo(),
	m_Logger(logger),
	m_ArticleCode(articleCode),
	m_OrderQuantity(qty),
	m_IbtPossible(true), /* auxiliary */
	m_RestDelPossible(true), /* restdelivery */
	m_DispoPossible(true), /* dispose */
	m_PartDelPossible(true) /* part delivery */
{
}

//-----------------------------------------------------------------------------------------

CscOrderPosition::CscOrderPosition(const orderinfoS& info, const log4cplus::Logger& logger) :
	m_OrderInfo(info),
	m_Logger(logger)
{
}

//-----------------------------------------------------------------------------------------

CscOrderPosition::~CscOrderPosition()
{
}

//-----------------------------------------------------------------------------------------

const log4cplus::Logger& CscOrderPosition::getLogger() const
{
	return m_Logger;
}

//-----------------------------------------------------------------------------------------

void CscOrderPosition::setIbtPossible(bool p)				{ m_IbtPossible = p; }
void CscOrderPosition::setRestDeliveryPossible(bool p)		{ m_RestDelPossible = p; }
void CscOrderPosition::setDispoPossible(bool p)				{ m_DispoPossible = p; }
void CscOrderPosition::setPartialDeliveryPossible(bool p)	{ m_PartDelPossible = p; }

//-----------------------------------------------------------------------------------------

basar::Int32		CscOrderPosition::getOrderQuantity()	{ return m_OrderQuantity; }
basar::VarString	CscOrderPosition::getArticleCode()		{ return m_ArticleCode; }

//-----------------------------------------------------------------------------------------

basar::Int32 CscOrderPosition::getDeliveryQuantity()
{
	return m_OrderInfo.DELIVERYQTY;
}

//-----------------------------------------------------------------------------------------

basar::Int32 CscOrderPosition::getIBTQuantity()
{
	return m_OrderInfo.AUXQTY;
}

//-----------------------------------------------------------------------------------------

basar::Int32 CscOrderPosition::getLackReason()
{
	return m_OrderInfo.LACKREASON;
}

//-----------------------------------------------------------------------------------------

basar::VarString CscOrderPosition::getText()
{
	return CscClientHelper::getStringFromStruct(m_OrderInfo.TEXT, 240);
}

//-----------------------------------------------------------------------------------------

void CscOrderPosition::save(bool importing)
{
    (void)importing;

	memset(&m_OrderInfo, 0, sizeof(orderinfoS));

	std::stringstream ss;
	ss <<  "addOrderItem(\"" << m_ArticleCode << "\", " << m_OrderQuantity << ")";
	BLOG_DEBUG(getLogger(), ss.str());

	::AddOrderItemCode(
		m_OrderQuantity, 
		const_cast<char*>(m_ArticleCode.c_str()),
		m_IbtPossible, /* auxiliary */
		m_RestDelPossible, /* restdelivery */
		m_DispoPossible, /* dispose */
		m_PartDelPossible, /* part delivery */
		0, /* internal OrderNo */
		0, /* interne IDF-Nummer */
		0, /* Ursprungs Auftragsnummer */
		0, /* Ursprungs Positionsnummer */
		0, /* Preistyp */
		const_cast<char*>(""),  /* Bermerkungstext */
		&m_OrderInfo);
}

//-----------------------------------------------------------------------------------------

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

//-----------------------------------------------------------------------------------------
