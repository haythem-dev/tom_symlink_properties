#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYCOLLECTIONDM_H

//-----------------------------------------------------------------------------------------

#include "ischeduleddeliverycollectiondm.h"
#include <componentmanager/ischeduleddeliveryconnectedfactoryptr.h>

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace scheduledDelivery {

//-----------------------------------------------------------------------------------------

class ScheduledDeliveryCollectionDM : public IScheduledDeliveryCollectionDM
{
public:
	ScheduledDeliveryCollectionDM();
	~ScheduledDeliveryCollectionDM();

	void injectScheduledDeliveryAccessor( libutil::infrastructure::accessor::IAccessorPtr );
	void                                                       injectScheduledDeliveryFactory ( libtender::componentManager::IScheduledDeliveryConnectedFactoryPtr );

	virtual void                                               findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual bool											   isExisting( basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual void											   getQuantity( basar::db::aspect::AccessorPropertyTable_YIterator );

	virtual basar::db::aspect::AccessorPropertyTableRef        get() const;
	virtual void                                               resetSearchYit();
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit() const;
	virtual IScheduledDeliveryPtr                              getScheduledDelivery( const basar::db::aspect::AccessorPropertyTable_YIterator ) const;
	virtual bool                                               isEmpty() const;
	
private:
	ScheduledDeliveryCollectionDM( const ScheduledDeliveryCollectionDM& r );
	ScheduledDeliveryCollectionDM& operator= ( const ScheduledDeliveryCollectionDM& r );

	libutil::domMod::SearchYIteratorPtr             getSearchYIterator() const;
	libutil::infrastructure::accessor::IAccessorPtr getScheduledDeliveryAccessor() const;
	libtender::componentManager::IScheduledDeliveryConnectedFactoryPtr     getScheduledDeliveryConnectedFactory() const;


	const log4cplus::Logger & getLogger() const;

	const log4cplus::Logger 						m_Logger;
	mutable libutil::domMod::SearchYIteratorPtr     m_SearchYIterator;
	libutil::infrastructure::accessor::IAccessorPtr m_Accessor;
	libtender::componentManager::IScheduledDeliveryConnectedFactoryPtr m_ScheduledDeliveryConnectedFactory;
};

//-----------------------------------------------------------------------------------------

} //end namespace scheduledDelivery
} //end namespace domMod
} //end namespace libtender

//-----------------------------------------------------------------------------------------

#endif //end GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYCOLLECTIONDM_H

//-----------------------------------------------------------------------------------------
