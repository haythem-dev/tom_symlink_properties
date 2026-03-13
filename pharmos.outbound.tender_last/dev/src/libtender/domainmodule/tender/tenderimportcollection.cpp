#include "tenderimportcollection.h"
#include "itender.h"
#include "tenderdm.h"
#include "tenderimportstruct.h"
#include <domainmodule/libtender_properties_definitions.h>
#include <componentmanager/itenderconnectedfactory.h>

namespace libtender
{
namespace domMod
{
namespace tender
{
using namespace properties;

TenderImportCollection::TenderImportCollection( const log4cplus::Logger logger )
: m_Logger( logger )
{
}

TenderImportCollection::~TenderImportCollection()
{
}

void TenderImportCollection::injectTenderFactory( componentManager::ITenderConnectedFactoryPtr tenderFactory )
{
	m_TenderFactory = tenderFactory;
}

void TenderImportCollection::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
{
	m_TransactionFactory = transactionFactory;
}

basar::Int32 TenderImportCollection::getTenderCount() const
{
	return static_cast<basar::Int32>(m_Tenders.size());
}

basar::Int32 TenderImportCollection::getStartingTenderNo() const
{
	InnerCollection::const_iterator iter = m_Tenders.begin();
	if( iter == m_Tenders.end() )
	{
		return 0;
	}

	ITenderPtr tender = iter->second;
	return tender->getHeader().getInt32( TENDER_NO );
}

void TenderImportCollection::addPosition( const TenderImportStruct & values )
{
	ITenderPtr tender = getTender( values );
	matchToPosition( tender->addEmptyPosition(), values );
}

void TenderImportCollection::save(bool importing)
{
	libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();

	try
	{
		transaction->begin();
		InnerCollection::iterator iter = m_Tenders.begin();
		for( ; iter != m_Tenders.end(); ++iter )
		{
			ITenderPtr tender = iter->second;
			tender->save(importing);
		}
		transaction->commit();
	}
	catch( const basar::Exception & e  )
	{
		transaction->rollback();
		std::stringstream ss;
		ss << "TenderImportCollection::save(): " << e.what().c_str();
		BLOG_ERROR( m_Logger, ss.str().c_str() );
		throw;
	}
}

void TenderImportCollection::clear()
{
	m_Tenders.clear();
}

bool TenderImportCollection::isValidID( const basar::Int32 id ) const
{
	InnerCollection::const_iterator iter = m_Tenders.find( id );
	return ( iter != m_Tenders.end() );
}

basar::Int32 TenderImportCollection::getTenderID( const basar::Int32 id ) const
{
	InnerCollection::const_iterator iter = m_Tenders.find( id );
	if( iter == m_Tenders.end() )
	{
		return 0;
	}

	ITenderPtr tender = iter->second;
	return tender->getHeader().getInt32( TENDER_NO );
}

ITenderPtr TenderImportCollection::getTender( const basar::Int32 id ) const
{
	ITenderPtr tender;
	InnerCollection::const_iterator iter = m_Tenders.find( id );
	if( iter != m_Tenders.end() )
	{
		tender = iter->second;
	}

	return tender;
}

ITenderPtr TenderImportCollection::getTender( const TenderImportStruct & values )
{
	ITenderPtr tender;
	InnerCollection::iterator iter = m_Tenders.find( values.id );
	if( iter == m_Tenders.end() )
	{
		tender = m_TenderFactory->create( basar::db::aspect::AccessorPropertyTable_YIterator() );
		m_Tenders[ values.id ] = tender;
	}

	iter = m_Tenders.find( values.id );
	tender = iter->second;
	matchToHeader( tender->getHeader(), values );

	return tender;
}

void TenderImportCollection::matchToHeader( basar::db::aspect::AccessorPropertyTable_YIterator yitHead, const TenderImportStruct & values )
{
	yitHead.setInt32( TENDER_TYPE, values.tendertype );
	yitHead.setString( CONTRACT_NO, values.contractno );
	yitHead.setInt32( CUSTOMER_NO, values.customerno );
	yitHead.setInt32( ACC_CUSTOMER_NO, values.customerno );
	yitHead.setInt32( PHARMACY_CHAIN_NO, values.pharmacychainno );
	yitHead.setString( CUSTOMERGROUPID, values.customergroupid );
	yitHead.setInt32( PARTNER_NO, values.partnerno );
	yitHead.setDate( DATE_FROM, values.datefrom );
	yitHead.setDate( DATE_TO, values.dateto );
	yitHead.setInt16( TENDER_STATE, values.tenderstate );
	yitHead.setInt16( BRANCH_NO, values.branchno );
	yitHead.setInt16( ACC_BRANCH_NO, values.branchno );
	yitHead.setInt16( PURCHASE_ORDER_PROPOSAL_FLAG, 0 );

	//Because of CH
	yitHead.setString( PHARMACY_GROUPID, "");
}

void TenderImportCollection::matchToPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitPos, const TenderImportStruct & values )
{
	yitPos.setInt32( ARTICLE_NO, values.articleno );
	yitPos.setString( ARTICLE_CODE, values.articlecode );
	yitPos.setInt32( CONTRACT_QTY, values.contractqty );
	yitPos.setDecimal( CONTRACT_PRICE, values.contractprice );
	yitPos.setDecimal( ADD_DISCOUNT_PCT, values.adddiscountpct );
	yitPos.setInt16( DISCOUNT_CALC_FROM, values.discountcalcfrom );
	yitPos.setInt16( DISCOUNT_APPLY_TO, values.discountapplyto );
}

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

