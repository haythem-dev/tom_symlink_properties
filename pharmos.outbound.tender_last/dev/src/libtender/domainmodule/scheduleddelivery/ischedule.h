#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULE_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULE_H

#include <libbasar_definitions.h>

#include "ischeduleddeliveryptr.h"
#include "ischeduleddeliverycollectiondmptr.h"


namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libtender
{
	namespace domMod
	{
		namespace scheduledDelivery
		{
			class ISchedule
			{
				public:
					virtual ~ISchedule() {};

					virtual libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr getScheduledDeliveryCollection() = 0;
					virtual void injectScheduledDeliveryCollection( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection ) = 0;
			};

		} //end namespace scheduledDelivery
	}     //end namespace domMod
}         //end namespace libtender

#endif //GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULE_H
