#include "tenderdm.h"
#include "itenderheader.h"
#include "itenderpositioncollection.h"
#include "itenderpositioncollectionptr.h"
#include "domainmodule/customerlist/icustomerlistdm.h"
#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>

#include <libbasarlogin.h>
#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

#include <domainmodule/constants.h>

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
namespace tender
{
const libutil::misc::ClassInfo& TenderDM::getClassInfo()
{
	static const libutil::misc::ClassInfo info( "TenderDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	return info;
}

TenderDM::TenderDM()
: m_Logger( libtender::LoggerPool::getLoggerDomModules() )
{
	METHODNAME_DEF( TenderDM, TenderDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

TenderDM::~TenderDM()
{
	METHODNAME_DEF( TenderDM, ~TenderDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void TenderDM::injectHeader( ITenderHeaderPtr header )
{
	m_Header = header;
}

void TenderDM::injectPositionCollection( ITenderPositionCollectionPtr positionCollection )
{
	m_PositionCollection = positionCollection;
}

void TenderDM::injectCustomerList( libtender::domMod::customerlist::ICustomerListDMPtr customerList )
{
	m_CustomerList = customerList;
}

void TenderDM::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
{
	m_TransactionFactory = transactionFactory;
}

void TenderDM::setAreaID(basar::Int16 areaID)
{
	m_AreaID = areaID;
}

basar::db::aspect::AccessorPropertyTable_YIterator TenderDM::getHeader()
{
	return m_Header->get();
}

ITenderPositionCollectionPtr TenderDM::getPositionCollection()
{
	return m_PositionCollection;
}

libtender::domMod::customerlist::ICustomerListDMPtr TenderDM::getCustomerList()
{
	return m_CustomerList;
}

basar::db::aspect::AccessorPropertyTable_YIterator TenderDM::addEmptyPosition()
{
	return m_PositionCollection->addEmptyPosition();
}

void TenderDM::deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	m_PositionCollection->deletePosition( yit );
}

void TenderDM::save(bool importing)
{
	METHODNAME_DEF(TenderDM, save)
	BLOG_TRACE_METHOD(getLogger(), fun);

	libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();

	try
	{
		transaction->begin();

        if (importing == true || m_AreaID != constants::REGION_RS)
        {
            m_PositionCollection->setState(m_Header->getState());
        }

		//m_PositionCollection->setState( m_Header->getState());
		m_PositionCollection->setNameOfUser( basar::login::Manager::getInstance().getUserName().c_str() );

		m_Header->get().setString(properties::NAME_OF_USER, basar::login::Manager::getInstance().getUserName().c_str());
		m_Header->save();

		m_PositionCollection->setForeignKey( m_Header->getPrimaryKey() );
		m_PositionCollection->save();

		m_CustomerList->setTenderNo(m_Header->getPrimaryKey());
		m_CustomerList->saveDeleteCustomers();
		m_CustomerList->saveExistingCustomers();
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

void TenderDM::deleteTender()
{
	METHODNAME_DEF( TenderDM, deleteTender )
	BLOG_TRACE_METHOD( getLogger(), fun );

	libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();

	try
	{
		transaction->begin();

		m_PositionCollection->save();
		m_Header->save();
		m_CustomerList->saveDeleteCustomers();

		transaction->commit();
	}
	catch( const basar::Exception & e  )
	{
		transaction->rollback();

		BLOG_ERROR( getLogger(), e.what().c_str() );
		throw;
	}
}

void TenderDM::findByKey( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
{
	METHODNAME_DEF( TenderDM, findByKey )
	BLOG_TRACE_METHOD( getLogger(), fun );

	try
	{
		m_Header->findByKey(yitSearch);

		switch(m_AreaID)
		{
		case constants::REGION_BG: m_PositionCollection->findByKeyBG(yitSearch);
			break;
		case constants::REGION_RS: m_PositionCollection->findByKeyRS(yitSearch);
			break;
		case constants::REGION_FR: m_PositionCollection->findByKeyFR(yitSearch);
			break;
		case constants::REGION_CH: m_PositionCollection->findByKeyCH(yitSearch);
			break;
		}

		getCustomerList()->findByTenderNo(yitSearch);
	}
	catch(const basar::Exception & e)
	{
		std::stringstream ss;
		ss << fun << ": " << e.what().c_str();
		BLOG_ERROR(getLogger(), ss.str().c_str());
		throw;
	}
}

libutil::domMod::SearchYIteratorPtr TenderDM::getSearchYIterator()
{
	METHODNAME_DEF( TenderDM, getSearchYIterator )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if( NULL == m_SearchYIterator.get() )
	{
		m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
		new libutil::domMod::SearchYIterator(properties::TENDER_ID.toDescriptionString()) );
	}

	return m_SearchYIterator;
}

const log4cplus::Logger& TenderDM::getLogger() const
{
	METHODNAME_DEF( TenderDM, getLogger )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_Logger;
}

void TenderDM::setTenderState( TenderStateEnum state )
{
	METHODNAME_DEF( TenderDM, setTenderState )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_Header->setState(state);
	m_PositionCollection->setState(state);	
}

TenderStateEnum TenderDM::getTenderState() const
{
	return m_Header->getState();
}

} // end namespace tender
} // end namespace domMod
} // end namespace libtender
