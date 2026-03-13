#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULED_DELIVERY_IMPORT_STRUCT_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULED_DELIVERY_IMPORT_STRUCT_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_i18nstring.h>
#include <libbasarcmnutil_date.h>
#include <libbasarcmnutil_decimal.h>

namespace libtender
{
namespace domMod
{
namespace scheduledDelivery
{
struct ScheduledDeliveryImportStruct
{
	basar::Int32		id;
	basar::Int16		branchno;
	basar::Int32		customerno;
	basar::Date			deliverydate;
	basar::VarString	articlecode;
	basar::Int32		quantity;
	basar::Int16		purchaseorderproposalflag;
	basar::Int16		branchnopop;
	

	// TODO: notwendig?
	// get's determined
	//basar::Int32		articleno;	
};

} // end namespace scheduledDelivery
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULED_DELIVERY_IMPORT_STRUCT_H
