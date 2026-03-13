#include "tenderimportuc.h"

#include <libtender/domainmodule/constants.h>
#include <loggerpool/tenderloggerpool.h>
#include <libbasardbaspect.h>
#include "tenderimportchecker.h"
#include "tenderimportfilereader.h"
#include <libtender/domainmodule/tender/itender.h>
#include <libtender/domainmodule/tender/itenderheader.h>
#include <libtender/domainmodule/tender/itenderimportcollection.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryimportcollection.h>
#include <libtender/domainmodule/purchaseorderproposal/ipurchaseorderproposal.h>
#include <libtender/domainmodule/libtender_properties_definitions.h>
#include <libtender/domainmodule/customerlist/icustomerlistdm.h>
#include "tenderimport/tenderimportvc.h"

#include <libtender\domainmodule\tender\itenderpositioncollectionptr.h>
#include <libtender\domainmodule\tender\itenderpositioncollection.h>

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4251 4311 4312 4481 4512 4800)
	#include <QCoreApplication>
#pragma warning (pop)

namespace tender
{
namespace useCase
{
BEGIN_HANDLERS_REGISTRATION( TenderImportUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( TenderImportUC, StartImportRequested )
END_HANDLERS_REGISTRATION()

TenderImportUC::TenderImportUC()
{
}

TenderImportUC::~TenderImportUC()
{
}

SYSTEM_EVENT_HANDLER_DEFINITION( TenderImportUC, StartImportRequested )
{
	METHODNAME_DEF( TenderImportUC, StartImportRequested )
	//BLOG_TRACE_METHOD( m_Logger, fun );
	rSource;
	
	static basar::I18nString title( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Tender import" ).toLocal8Bit().constData() );
	std::stringstream ss;

	libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction;

	try
	{
		m_TenderImportVC->resetProgress();
		m_TenderImportChecker->reset();

		basar::VarString positionsFilename;
		m_TenderImportVC->matchToPositionsFilename( positionsFilename );

		basar::VarString deliveriesFilename;
		m_TenderImportVC->matchToDeliveriesFilename( deliveriesFilename );

		m_TenderImportVC->setProgressDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Reading tender positions..." ).toLocal8Bit().constData() );
		m_TenderImportVC->progress( 0 );

		m_TenderImportFileReader->readTenderPositions( positionsFilename );
		m_TenderImportVC->progress( 0 );

		basar::Int32 tenderPositionCount = m_TenderImportFileReader->getTenderLineCount();
		ss.str( "" );
		ss << tenderPositionCount << " " << QCoreApplication::translate( "tender::useCase::TenderImportUC", "tender positions read" ).toLocal8Bit().constData();
		m_TenderImportVC->addInfoMessage( ss.str().c_str() );

		if( 0 == tenderPositionCount )
		{
			return basar::appl::EventReturnType( basar::appl::HANDLER_ERROR );
		}

		m_TenderImportVC->setProgressDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Checking tender positions..." ).toLocal8Bit().constData() );

		basar::Int32 numberErrors = 0;
		for( basar::Int32 i = 0; i < tenderPositionCount; ++i )
		{
			TenderImportFileReader::TenderLineValue & position = m_TenderImportFileReader->getTenderLineValue( i );
			if( false == m_TenderImportChecker->checkTenderPosition( position.lineNumber, position.positionData ) )
			{
				++numberErrors;
			}
			m_TenderImportVC->progress( i * 100 / tenderPositionCount );
		}

		m_TenderImportVC->progress( 0 );

		if( 0 < numberErrors )
		{
			ss.str( "" );
			ss << numberErrors << " " << QCoreApplication::translate( "tender::useCase::TenderImportUC", "lines with errors found" ).toLocal8Bit().constData();
			m_TenderImportVC->addInfoMessage( ss.str().c_str() );
			return basar::appl::EventReturnType( basar::appl::HANDLER_ERROR );
		}

		m_TenderImportVC->setProgressDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Creating tenders..." ).toLocal8Bit().constData() );
		for( basar::Int32 i = 0; i < tenderPositionCount; ++i )
		{
			TenderImportFileReader::TenderLineValue & position = m_TenderImportFileReader->getTenderLineValue( i );
			m_TenderImportCollection->addPosition( position.positionData );
			m_TenderImportVC->progress( i * 100 / tenderPositionCount );
		}

		m_TenderImportVC->progress( 0 );
		ss.str( "" );
		ss << m_TenderImportCollection->getTenderCount() << " " << QCoreApplication::translate( "tender::useCase::TenderImportUC", "tenders created" ).toLocal8Bit().constData();
		m_TenderImportVC->addInfoMessage( ss.str().c_str() );

		basar::Int32 scheduledDeliveryPositionCount = 0;

		if(deliveriesFilename != "")
		{
			m_TenderImportVC->setProgressDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Reading scheduled delivery positions..." ).toLocal8Bit().constData() );
			m_TenderImportVC->progress( 0 );

			m_TenderImportFileReader->readScheduledDeliveryPositions( deliveriesFilename );
			m_TenderImportVC->progress( 0 );

			scheduledDeliveryPositionCount = m_TenderImportFileReader->getScheduledDeliveryLineCount();
			ss.str( "" );
			ss << scheduledDeliveryPositionCount << " " << QCoreApplication::translate( "tender::useCase::TenderImportUC", "scheduled delivery positions read" ).toLocal8Bit().constData();
			m_TenderImportVC->addInfoMessage( ss.str().c_str() );

			if( 0 == scheduledDeliveryPositionCount )
			{
				return basar::appl::EventReturnType( basar::appl::HANDLER_ERROR );
			}

			m_TenderImportVC->setProgressDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Checking scheduled delivery positions..." ).toLocal8Bit().constData() );

			numberErrors = 0;
			for( basar::Int32 i = 0; i < scheduledDeliveryPositionCount; ++i )
			{
				TenderImportFileReader::ScheduledDeliveryLineValue & position = m_TenderImportFileReader->getScheduledDeliveryLineValue( i );

				libtender::domMod::tender::ITenderPtr tender = m_TenderImportCollection->getTender(position.positionData.id);
				basar::db::aspect::AccessorPropertyTable_YIterator tenderHead = tender->getHeader();
				if( false == m_TenderImportChecker->checkScheduledDeliveryPosition( position.lineNumber, position.positionData, tenderHead ) )
				{
					++numberErrors;
				}

				if( m_Area == constants::REGION_BG  && tenderHead.getInt32(properties::TENDER_TYPE) == 2)
				{
					addToCustomerList(tender, position.positionData);
				}

				m_TenderImportVC->progress( i * 100 / scheduledDeliveryPositionCount );
			}

			m_TenderImportVC->progress( 0 );

			if( 0 < numberErrors )
			{
				ss.str( "" );
				ss << numberErrors << " " << QCoreApplication::translate( "tender::useCase::TenderImportUC", "lines with errors found" ).toLocal8Bit().constData();
				m_TenderImportVC->addInfoMessage( ss.str().c_str() );
				return basar::appl::EventReturnType( basar::appl::HANDLER_ERROR );
			}
		}

		transaction = m_TransactionFactory->createDBTransaction();
		{
			transaction->begin();

			m_TenderImportVC->setProgressDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Saving tenders..." ).toLocal8Bit().constData() );

			bool saveTendersByImporting = true;

			m_TenderImportCollection->save(saveTendersByImporting);

			if(scheduledDeliveryPositionCount != 0)
			{
				m_TenderImportVC->setProgressDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Creating scheduled deliveries..." ).toLocal8Bit().constData() );
				for( basar::Int32 i = 0; i < scheduledDeliveryPositionCount; ++i )
				{
					TenderImportFileReader::ScheduledDeliveryLineValue & position = m_TenderImportFileReader->getScheduledDeliveryLineValue( i );

					libtender::domMod::tender::ITenderPtr tender = m_TenderImportCollection->getTender(position.positionData.id);
					m_ScheduledDeliveryImportCollection->addPosition( position.positionData );
					m_TenderImportVC->progress( i * 100 / scheduledDeliveryPositionCount );
				}
				
				m_TenderImportVC->progress( 0 );
				ss.str( "" );
				ss << m_ScheduledDeliveryImportCollection->getScheduledDeliveryCount() << " " << QCoreApplication::translate( "tender::useCase::TenderImportUC", "scheduled deliveries created" ).toLocal8Bit().constData();
				m_TenderImportVC->addInfoMessage( ss.str().c_str() );

				m_TenderImportVC->setProgressDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Saving scheduled deliveries..." ).toLocal8Bit().constData() );
				m_ScheduledDeliveryImportCollection->save();

				m_TenderImportVC->setProgressDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Creating order proposals..." ).toLocal8Bit().constData() );
				basar::Int32 orderProposalCount = 0;
				for( basar::Int32 i = 0; i < scheduledDeliveryPositionCount; ++i )
				{
					TenderImportFileReader::ScheduledDeliveryLineValue & position = m_TenderImportFileReader->getScheduledDeliveryLineValue( i );
					createPurchaseOrderProposal( position.positionData );
					if( position.positionData.purchaseorderproposalflag )
					{
						++orderProposalCount;
					}
					m_TenderImportVC->progress( i * 100 / scheduledDeliveryPositionCount );
				}

				m_TenderImportVC->progress( 0 );
				ss.str( "" );
				ss << orderProposalCount << " " << QCoreApplication::translate( "tender::useCase::TenderImportUC", "order proposals created" ).toLocal8Bit().constData();
				m_TenderImportVC->addInfoMessage( ss.str().c_str() );
			}

			transaction->commit();
		}		

		m_TenderImportVC->progress( 0 );
		m_TenderImportVC->setProgressDescription( "" );
		m_TenderImportVC->addInfoMessage( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Import finished" ).toLocal8Bit().constData() );

		ss.str( "" );
		ss << QCoreApplication::translate( "tender::useCase::TenderImportUC", "Starting tender number" ).toLocal8Bit().constData() << ": " << m_TenderImportCollection->getStartingTenderNo();
		m_TenderImportVC->addInfoMessage( ss.str().c_str() );

		m_TenderImportCollection->clear();
		m_ScheduledDeliveryImportCollection->clear();
	}
	catch( const basar::Exception & e )
	{
		if( NULL != transaction )
		{
			transaction->rollback();
		}

		m_TenderImportVC->addErrorMessage( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Error importing tenders" ).toLocal8Bit().constData() );

		ss.str( "" );
		ss << QCoreApplication::translate( "tender::useCase::TenderImportUC", "Error importing tenders" ).toLocal8Bit().constData() << ":\n" << e.what().c_str();
		m_TenderImportVC->showErrorMessage( ss.str().c_str(), title );
		BLOG_ERROR( libtender::LoggerPool::getLoggerTenderImport(), e.what().c_str() );

		throw;
	}

	return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
}

void TenderImportUC::createPurchaseOrderProposal( const libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct & values )
{
	if( 0 == values.purchaseorderproposalflag )
	{
		return;
	}

	using namespace libtender::domMod::tender;
	ITenderPtr tender = m_TenderImportCollection->getTender( values.id );
	basar::Int32 articleNo = m_TenderImportChecker->getArticleNoByArticleCode( tender->getHeader().getInt32( properties::TENDER_TYPE ), values.articlecode );

	m_PurchaseOrderProposal->clear();
	m_PurchaseOrderProposal->setBranchNo( values.branchno );
	m_PurchaseOrderProposal->setArticleNo( articleNo );
	m_PurchaseOrderProposal->setOrderNoBatch( m_ScheduledDeliveryImportCollection->getScheduledDeliveryID( values ) );
	m_PurchaseOrderProposal->setOrderTypeBatch( constants::ORDER_TYPE_BATCH ); // TODO
	m_PurchaseOrderProposal->setOrderProposalQuantity( values.quantity );
	m_PurchaseOrderProposal->setExpectedGoodsinDate( values.deliverydate );
	m_PurchaseOrderProposal->setSupplierNo( tender->getHeader().getInt32( properties::PARTNER_NO ) );
	m_PurchaseOrderProposal->save();
}

void TenderImportUC::setFileName( const basar::VarString & filename )
{
	m_Filename = filename;
}

void TenderImportUC::inject( tender::viewConn::TenderImportVCPtr tenderImportVC )
{
	m_TenderImportVC = tenderImportVC;
}

void TenderImportUC::inject( libtender::domMod::tender::ITenderImportCollectionPtr tenderImportCollection )
{
	m_TenderImportCollection = tenderImportCollection;
}

void TenderImportUC::inject( libtender::domMod::scheduledDelivery::IScheduledDeliveryImportCollectionPtr scheduledDeliveryImportCollection )
{
	m_ScheduledDeliveryImportCollection = scheduledDeliveryImportCollection;
}

void TenderImportUC::inject( tender::useCase::TenderImportFileReaderPtr tenderImportFileReader )
{
	m_TenderImportFileReader = tenderImportFileReader;
}

void TenderImportUC::inject( tender::useCase::TenderImportCheckerPtr tenderImportChecker )
{
	m_TenderImportChecker = tenderImportChecker;
}

void TenderImportUC::inject( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
{
	m_TransactionFactory = transactionFactory;
}

void TenderImportUC::inject( libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr purchaseOrderProposal )
{
	m_PurchaseOrderProposal = purchaseOrderProposal;
}

void TenderImportUC::setParentWindow( QWidget * parent )
{
	m_TenderImportVC->setParent( parent );
}

void TenderImportUC::setArea(basar::Int16 area)
{
	m_Area = area;
}

void TenderImportUC::run()
{
	static basar::I18nString title( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Tender import" ).toLocal8Bit().constData() );

    try 
	{
		registerEventHandlers( true );



		//m_TenderImportVC->addMessage( "Message" );
		//m_TenderImportVC->addInfoMessage( "Info" );
		//m_TenderImportVC->addWarningMessage( "Warning" );
		//m_TenderImportVC->addErrorMessage( "Error" );

		m_TenderImportVC->show();

		//m_TenderImportVC->info( title, "Nase" );



/*		m_TenderImportVC->init( m_Filename );
		m_TenderImportVC->setDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Reading file..." ).toLocal8Bit().constData() );
		m_TenderImportVC->show();

		TenderImportFileReader fileReader;
		fileReader.inject( shared_from_this() );
		m_TenderImportFileReader.read( m_Filename );

		m_TenderImportVC->setDescription( QCoreApplication::translate( "tender::useCase::TenderImportUC", "Process tender positions..." ).toLocal8Bit().constData() );
		m_TenderImportVC->progress( 0 );

		basar::Int32 tenderPositionCount = fileReader.getLineCount();

		for( basar::Int32 i = 0; i < tenderPositionCount; ++i )
		{
			const libtender::domMod::tender::TenderImportStruct & position = fileReader.getPosition( i );
			m_TenderImportCollection->addPosition( position );
			m_TenderImportVC->progress( i * 100 / tenderPositionCount );
		}

		m_TenderImportCollection->save();
		basar::I18nString msg;
		msg.itos( tenderPositionCount );
		msg.append( " " );
		msg.append( QCoreApplication::translate( "tender::useCase::TenderImportUC", "positions imported" ).toLocal8Bit().constData() );
		m_TenderImportVC->hide();
		m_TenderImportVC->info( msg.c_str(), title );
*/

		m_TenderImportVC->hide();

		registerEventHandlers( false );
	}
	catch( const basar::Exception & e )
	{
		m_TenderImportVC->showErrorMessage( e.what().c_str(), title );
		m_TenderImportVC->hide();

		throw;
	}
}

void TenderImportUC::progress( const basar::Int32 percent )
{
	m_TenderImportVC->progress( percent );
}

void TenderImportUC::handleInfo( const basar::I18nString & info )
{
	m_TenderImportVC->addInfoMessage( info );
}

void TenderImportUC::handleWarning( const basar::I18nString & warning )
{
	m_TenderImportVC->addWarningMessage( warning );
}

void TenderImportUC::handleError( const basar::I18nString & error )
{
	m_TenderImportVC->addErrorMessage( error );
}

void TenderImportUC::addToCustomerList(libtender::domMod::tender::ITenderPtr tender, libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct positionData)
{
	bool isAlreadyAddedToCustomerList = false;

	for( basar::db::aspect::AccessorPropertyTable_YIterator yit = tender->getCustomerList()->get().begin(); !yit.isEnd(); ++yit)
	{
		if(yit.getInt32(properties::CUSTOMER_NO) == positionData.customerno)
		{
			isAlreadyAddedToCustomerList = true;
			break;
		}
	}

	if(!isAlreadyAddedToCustomerList)
	{
		basar::db::aspect::AccessorPropertyTable_YIterator customer = tender->getCustomerList()->addEmptyCustomer();
		customer.setInt16(properties::BRANCH_NO, tender->getHeader().getInt16(properties::BRANCH_NO));
		customer.setInt32(properties::CUSTOMER_NO, positionData.customerno);
	}
}

} // namespace useCase
} // namespace tender
