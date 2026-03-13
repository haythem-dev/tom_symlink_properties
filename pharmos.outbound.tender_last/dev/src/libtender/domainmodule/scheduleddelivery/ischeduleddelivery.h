#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERY_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERY_H

#include "ischeduleddeliveryptr.h"
#include "ischeduleddeliveryheaderptr.h"
#include "ischeduleddeliverypositioncollectionptr.h"

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
			class IScheduledDelivery
			{
				public:
					virtual ~IScheduledDelivery() {}

					virtual IScheduledDeliveryHeaderPtr             getHeader() = 0;
					virtual IScheduledDeliveryPositionCollectionPtr getPositionCollection() = 0;

					virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition() = 0;
					virtual void deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit ) = 0;

					virtual void findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
					virtual void save() = 0;

					virtual bool isDeliveryExisting() = 0;
					virtual bool isPurchaseOrderProposalExisting() = 0;
			};

		} //end namespace scheduledDelivery
	}     //end namespace scheduledDelivery
}         //end namespace scheduledDelivery

#endif //GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERY_H
