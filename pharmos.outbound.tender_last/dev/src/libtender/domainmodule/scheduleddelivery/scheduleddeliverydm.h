#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYDM_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYDM_H

#include <libutil/searchyiterator.h>
#include <libutil/dbtransaction.h>
#include <libutil/classinfo.h>
#include <libutil/util.h>

#include "ischeduleddelivery.h"
#include "ischeduleddeliveryptr.h"
#include "ischeduleddeliveryheaderptr.h"
#include "ischeduleddeliverypositioncollectionptr.h"

namespace libtender
{
namespace domMod
{
namespace scheduledDelivery
{
class ScheduledDeliveryDM : public IScheduledDelivery
{
public:
	ScheduledDeliveryDM();
	~ScheduledDeliveryDM();

	void injectHeader( IScheduledDeliveryHeaderPtr );
	void injectPositionCollection( IScheduledDeliveryPositionCollectionPtr );
	void injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr );

	virtual IScheduledDeliveryHeaderPtr                    getHeader();
	virtual IScheduledDeliveryPositionCollectionPtr        getPositionCollection();
					
	virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition();
	virtual void deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	virtual void findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual void save();

	virtual bool isDeliveryExisting();
	virtual bool isPurchaseOrderProposalExisting();

private:
	ScheduledDeliveryDM( const ScheduledDeliveryDM & );
	ScheduledDeliveryDM & operator = ( const ScheduledDeliveryDM & );

	libutil::domMod::SearchYIteratorPtr	getSearchYIterator();
	const log4cplus::Logger &			getLogger() const;

	const log4cplus::Logger 												m_Logger;
	libutil::domMod::SearchYIteratorPtr										m_SearchYIterator;
	IScheduledDeliveryHeaderPtr												m_Header;
	IScheduledDeliveryPositionCollectionPtr									m_PositionCollection;
	libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	m_TransactionFactory;
};

} //end namespace scheduledDelivery
}     //end namespace domMod
}         //end namespace libtender

#endif //GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYDM_H
