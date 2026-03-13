#ifndef GUARD_DOMMOD_CSCORDER_CSCORDERDM_H
#define GUARD_DOMMOD_CSCORDER_CSCORDERDM_H

//-----------------------------------------------------------------------------------------

#include "icscorder.h"
#include "infrastructure/cscclient/icscclientconnectionptr.h"
#include "icscorderpositionptr.h"
#include <libbasarcmnutil_date.h>
#include <libbasarcmnutil_datetime.h>
#include <libbasarcmnutil_decimal.h>
#include <list>
#include <domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>

//-----------------------------------------------------------------------------------------

namespace log4cplus {
	class Logger;
}

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace cscorder {

//-----------------------------------------------------------------------------------------

class CscOrderDM : public ICscOrder
{
public:
	CscOrderDM(const log4cplus::Logger& logger);
	~CscOrderDM();

	void injectCscClientConnection(infrastructure::cscclient::ICscClientConnectionPtr);

	// order methods
	virtual basar::Int32 openOrderVz(const basar::Int16 branchNo, const basar::Int32 customerNo, const basar::VarString& orderType, const basar::VarString& subOrderType = basar::VarString());
	virtual basar::Int32 closeOrder();
    virtual basar::Int32 cancelOrder();
	virtual basar::Int32 setOrderType(
		const basar::VarString& orderType,
		const basar::VarString& pickingType = basar::VarString("0"),
		const basar::VarString& bookingType = basar::VarString("0"));
	virtual basar::Int32 setDeliveryDate(const basar::Date deliveryDate);
	virtual basar::Int32 setTenderNo(const basar::Int32 tenderNo);
	virtual basar::Int32 setOrderText(const basar::VarString& orderText);

	virtual basar::Int32 openOrderReferenceVzType( const basar::Int16 branchNo, const basar::Int32 customerNo,
												   const basar::VarString & orderType,
												   const basar::VarString & pickingType, const basar::VarString & bookingType,
												   const basar::VarString & reference = basar::VarString(),
												   const basar::VarString & subOrderType = basar::VarString() );

	// order item methods
	virtual ICscOrderPositionPtr createEmptyPosition(const basar::VarString& articleCode, const basar::Int32 qty);
	virtual ICscOrderPositionPtr addOrderItem(const basar::VarString& articleCode, const basar::Int32 qty);

 	virtual void save(bool importing = false);

	virtual basar::Int32 getOrderNo();
	virtual basar::Date getDeliveryDate();
	virtual basar::Date getTourDate();
	virtual basar::Int32 getLackReason();
	virtual basar::VarString getText();
	virtual basar::Int32 getCustomerNo();

	virtual csc::order::PlaceOrderResponse createCscOrder(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery);
	virtual void createCscOrder(basar::db::aspect::AccessorPropertyTable_YIterator cscOrderCustCollYit, basar::Int32 customerNo, basar::Int16 branchNo, basar::VarString orderType, basar::Int32& orderNo);

private:
	CscOrderDM( const CscOrderDM & );
	CscOrderDM & operator = ( const CscOrderDM & );

	const log4cplus::Logger& getLogger() const;
	void init();

	void LogPlaceOrderResponse(csc::order::PlaceOrderResponse);

	const log4cplus::Logger&								m_Logger;
	infrastructure::cscclient::ICscClientConnectionPtr		m_Connection;

	// wird von aussen gesetzt
	basar::Int16											m_BranchNo;
	basar::Int32											m_CustomerNo;

	// wird von openorder gesetzt
	basar::Int32											m_OrderNo;
	basar::VarString										m_PharmacyName;

	basar::Date												m_DeliveryDate;
	basar::Time												m_DeliveryTime;
	basar::VarString										m_TourId;
	basar::Date												m_TourDate;

	basar::Decimal											m_ValueOfOrder;
	basar::Int32											m_LinesOfOrder;

	basar::Int32											m_LackReason;
	basar::VarString										m_Text;
	basar::VarString										m_OrderType;

	std::list <ICscOrderPositionPtr>						m_OrderPositions;
};

//-----------------------------------------------------------------------------------------

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

//-----------------------------------------------------------------------------------------

#endif // GUARD_DOMMOD_CSCORDER_CSCORDERDM_H

//-----------------------------------------------------------------------------------------

