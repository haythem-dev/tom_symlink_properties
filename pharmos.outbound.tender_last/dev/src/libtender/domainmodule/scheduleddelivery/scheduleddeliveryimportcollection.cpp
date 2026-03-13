#include "scheduleddeliveryimportcollection.h"
#include "ischeduleddelivery.h"
#include "ischeduleddeliveryheader.h"
#include "scheduleddeliveryimportstruct.h"
#include <domainmodule/libtender_properties_definitions.h>
#include <componentmanager/ischeduleddeliveryconnectedfactory.h>
#include <domainmodule/tender/itenderimportcollection.h>


namespace libtender
{
namespace domMod
{
namespace scheduledDelivery
{
using namespace properties;

ScheduledDeliveryImportCollection::ScheduledDeliveryImportCollection( const log4cplus::Logger logger )
: m_Logger( logger )
{
	
}

ScheduledDeliveryImportCollection::~ScheduledDeliveryImportCollection()
{
}

void ScheduledDeliveryImportCollection::injectScheduledDeliveryFactory( componentManager::IScheduledDeliveryConnectedFactoryPtr scheduledDeliveryFactory )
{
	m_ScheduledDeliveryFactory = scheduledDeliveryFactory;
}

void ScheduledDeliveryImportCollection::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
{
	m_TransactionFactory = transactionFactory;
}

void ScheduledDeliveryImportCollection::injectTenderImportCollection( domMod::tender::ITenderImportCollectionPtr tenderImportCollection )
{
	m_TenderImportCollection = tenderImportCollection;
}

basar::Int32 ScheduledDeliveryImportCollection::getScheduledDeliveryCount() const
{
	return static_cast<basar::Int32>(m_ScheduledDeliveries.size());
}

void ScheduledDeliveryImportCollection::addPosition( const ScheduledDeliveryImportStruct & values )
{
	IScheduledDeliveryPtr scheduledDelivery = getScheduledDelivery( values );
	matchToPosition( scheduledDelivery->addEmptyPosition(), values );
}

void ScheduledDeliveryImportCollection::save()
{
	libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();

	try
	{
		transaction->begin();
		InnerCollection::iterator iter = m_ScheduledDeliveries.begin();
		for( ; iter != m_ScheduledDeliveries.end(); ++iter )
		{
			basar::Int32 tenderID = m_TenderImportCollection->getTenderID( iter->first );
			InnerScheduledDeliveryCollection & coll = iter->second;
			InnerScheduledDeliveryCollection::iterator collIter = coll.begin();
			for( ; collIter != coll.end(); ++collIter )
			{
				IScheduledDeliveryPtr scheduledDelivery = collIter->second;
				scheduledDelivery->getHeader()->get().setInt32( TENDER_ID, tenderID );
				scheduledDelivery->save();
			}
		}
		transaction->commit();
	}
	catch( const basar::Exception & e  )
	{
		transaction->rollback();
		std::stringstream ss;
		ss << "ScheduledDeliveryImportCollection::save(): " << e.what().c_str();
		BLOG_ERROR( m_Logger, ss.str().c_str() );
		throw;
	}
}

void ScheduledDeliveryImportCollection::clear()
{
	m_ScheduledDeliveries.clear();
}

basar::Int32 ScheduledDeliveryImportCollection::getScheduledDeliveryID( const ScheduledDeliveryImportStruct & values ) const
{
	InnerCollection::const_iterator iter = m_ScheduledDeliveries.find( values.id );
	if( iter == m_ScheduledDeliveries.end() )
	{
		return 0;
	}

	dateandcustomer dnc; 
	dnc.datum = values.deliverydate.getDate();
	dnc.customer = values.customerno;
    
	const InnerScheduledDeliveryCollection & innerScheduledDeliveryCollection = iter->second;
	
	InnerScheduledDeliveryCollection::const_iterator collIter = innerScheduledDeliveryCollection.find( dnc );
	if( collIter == innerScheduledDeliveryCollection.end() )
	{
		return 0;
	}

	IScheduledDeliveryPtr schedule = collIter->second;
	return schedule->getHeader()->getScheduledDeliveryId();
}

ScheduledDeliveryImportCollection::InnerScheduledDeliveryCollection & ScheduledDeliveryImportCollection::getScheduledDeliveryCollection( const ScheduledDeliveryImportStruct & values )
{
	InnerCollection::iterator iter = m_ScheduledDeliveries.find( values.id );
	if( iter == m_ScheduledDeliveries.end() )
	{
		m_ScheduledDeliveries[ values.id ] = InnerScheduledDeliveryCollection();
	}

	iter = m_ScheduledDeliveries.find( values.id );
	return iter->second;
}

IScheduledDeliveryPtr ScheduledDeliveryImportCollection::getScheduledDelivery( const ScheduledDeliveryImportStruct & values )
{
	dateandcustomer ac; 
	ac.datum = values.deliverydate.getDate();
	ac.customer = values.customerno;

	InnerScheduledDeliveryCollection & innerScheduledDeliveryCollection = getScheduledDeliveryCollection( values );
	IScheduledDeliveryPtr scheduledDelivery;
	InnerScheduledDeliveryCollection::iterator iter = innerScheduledDeliveryCollection.find( ac );
	if( iter == innerScheduledDeliveryCollection.end() )
	{
		scheduledDelivery = m_ScheduledDeliveryFactory->create( basar::db::aspect::AccessorPropertyTable_YIterator() );
		innerScheduledDeliveryCollection[ ac ] = scheduledDelivery;

		//--> hier match?
	}

	iter = innerScheduledDeliveryCollection.find( ac );
	scheduledDelivery = iter->second;
	matchToHeader( scheduledDelivery->getHeader()->get(), values );

	return scheduledDelivery;
}

void ScheduledDeliveryImportCollection::matchToHeader( basar::db::aspect::AccessorPropertyTable_YIterator yitHead, const ScheduledDeliveryImportStruct & values )
{
	yitHead.setInt16( BRANCH_NO, values.branchno );
	yitHead.setInt32( CUSTOMER_NO, values.customerno );
	yitHead.setDate( DELIVERY_DATE, values.deliverydate );
}

void ScheduledDeliveryImportCollection::matchToPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitPos, const ScheduledDeliveryImportStruct & values )
{
	yitPos.setString( ARTICLE_CODE, values.articlecode );
	yitPos.setInt32( QUANTITY, values.quantity );
	yitPos.setInt16( PURCHASE_ORDER_PROPOSAL_FLAG, values.purchaseorderproposalflag );
	yitPos.setInt16( BRANCH_NO_POP, values.branchnopop );
}

} // end namespace scheduledDelivery
} // end namespace domMod
} // end namespace libtender

