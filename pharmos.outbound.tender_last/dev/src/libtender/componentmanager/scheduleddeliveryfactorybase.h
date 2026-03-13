#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYFACTORYBASE_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYFACTORYBASE_H

#include <domainmodule/scheduleddelivery/ischeduleddeliveryheaderptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverypositioncollectionptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <infrastructure/purchasedbconn/ipurchasedbparameterproxyptr.h>
#include <libutil/accessor.h>
#include <libutil/dbconnection.h>
#include <libutil/dbtransaction.h>
#include <libutil/infrastructure/session/isessionptr.h>

namespace libtender
{
namespace componentManager
{
class ScheduledDeliveryFactoryBase
{
public:
	virtual ~ScheduledDeliveryFactoryBase();
	void injectDBConnection( basar::db::aspect::ConnectionRef );
	void injectPurchaseDBParameterProxy( infrastructure::db::IPurchaseDBParameterProxyPtr );
	void injectSession( libutil::infrastructure::session::ISessionPtr session );
	void injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	transactionFactory );

protected:
	ScheduledDeliveryFactoryBase();
	domMod::scheduledDelivery::IScheduledDeliveryPtr create() const;

private:
	ScheduledDeliveryFactoryBase( const ScheduledDeliveryFactoryBase & );
	ScheduledDeliveryFactoryBase & operator=( const ScheduledDeliveryFactoryBase & );

	libtender::domMod::scheduledDelivery::IScheduledDeliveryHeaderPtr getScheduledDeliveryHeader() const;
	libtender::domMod::scheduledDelivery::IScheduledDeliveryPositionCollectionPtr getScheduledDeliveryPositionCollection() const;

	libutil::infrastructure::accessor::IAccessorPtr getScheduledDeliveryHeaderAccessor() const;
	libutil::infrastructure::accessor::IAccessorPtr getScheduledDeliveryPositionCollectionAccessor() const;
	basar::db::aspect::ConnectionRef getDBConnection() const;
	infrastructure::db::IPurchaseDBParameterProxyPtr getPurchaseDBParameterProxy() const;
	libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr getTransactionFactory() const;

private:
	basar::db::aspect::ConnectionRef					m_DBConnection;
	infrastructure::db::IPurchaseDBParameterProxyPtr	m_PurchaseDBParameterProxy;
	libutil::infrastructure::session::ISessionPtr		m_Session;
	libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	m_TransactionFactory;
};

}
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYFACTORYBASE_H
