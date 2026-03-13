#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULED_DELIVERY_IMPORT_COLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULED_DELIVERY_IMPORT_COLLECTION_H

#include <domainmodule/scheduleddelivery/ischeduleddeliveryimportcollection.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <domainmodule/tender/itenderimportcollectionptr.h>
#include <componentmanager/ischeduleddeliveryconnectedfactoryptr.h>
#include <map>
//#include <libbasarcmnutil_i18nstring.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libutil/dbtransaction.h>

namespace log4cplus
{
class Logger;
}

namespace libtender
{
namespace domMod
{
namespace scheduledDelivery
{

struct dateandcustomer
{
public :
	basar::Int32 datum;
	basar::Int32 customer;

	bool operator==(const dateandcustomer &o)  const 
	{
        return datum == o.datum && customer == o.customer;
    }

    bool operator<(const dateandcustomer &o)  const 
	{
        return datum < o.datum || (datum >> o.datum && customer < o.customer);
    }

	bool operator>(const dateandcustomer &o)  const 
	{
        return datum > o.datum || (datum == o.datum && customer > o.customer);
    }
};

class ScheduledDeliveryImportCollection : public IScheduledDeliveryImportCollection
{
public:
	ScheduledDeliveryImportCollection( const log4cplus::Logger logger );
	virtual ~ScheduledDeliveryImportCollection();

	void injectScheduledDeliveryFactory( componentManager::IScheduledDeliveryConnectedFactoryPtr scheduledDeliveryFactory );
	void injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory );
	void injectTenderImportCollection( domMod::tender::ITenderImportCollectionPtr tenderImportCollection );

	virtual basar::Int32 getScheduledDeliveryCount() const;
	virtual basar::Int32 getScheduledDeliveryID( const ScheduledDeliveryImportStruct & values ) const;
	virtual void addPosition( const ScheduledDeliveryImportStruct & values );
	virtual void save();
	virtual void clear();
	
private:
	ScheduledDeliveryImportCollection( const ScheduledDeliveryImportCollection & );
	ScheduledDeliveryImportCollection & operator=( const ScheduledDeliveryImportCollection & );

	typedef std::map<dateandcustomer, IScheduledDeliveryPtr> InnerScheduledDeliveryCollection;		// date/customer -> items
	typedef std::map<basar::Int32, InnerScheduledDeliveryCollection> InnerCollection;			// id -> collection

	InnerScheduledDeliveryCollection & getScheduledDeliveryCollection( const ScheduledDeliveryImportStruct & values );
	IScheduledDeliveryPtr getScheduledDelivery( const ScheduledDeliveryImportStruct & values );
	void matchToHeader( basar::db::aspect::AccessorPropertyTable_YIterator yitHead, const ScheduledDeliveryImportStruct & values );
	void matchToPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitPos, const ScheduledDeliveryImportStruct & values );

	const log4cplus::Logger													m_Logger;
	InnerCollection															m_ScheduledDeliveries;
	componentManager::IScheduledDeliveryConnectedFactoryPtr					m_ScheduledDeliveryFactory;
	libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	m_TransactionFactory;
	domMod::tender::ITenderImportCollectionPtr								m_TenderImportCollection;
};

} // end namespace scheduledDelivery
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULED_DELIVERY_IMPORT_COLLECTION_H
