#ifndef GUARD_TENDER_USECASE_TENDER_IMPORT_UC_H
#define GUARD_TENDER_USECASE_TENDER_IMPORT_UC_H

#include <libutil/irunnable.h>
#include <libbasarappl.h>
#include <libbasarguitie_viewconnptr.hpp>
#include "tenderimport/tenderimportvcptr.h"
#include "tenderimport/tenderimportfilereaderptr.h"
#include "tenderimport/tenderimportcheckerptr.h"
#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/tender/itenderimportcollectionptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryimportcollectionptr.h>
#include <libtender/domainmodule/scheduleddelivery/scheduleddeliveryimportstruct.h>
#include <libtender/domainmodule/purchaseorderproposal/ipurchaseorderproposalptr.h>
#include "iprogress.h"
#include "ierrorhandler.h"
#include <boost/enable_shared_from_this.hpp>
#include <libutil/dbtransaction.h>

namespace tender
{
namespace useCase
{
class TenderImportUC : public libutil::useCase::IRunnable,
					   public IProgress,
					   public IErrorHandler,
					   public boost::enable_shared_from_this<TenderImportUC>
{
public:
	TenderImportUC();
	virtual ~TenderImportUC();

	void setFileName( const basar::VarString & filename );

	void inject( tender::viewConn::TenderImportVCPtr tenderImportVC );
	void inject( libtender::domMod::tender::ITenderImportCollectionPtr tenderImportCollection );
	void inject( libtender::domMod::scheduledDelivery::IScheduledDeliveryImportCollectionPtr scheduledDeliveryImportCollection );
	void inject( tender::useCase::TenderImportFileReaderPtr tenderImportFileReader );
	void inject( tender::useCase::TenderImportCheckerPtr tenderImportChecker );
	void inject( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory );
	void inject( libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr purchaseOrderProposal );

	void setParentWindow( QWidget * parent );
	void setArea(basar::Int16 area);

	virtual void run();
	virtual void progress( const basar::Int32 percent );
	virtual void handleInfo( const basar::I18nString & info );
	virtual void handleWarning( const basar::I18nString & warning );
	virtual void handleError( const basar::I18nString & error );

	SYSTEM_EVENT_HANDLER_DECLARATION( StartImportRequested );

private:
	TenderImportUC( const TenderImportUC & );
	TenderImportUC operator=( const TenderImportUC & );

	void registerEventHandlers( const bool );
	void createPurchaseOrderProposal( const libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct & values );
	void addToCustomerList(libtender::domMod::tender::ITenderPtr tender, libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct positionData);

	tender::viewConn::TenderImportVCPtr												m_TenderImportVC;
	basar::VarString																m_Filename;
	libtender::domMod::tender::ITenderImportCollectionPtr							m_TenderImportCollection;
	libtender::domMod::scheduledDelivery::IScheduledDeliveryImportCollectionPtr		m_ScheduledDeliveryImportCollection;
	tender::useCase::TenderImportFileReaderPtr										m_TenderImportFileReader;
	tender::useCase::TenderImportCheckerPtr											m_TenderImportChecker;
	libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr			m_TransactionFactory;
	libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr				m_PurchaseOrderProposal;
	basar::Int16																	m_Area;
};

} // namespace useCase
} // namespace tender

#endif // GUARD_TENDER_USECASE_TENDER_IMPORT_UC_H
