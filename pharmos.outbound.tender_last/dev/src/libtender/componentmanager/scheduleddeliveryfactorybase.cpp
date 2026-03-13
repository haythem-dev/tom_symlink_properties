#include "scheduleddeliveryfactorybase.h"

#include <domainmodule/scheduleddelivery/scheduleddeliveryheaderptr.h>
#include <domainmodule/scheduleddelivery/scheduleddeliveryheader.h>
#include <domainmodule/scheduleddelivery/scheduleddeliverypositioncollectionptr.h>
#include <domainmodule/scheduleddelivery/scheduleddeliverypositioncollection.h>
#include <domainmodule/scheduleddelivery/scheduleddeliverydmptr.h>
#include <domainmodule/scheduleddelivery/scheduleddeliverydm.h>

#include <libutil/session.h>

#include <infrastructure/accessor/scheduleddelivery/scheduleddeliveryheaderacc.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliveryheaderacc_definitions.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverypositionsacc.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverypositionsacc_definition.h>

namespace libtender
{
namespace componentManager
{
ScheduledDeliveryFactoryBase::ScheduledDeliveryFactoryBase()
{        
}

ScheduledDeliveryFactoryBase::~ScheduledDeliveryFactoryBase()
{       
}
        
void ScheduledDeliveryFactoryBase::injectDBConnection( basar::db::aspect::ConnectionRef dbConnection )
{
	m_DBConnection = dbConnection;
}

void ScheduledDeliveryFactoryBase::injectPurchaseDBParameterProxy( infrastructure::db::IPurchaseDBParameterProxyPtr proxy )
{
	m_PurchaseDBParameterProxy = proxy;
}

void ScheduledDeliveryFactoryBase::injectSession( libutil::infrastructure::session::ISessionPtr session )
{
	m_Session = session;
}

void ScheduledDeliveryFactoryBase::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
{
	m_TransactionFactory = transactionFactory;
}

domMod::scheduledDelivery::IScheduledDeliveryPtr ScheduledDeliveryFactoryBase::create() const
{
	domMod::scheduledDelivery::ScheduledDeliveryDMPtr scheduledDelivery( new domMod::scheduledDelivery::ScheduledDeliveryDM() );
	scheduledDelivery->injectHeader( getScheduledDeliveryHeader() );
	scheduledDelivery->injectPositionCollection( getScheduledDeliveryPositionCollection() );
	scheduledDelivery->injectTransactionFactory( getTransactionFactory() );

	return scheduledDelivery;
}
        
libtender::domMod::scheduledDelivery::IScheduledDeliveryHeaderPtr ScheduledDeliveryFactoryBase::getScheduledDeliveryHeader() const
{
	libtender::domMod::scheduledDelivery::ScheduledDeliveryHeaderPtr scheduledDeliveryHeader( new libtender::domMod::scheduledDelivery::ScheduledDeliveryHeader() );
	scheduledDeliveryHeader->injectScheduledDeliveryHeaderAccessor( getScheduledDeliveryHeaderAccessor() );
	return scheduledDeliveryHeader;
}

libtender::domMod::scheduledDelivery::IScheduledDeliveryPositionCollectionPtr ScheduledDeliveryFactoryBase::getScheduledDeliveryPositionCollection() const
{
	libtender::domMod::scheduledDelivery::ScheduledDeliveryPositionCollectionPtr scheduledDeliveryPositionCollection( new libtender::domMod::scheduledDelivery::ScheduledDeliveryPositionCollection() );
	scheduledDeliveryPositionCollection->injectScheduledDeliveryPositionCollectionAccessor( getScheduledDeliveryPositionCollectionAccessor() );
	scheduledDeliveryPositionCollection->injectPurchaseDBParameterProxy( getPurchaseDBParameterProxy() );
	return scheduledDeliveryPositionCollection;
}

libutil::infrastructure::accessor::IAccessorPtr ScheduledDeliveryFactoryBase::getScheduledDeliveryHeaderAccessor() const
{
	libtender::infrastructure::accessor::scheduledDelivery::SelectScheduledDeliveryHeaderByKey();
	libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr( 
		new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::scheduledDelivery::lit::SCHEDULEDDELIVERY_HEADER_ACC, getDBConnection() ) 
			);
	return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ScheduledDeliveryFactoryBase::getScheduledDeliveryPositionCollectionAccessor() const
{
	libtender::infrastructure::accessor::scheduledDelivery::SelectScheduledDeliveryPositionsByKey();  // pseudo reference
	libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr( 
		new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::scheduledDelivery::lit::SCHEDULEDDELIVERY_POSITIONS_ACC, getDBConnection(), basar::db::aspect::FULL_CACHING ) 
			);
	return accessor;
}

basar::db::aspect::ConnectionRef ScheduledDeliveryFactoryBase::getDBConnection() const
{
	return m_DBConnection;
}

infrastructure::db::IPurchaseDBParameterProxyPtr ScheduledDeliveryFactoryBase::getPurchaseDBParameterProxy() const
{
	return m_PurchaseDBParameterProxy;
}

libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr ScheduledDeliveryFactoryBase::getTransactionFactory() const
{
	return m_TransactionFactory;
}

} // end namespace componentManager
} // end namespace libtender


