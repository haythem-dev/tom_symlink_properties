#include "tenderfactorybase.h"

#include <domainmodule/tender/tenderheaderptr.h>
#include <domainmodule/tender/tenderheader.h>
#include <domainmodule/tender/tenderpositioncollectionptr.h>
#include <domainmodule/tender/tenderpositioncollection.h>
#include <domainmodule/tender/tenderdmptr.h>
#include <domainmodule/tender/tenderdm.h>
#include <domainmodule/customerlist/customerlistdmptr.h>
#include <domainmodule/customerlist/customerlistdm.h>
#include <libutil/session.h>

#include <infrastructure/accessor/tender/tenderheaderacc.h>
#include <infrastructure/accessor/tender/tenderheaderacc_definitions.h>
#include <infrastructure/accessor/tender/tenderpositionsacc.h>
#include <infrastructure/accessor/tender/tenderpositionsacc_definitions.h>
#include <infrastructure/accessor/customerlist/customerlistacc.h>
#include <infrastructure/accessor/customerlist/customerlistacc_definitions.h>

#include <infrastructure/countrycodemapper.h>

namespace libtender
{
namespace componentManager
{
TenderFactoryBase::TenderFactoryBase()
{        
}

TenderFactoryBase::~TenderFactoryBase()
{       
}
        
void TenderFactoryBase::injectDBConnection( basar::db::aspect::ConnectionRef dbConnection )
{
	m_DBConnection = dbConnection;
}

void TenderFactoryBase::injectSession( libutil::infrastructure::session::ISessionPtr session )
{
	m_Session = session;
}

void TenderFactoryBase::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
{
	m_TransactionFactory = transactionFactory;
}

domMod::tender::ITenderPtr TenderFactoryBase::create() const
{
	domMod::tender::TenderDMPtr tender( new domMod::tender::TenderDM() );
	tender->injectHeader( getTenderHeader() );
	tender->injectPositionCollection( getTenderPositionCollection() );
	tender->injectCustomerList( getCustomerList() );
	tender->injectTransactionFactory( getTransactionFactory() );
	tender->setAreaID( m_Session->getUser()->getAreaID() );
	
	return tender;
}
        
libtender::domMod::tender::ITenderHeaderPtr TenderFactoryBase::getTenderHeader() const
{
	libtender::domMod::tender::TenderHeaderPtr tenderHeader( new libtender::domMod::tender::TenderHeader() );
	tenderHeader->injectTenderHeaderAccessor( getTenderHeaderAccessor() );
	return tenderHeader;
}

libtender::domMod::tender::ITenderPositionCollectionPtr TenderFactoryBase::getTenderPositionCollection() const
{
	libtender::domMod::tender::TenderPositionCollectionPtr tenderPositionCollection( new libtender::domMod::tender::TenderPositionCollection() );
	tenderPositionCollection->injectTenderPositionCollectionAccessor( getTenderPositionCollectionAccessor() );

	return tenderPositionCollection;
}

libtender::domMod::customerlist::ICustomerListDMPtr TenderFactoryBase::getCustomerList() const
{
	libtender::domMod::customerlist::CustomerListDMPtr customerList( new libtender::domMod::customerlist::CustomerListDM() );
	customerList->injectCustomerAccessor( getCustomerListAccessor() );
	return customerList;
}

libutil::infrastructure::accessor::IAccessorPtr TenderFactoryBase::getTenderHeaderAccessor() const
{
	libtender::infrastructure::accessor::tender::SelectTenderHeaderByKey();  // pseudo reference
	libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr( 
		new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::tender::lit::TENDER_HEADER_ACC, getDBConnection() ) 
			);
	return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr TenderFactoryBase::getTenderPositionCollectionAccessor() const
{
	libtender::infrastructure::accessor::tender::SelectTenderPositionsByKeyFR();  // pseudo reference
	libtender::infrastructure::accessor::tender::SelectTenderPositionsByKeyCH();  // pseudo reference

	//static bool accessorDefined = false;
	//if( false == accessorDefined )
	//{
	//	// define the accessor once
	//	using namespace basar::db::aspect;
	//	AccessorDefinitionRef accessorDefinition = Manager::getInstance().getAccessorDefinition( libtender::infrastructure::accessor::tender::lit::TENDER_POSITIONS_ACC );
	//	AccessMethodRef am = accessorDefinition.getAccessMethod( libtender::infrastructure::accessor::tender::lit::SELECT_TENDER_POSITIONS_BY_KEY );
	//	if( libtender::infrastructure::CID_CH == getCountryID() )
	//	{
	//		libtender::infrastructure::accessor::tender::SelectTenderPositionsByKeyCH sqlStringBuilder;
	//		am.push_back( sqlStringBuilder );
	//	}
	//	else
	//	{
	//		libtender::infrastructure::accessor::tender::SelectTenderPositionsByKey sqlStringBuilder;
	//		am.push_back( sqlStringBuilder );
	//	}
	//	accessorDefined = true;
	//}

	return libutil::infrastructure::accessor::AccessorProxyPtr( 
		new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::tender::lit::TENDER_POSITIONS_ACC, getDBConnection() ) 
			);
}

libutil::infrastructure::accessor::IAccessorPtr TenderFactoryBase::getCustomerListAccessor() const
{
	libtender::infrastructure::accessor::customerlist::SelectCustomerByTenderNo(); // pseudo reference
	libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
		new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::customerlist::lit::CUSTOMER_LIST_ACC, getDBConnection(), basar::db::aspect::FULL_CACHING, false )
		);
	return accessor;
}

basar::db::aspect::ConnectionRef TenderFactoryBase::getDBConnection() const
{
	return m_DBConnection;
}

libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr TenderFactoryBase::getTransactionFactory() const
{
	return m_TransactionFactory;
}
infrastructure::CountryIDEnum TenderFactoryBase::getCountryID() const
{
	return infrastructure::CountryCodeMapper::getCountryID( m_Session->getUser()->getAreaID() );
}


} // end namespace componentManager
} // end namespace libtender
