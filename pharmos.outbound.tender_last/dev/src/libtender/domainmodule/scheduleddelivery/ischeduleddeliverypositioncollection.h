#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPOSITIONCOLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPOSITIONCOLLECTION_H

#include <libbasar_definitions.h>

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
			class IScheduledDeliveryPositionCollection
			{
				public:
					virtual ~IScheduledDeliveryPositionCollection() {}
				
					virtual void findByKey( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
					virtual basar::Int32 getAlreadyReservedQuantity( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

                    virtual void                                                resetSearchYit()     = 0;
                    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const = 0;

					virtual basar::db::aspect::AccessorPropertyTableRef       get() = 0;
					virtual const basar::db::aspect::AccessorPropertyTableRef get() const = 0;
				
					virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition() = 0;
					virtual void deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit ) = 0;

					virtual void setForeignKey( const basar::Int32 ) = 0;
					virtual void save() = 0;
					virtual bool isEmpty() const = 0;
			};
		
		} // end namespace scheduledDelivery
	}	  // end namespace domMod
}		  // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPOSITIONCOLLECTION_H
