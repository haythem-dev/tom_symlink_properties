#include "scheduleddeliverydm.h"
#include "ischeduleddeliveryheader.h"
#include "ischeduleddeliverypositioncollection.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

#include <libbasardbaspect.h>

namespace libtender
{
	namespace domMod
	{
		namespace tender
		{
			using basar::db::aspect::AccessorPropertyTable_YIterator;
			using basar::db::aspect::AccessorPropertyTableRef;
		}
	}
}

namespace libtender
{
namespace domMod
{
namespace scheduledDelivery
{

ScheduledDeliveryDM::ScheduledDeliveryDM()
: m_Logger( libtender::LoggerPool::getLoggerDomModules() )
{
	METHODNAME_DEF( ScheduledDeliveryDM, ScheduledDeliveryDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

ScheduledDeliveryDM::~ScheduledDeliveryDM()
{
	METHODNAME_DEF( ScheduledDeliveryDM, ~ScheduledDeliveryDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void ScheduledDeliveryDM::injectHeader( IScheduledDeliveryHeaderPtr header )
{
	m_Header = header;
}

void ScheduledDeliveryDM::injectPositionCollection( IScheduledDeliveryPositionCollectionPtr positionCollection )
{
	m_PositionCollection = positionCollection;
}

void ScheduledDeliveryDM::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
{
	m_TransactionFactory = transactionFactory;
}

IScheduledDeliveryHeaderPtr ScheduledDeliveryDM::getHeader()
{
	return m_Header;
}

IScheduledDeliveryPositionCollectionPtr ScheduledDeliveryDM::getPositionCollection()
{
	return m_PositionCollection;
}

libutil::domMod::SearchYIteratorPtr	ScheduledDeliveryDM::getSearchYIterator()
{
	METHODNAME_DEF( ScheduledDeliveryDM, getSearchYIterator )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( NULL == m_SearchYIterator.get() )
	{
		m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
		new libutil::domMod::SearchYIterator( properties::TENDER_NO.toDescriptionString()  ) );
	}

	return m_SearchYIterator;
}

const log4cplus::Logger & ScheduledDeliveryDM::getLogger() const
{
	METHODNAME_DEF( ScheduledDeliveryDM, getLogger )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_Logger;
}

basar::db::aspect::AccessorPropertyTable_YIterator ScheduledDeliveryDM::addEmptyPosition()
{
	return m_PositionCollection->addEmptyPosition();
}

void ScheduledDeliveryDM::deletePosition(basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	m_PositionCollection->deletePosition(yit);
}

void ScheduledDeliveryDM::findByKey(const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch)
{
	METHODNAME_DEF( ScheduledDeliveryDM, findByKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	try
	{
		m_Header->findByKey(yitSearch);
		m_PositionCollection->getSearchYit().match( yitSearch);
		m_PositionCollection->findByKey(m_PositionCollection->getSearchYit());
	}
	catch(const basar::Exception & e)
	{
		BLOG_ERROR(getLogger(), e.what().c_str());
		throw;
	}
}

bool ScheduledDeliveryDM::isDeliveryExisting()
{
	METHODNAME_DEF( ScheduledDeliveryDM, isDeliveryExisting )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return m_Header->get().isContainedAndSet(properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID);
}

bool ScheduledDeliveryDM::isPurchaseOrderProposalExisting()
{
	METHODNAME_DEF( ScheduledDeliveryDM, isPurchaseOrderProposalExisting )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_PositionCollection->get().begin();

	while (!yitPos.isEnd())
	{
		if (yitPos.isContainedAndSet(properties::ORDER_PROPOSAL_NO) && yitPos.getInt32(properties::ORDER_PROPOSAL_NO) > 0)
		{
			return true;
		}

		++yitPos;
	}

	return false;
}

void ScheduledDeliveryDM::save()
{
	METHODNAME_DEF( ScheduledDeliveryDM, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

	libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();

	try
	{
		transaction->begin();
		
		m_Header->save();
		m_PositionCollection->setForeignKey(m_Header->getPrimaryKey());
		m_PositionCollection->save();

		transaction->commit();
	}
	catch(const basar::Exception & e)
	{
		transaction->rollback();
		std::stringstream ss;
		ss << fun << ": " << e.what().c_str();
		BLOG_ERROR(getLogger(), ss.str().c_str());
		throw;
	}
}

} //end namespace scheduledDelivery
} //end namespace domMod
} //end namespace libtender
