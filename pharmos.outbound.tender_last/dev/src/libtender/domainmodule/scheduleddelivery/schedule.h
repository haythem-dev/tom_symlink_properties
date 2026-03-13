#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULE_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULE_H

#include "ischedule.h"
#include "ischeduleddeliverycollectiondmptr.h"

#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorInstanceRef;
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
			class Schedule : public ISchedule
			{
				public:
					static const libutil::misc::ClassInfo& getClassInfo();

				public:
					Schedule();
					~Schedule();

					virtual libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr getScheduledDeliveryCollection();
					virtual void injectScheduledDeliveryCollection( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection );

				private:
					Schedule( const Schedule& r );
					Schedule& operator = ( const Schedule& r );


					const log4cplus::Logger & getLogger() const;

					const log4cplus::Logger 												m_Logger;
					libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr m_ScheduledDeliveryCollection;
			};

		} // end namespace scheduledDelivery
	}     // end namespace domMod
}         // end namespace libtender

#endif //GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULE_H
