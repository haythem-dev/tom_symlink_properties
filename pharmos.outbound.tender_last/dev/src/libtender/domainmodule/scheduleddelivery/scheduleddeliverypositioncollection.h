#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYPOSITIONCOLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYPOSITIONCOLLECTION_H

//-----------------------------------------------------------------------------------------

#include "ischeduleddeliverypositioncollection.h"

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>

#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverypositionsacc.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverypositionsacc_definition.h>
#include <infrastructure/purchasedbconn/ipurchasedbparameterproxyptr.h>

//-----------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace scheduledDelivery {

//-----------------------------------------------------------------------------------------

class ScheduledDeliveryPositionCollection : public IScheduledDeliveryPositionCollection
{
public:
	ScheduledDeliveryPositionCollection();
	~ScheduledDeliveryPositionCollection();

	void injectScheduledDeliveryPositionCollectionAccessor( libutil::infrastructure::accessor::IAccessorPtr );
	void injectPurchaseDBParameterProxy( infrastructure::db::IPurchaseDBParameterProxyPtr );

	virtual void findByKey( basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual basar::Int32 getAlreadyReservedQuantity( const basar::db::aspect::AccessorPropertyTable_YIterator );

	virtual basar::db::aspect::AccessorPropertyTableRef       get();
	virtual const basar::db::aspect::AccessorPropertyTableRef get() const;

	virtual void                                                resetSearchYit();
	virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;

	virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition();
	virtual void deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	virtual void setForeignKey( const basar::Int32 );
	virtual void save();
	virtual bool isEmpty() const;
private:
	ScheduledDeliveryPositionCollection( const ScheduledDeliveryPositionCollection& r );
	ScheduledDeliveryPositionCollection& operator = ( const ScheduledDeliveryPositionCollection& r );

	libutil::domMod::SearchYIteratorPtr getSearchYIterator() const;
	void saveDeletePositions();
	void saveExistingPositions();

	void completeForeignKey( basar::db::aspect::AccessorPropertyTable_YIterator );
	bool isForeignKeySet   ( const basar::db::aspect::AccessorPropertyTable_YIterator yitPos ) const;

	bool isSavable() const;
	void checkNumberPositions() const;

	libutil::infrastructure::accessor::IAccessorPtr		getAccessor() const;
	const log4cplus::Logger&							getLogger() const;

	const log4cplus::Logger								m_Logger;
	libutil::infrastructure::accessor::IAccessorPtr		m_Accessor;
	mutable libutil::domMod::SearchYIteratorPtr			m_SearchYIterator;
	infrastructure::db::IPurchaseDBParameterProxyPtr	m_PurchaseDBParameterProxy;

};

//-----------------------------------------------------------------------------------------

} //end namespace scheduledDelivery
} //end namespace domMod
} //end namespace libtender

//-----------------------------------------------------------------------------------------

#endif //GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYPOSITIONCOLLECTION_H

//-----------------------------------------------------------------------------------------
