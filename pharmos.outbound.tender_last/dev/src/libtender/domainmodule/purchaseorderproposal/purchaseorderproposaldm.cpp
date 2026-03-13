#include "purchaseorderproposaldm.h"
#include <domainmodule/libtender_properties_definitions.h>
#include <infrastructure/accessor/purchaseorderproposal/purchaseorderproposalacc_definitions.h>
#include <infrastructure/purchasedbconn/purchasedbparameterproxy.h>
#include <loggerpool/tenderloggerpool.h>
#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

//-------------------------------------------------------------------------------------------------//

namespace libtender {
namespace domMod {
namespace purchaseOrderProposal {

//-------------------------------------------------------------------------------------------------//

PurchaseOrderProposalDM::PurchaseOrderProposalDM() :
	m_Logger( libtender::LoggerPool::getLoggerDomModules() )
{
	METHODNAME_DEF( PurchaseOrderProposalDM, PurchaseOrderProposalDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

//-------------------------------------------------------------------------------------------------//

PurchaseOrderProposalDM::~PurchaseOrderProposalDM()
{
	METHODNAME_DEF( ScheduledDeliveryDM, ~ScheduledDeliveryDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

//-------------------------------------------------------------------------------------------------//

void PurchaseOrderProposalDM::injectPurchaseOrderProposalAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
{
	METHODNAME_DEF( PurchaseOrderProposalDM, injectPurchaseOrderProposalAccessor )
	BLOG_TRACE_METHOD( getLogger(), fun );

	m_Accessor = accessor;
}

//-------------------------------------------------------------------------------------------------//

void PurchaseOrderProposalDM::injectPurchaseDBParameterProxy( infrastructure::db::IPurchaseDBParameterProxyPtr proxy)
{
	m_PurchaseDBParameterProxy = proxy;
}

//-------------------------------------------------------------------------------------------------//

void PurchaseOrderProposalDM::setBranchNo(const basar::Int16 val)				{ get().setInt16(properties::BRANCH_NO, val); }
void PurchaseOrderProposalDM::setArticleNo(const basar::Int32 val)				{ get().setInt32(properties::ARTICLE_NO, val); }
void PurchaseOrderProposalDM::setOrderProposalQuantity(const basar::Int32 val)	{ get().setInt32(properties::ORDER_PROPOSAL_QUANTITY, val); }
void PurchaseOrderProposalDM::setOrderNoBatch(const basar::Int32 val)			{ get().setInt32(properties::ORDER_NO_BATCH, val); }
void PurchaseOrderProposalDM::setOrderTypeBatch(const basar::Int16 val)			{ get().setInt16(properties::ORDER_TYPE_BATCH, val); }
void PurchaseOrderProposalDM::setExpectedGoodsinDate(const basar::Date val)		{ get().setInt32(properties::EXPECTED_GOODSIN_DATE, val.getDate()); }
void PurchaseOrderProposalDM::setSupplierNo(const basar::Int32 val)				{ get().setInt32(properties::SUPPLIER_NO, val); }

//-------------------------------------------------------------------------------------------------//

basar::Int32 PurchaseOrderProposalDM::getOrderProposalNo() const
{
	basar::Int32 orderProposalNo = 0;
	if (get().isContainedAndSet(properties::ORDER_PROPOSAL_NO))
	{
		orderProposalNo = get().getInt32(properties::ORDER_PROPOSAL_NO);
	}
	return orderProposalNo;
}

//-------------------------------------------------------------------------------------------------//

basar::Int32 PurchaseOrderProposalDM::getPurchaseOrderNo() const
{
	basar::Int32 purchaseOrderNo = 0;
	if (get().isContainedAndSet(properties::PURCHASE_ORDER_NO))
	{
		purchaseOrderNo = get().getInt32(properties::PURCHASE_ORDER_NO);
	}
	return purchaseOrderNo;
}

//-------------------------------------------------------------------------------------------------//

void PurchaseOrderProposalDM::markForDelete()
{
	getAccessor()->getPropertyTable().markForDelete(get());
}

//-------------------------------------------------------------------------------------------------//

libutil::infrastructure::accessor::IAccessorPtr PurchaseOrderProposalDM::getAccessor() const
{
	CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
	return m_Accessor;
}

//-------------------------------------------------------------------------------------------------//

basar::db::aspect::AccessorPropertyTable_YIterator PurchaseOrderProposalDM::getSearchYit() const
{
	METHODNAME_DEF( PurchaseOrderProposalDM, getSearchYit )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return getSearchYIterator()->get();
}

//-------------------------------------------------------------------------------------------------//

void PurchaseOrderProposalDM::resetSearchYit()
{
	METHODNAME_DEF( PurchaseOrderProposalDM, resetSearchYit )
	BLOG_TRACE_METHOD( getLogger(), fun );

	getSearchYIterator()->reset();
}

//-------------------------------------------------------------------------------------------------//

libutil::domMod::SearchYIteratorPtr	PurchaseOrderProposalDM::getSearchYIterator() const
{
	METHODNAME_DEF( PurchaseOrderProposalDM, getSearchYIterator )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( NULL == m_SearchYIterator.get() )
	{
		m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
			new libutil::domMod::SearchYIterator(
				properties::PURCHASE_DBSRV.toDescriptionString() +
				properties::PURCHASE_DB.toDescriptionString() +
				properties::BRANCH_NO.toDescriptionString() +
				properties::ARTICLE_NO.toDescriptionString() +
				properties::ORDER_NO_BATCH.toDescriptionString() +
				properties::ORDER_TYPE_BATCH.toDescriptionString()
				) 
			);
	}
	m_SearchYIterator->get().setString( properties::PURCHASE_DBSRV, m_PurchaseDBParameterProxy->getPurchaseDBServer() );
	m_SearchYIterator->get().setString( properties::PURCHASE_DB, m_PurchaseDBParameterProxy->getPurchaseDB() );

	return m_SearchYIterator;
}

//-------------------------------------------------------------------------------------------------//

const log4cplus::Logger& PurchaseOrderProposalDM::getLogger() const
{
	METHODNAME_DEF( PurchaseOrderProposalDM, getLogger )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_Logger;
}

//-------------------------------------------------------------------------------------------------//

void PurchaseOrderProposalDM::findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
{
	METHODNAME_DEF( ScheduledDeliveryHeader, findByKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::purchaseOrderProposal::lit::SELECT_PURCHASEORDERPROPOSAL_BY_KEY;
	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		getAccessor()->getPropertyTable().clear();
		executeResultInfo = getAccessor()->execute( SELECT_PURCHASEORDERPROPOSAL_BY_KEY, yitSearch );
	}
	catch( basar::Exception& e  )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SELECT_PURCHASEORDERPROPOSAL_BY_KEY, __FILE__, __LINE__ );
		throw;
	}

	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PURCHASEORDERPROPOSAL_BY_KEY, executeResultInfo )
	}
}

//-------------------------------------------------------------------------------------------------//

basar::db::aspect::AccessorPropertyTable_YIterator PurchaseOrderProposalDM::get()
{
	if( true == isEmpty() )
	{
		getAccessor()->getPropertyTable().insert( basar::FOR_INSERT );
	}
	basar::db::aspect::AccessorPropertyTable_YIterator yit = getAccessor()->getPropertyTable().begin();

	yit.setString( properties::PURCHASE_DBSRV, m_PurchaseDBParameterProxy->getPurchaseDBServer() );
	yit.setString( properties::PURCHASE_DB, m_PurchaseDBParameterProxy->getPurchaseDB() );
	return yit;
}

//-------------------------------------------------------------------------------------------------//

const basar::db::aspect::AccessorPropertyTable_YIterator PurchaseOrderProposalDM::get() const
{
	if( true == isEmpty() )
	{
		getAccessor()->getPropertyTable().insert( basar::FOR_INSERT );
	}
	basar::db::aspect::AccessorPropertyTable_YIterator yit = getAccessor()->getPropertyTable().begin();

	yit.setString( properties::PURCHASE_DBSRV, m_PurchaseDBParameterProxy->getPurchaseDBServer() );
	yit.setString( properties::PURCHASE_DB, m_PurchaseDBParameterProxy->getPurchaseDB() );
	return yit;
}

//-------------------------------------------------------------------------------------------------//

basar::Int32 PurchaseOrderProposalDM::getPrimaryKey() const
{
	METHODNAME_DEF( ScheduledDeliveryHeader, getPrimaryKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return get().getInt32( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.getName() );
}

//-------------------------------------------------------------------------------------------------//

bool PurchaseOrderProposalDM::isEmpty() const
{
	if( 0 == m_Accessor.get() ||
		getAccessor()->getPropertyTable().isNull() || 
		getAccessor()->getPropertyTable().isEmpty() )
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------//

void PurchaseOrderProposalDM::save()
{
	METHODNAME_DEF( PurchaseOrderProposalDM, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using libtender::infrastructure::accessor::purchaseOrderProposal::lit::SAVE_PURCHASEORDERPROPOSAL;
	
	basar::db::aspect::ExecuteResultInfo executeResultInfo;
	try
	{
		executeResultInfo = getAccessor()->execute( SAVE_PURCHASEORDERPROPOSAL, get() );
	}
	catch( basar::Exception & e )
	{
		libutil::exceptions::ExceptionHandler::processAccessorException( e, m_Logger, fun, SAVE_PURCHASEORDERPROPOSAL, __FILE__, __LINE__ );
		throw;
	}
	
	if( executeResultInfo.hasError() )
	{
		THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_PURCHASEORDERPROPOSAL, executeResultInfo );
	}
}

void PurchaseOrderProposalDM::clear()
{
	METHODNAME_DEF( PurchaseOrderProposalDM, clear )
	BLOG_TRACE_METHOD( getLogger(), fun );

	getAccessor()->getPropertyTable().clear();
}

//-------------------------------------------------------------------------------------------------//

} // namespace purchaseOrderProposal
} // namespace domMod
} // namespace libtender

//-------------------------------------------------------------------------------------------------//
