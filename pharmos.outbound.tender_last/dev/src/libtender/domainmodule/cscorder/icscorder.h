#ifndef GUARD_DOMMOD_CSCORDER_ICSCORDER_H
#define GUARD_DOMMOD_CSCORDER_ICSCORDER_H

//-----------------------------------------------------------------------------------------

#include "icscorderpositionptr.h"
#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libbasardbaspect.h>

#ifdef WIN32
#pragma warning (push)
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4127) // conditional expression is constant
#pragma warning (disable : 4505) // unreferenced local function has been removed
#pragma warning (disable : 4706)
#endif
#include <gen-cpp/CscServiceBatch.h>
#include <gen-cpp/cscbase_constants.h>
#ifdef WIN32
#pragma warning (pop)
#endif

using namespace csc::base;
using namespace csc::service;

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace cscorder {

class ICscOrder
{
public:
	virtual ~ICscOrder() {}

	virtual basar::Int32 openOrderVz(const basar::Int16 branchNo, const basar::Int32 customerno, const basar::VarString& orderType, const basar::VarString& subOrderType = basar::VarString()) = 0;
	virtual basar::Int32 closeOrder() = 0;
    virtual basar::Int32 cancelOrder() = 0;
	virtual basar::Int32 setOrderType(const basar::VarString& orderType, const basar::VarString& pickingType, const basar::VarString& bookingType) = 0;
	virtual basar::Int32 setDeliveryDate(const basar::Date deliveryDate) = 0;
	virtual basar::Int32 setTenderNo(const basar::Int32 tenderNo) = 0;
	virtual basar::Int32 setOrderText(const basar::VarString& orderText) = 0;

	virtual basar::Int32 openOrderReferenceVzType( const basar::Int16 branchNo, const basar::Int32 customerNo,
												   const basar::VarString & orderType,
												   const basar::VarString & pickingType, const basar::VarString & bookingType,
												   const basar::VarString & reference = basar::VarString(),
												   const basar::VarString & subOrderType = basar::VarString() ) = 0;

	virtual ICscOrderPositionPtr createEmptyPosition(const basar::VarString& articleCode, const basar::Int32 qty) = 0;
	virtual ICscOrderPositionPtr addOrderItem(const basar::VarString& articleCode, const basar::Int32 qty) = 0;

	virtual basar::Int32 getOrderNo() = 0;
	virtual basar::Date getDeliveryDate() = 0;
	virtual basar::Date getTourDate() = 0;
	virtual basar::Int32 getLackReason() = 0;
	virtual basar::VarString getText() = 0;
	virtual basar::Int32 getCustomerNo() = 0;

	virtual csc::order::PlaceOrderResponse createCscOrder(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery) = 0;
	virtual void createCscOrder(basar::db::aspect::AccessorPropertyTable_YIterator cscOrderCustCollYit, basar::Int32 customerNo, basar::Int16 branchNo, basar::VarString orderType,  basar::Int32& orderNo) = 0;
};

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

#endif
