#include "schedule.h"
#include <domainmodule/scheduleddelivery/ischeduleddeliverycollectiondm.h>
#include <loggerpool/tenderloggerpool.h>
#include <domainmodule/libtender_properties_definitions.h>

#include <libbasardbaspect_exceptions.h>
#include <libutil/exception.h>
#include <libutil/validator.h>

namespace libtender
{
	namespace domMod
	{
		namespace tender
		{
			using basar::db::aspect::AccessorPropertyTable_YIterator;
		}
	}
}

namespace libtender
{
	namespace domMod
	{
		namespace scheduledDelivery
		{
			const libutil::misc::ClassInfo& Schedule::getClassInfo()
			{
				static const libutil::misc::ClassInfo info( "Schedule", libutil::misc::Version( 1, 0, 0, 0 ) );
				return info;
			}
			
			Schedule::Schedule()
			: m_Logger( libtender::LoggerPool::getLoggerDomModules() )
			{
			}
			
			Schedule::~Schedule()
			{
			}

			libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr Schedule::getScheduledDeliveryCollection()
			{
				METHODNAME_DEF( Schedule, getScheduledDeliveryCollection )
                BLOG_TRACE_METHOD( getLogger(), fun );

				return m_ScheduledDeliveryCollection;
			}

			void Schedule::injectScheduledDeliveryCollection( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection )
			{
				METHODNAME_DEF( Schedule, injectScheduledDeliveryCollection )
                BLOG_TRACE_METHOD( getLogger(), fun );

				if( NULL != scheduledDeliveryCollection.get() )
				{
					m_ScheduledDeliveryCollection = scheduledDeliveryCollection;
				}
			}

			const log4cplus::Logger& Schedule::getLogger() const
			{
				return m_Logger;
			}

		} //end namespace scheduledDelivery
	}     //end namespace domMod
}         //end namespace libtender
