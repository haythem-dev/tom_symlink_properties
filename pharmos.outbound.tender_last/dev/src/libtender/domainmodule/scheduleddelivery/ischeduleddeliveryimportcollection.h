#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_I_SCHEDULED_DELIVERY_IMPORT_COLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_I_SCHEDULED_DELIVERY_IMPORT_COLLECTION_H

#include "scheduleddeliveryimportstruct.h"

namespace libtender
{
namespace domMod
{
namespace scheduledDelivery
{
class IScheduledDeliveryImportCollection
{
public:
	virtual ~IScheduledDeliveryImportCollection() {}

	virtual basar::Int32 getScheduledDeliveryCount() const = 0;
	virtual basar::Int32 getScheduledDeliveryID( const ScheduledDeliveryImportStruct & values ) const = 0;
	virtual void addPosition( const libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct & values ) = 0;
	virtual void save() = 0;
	virtual void clear() = 0;
};

} // end namespace scheduledDelivery
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_I_SCHEDULED_DELIVERY_IMPORT_COLLECTION_H
