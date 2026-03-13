#include "scheduleddeliveryconnectedfactory.h"

#include <domainmodule/scheduleddelivery/scheduleddeliverydm.h>

namespace libtender
{
    namespace componentManager
    {
        ScheduledDeliveryConnectedFactory::ScheduledDeliveryConnectedFactory()
        {
        }

        ScheduledDeliveryConnectedFactory::~ScheduledDeliveryConnectedFactory()
        {
        }
        
		domMod::scheduledDelivery::IScheduledDeliveryPtr ScheduledDeliveryConnectedFactory::create( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch ) const
        {
			domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery = ScheduledDeliveryFactoryBase::create();

            try
            {
				if(!yitSearch.isNull() && !yitSearch.isEnd())
				{
					scheduledDelivery->findByKey(yitSearch);
				}
            }
            catch( basar::Exception & /*e*/  )
            {
				// TODO How to create empty Delivery?
                // e;
                //throw;
            }
            
            return scheduledDelivery;
        }
    }
}
