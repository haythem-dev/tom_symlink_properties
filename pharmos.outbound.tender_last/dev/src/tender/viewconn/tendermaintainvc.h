#ifndef GUARD_TENDER_VIEWCONN_TENDERMAINTAIN_VC_H
#define GUARD_TENDER_VIEWCONN_TENDERMAINTAIN_VC_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
	#include "ui_tendermaintainvc.h"
	#include <QObject>
    #include <QAction>
#pragma warning (pop)

#include "itendercontractinfoptr.h"
#include "itenderarticleinfoptr.h"

#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/tender/tenderstateenum.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h>
#include <libtender/domainmodule/tender/tendertypecollectionptr.h>
#include <libtender/domainmodule/tender/tenderstatecollectionptr.h>
#include <libutil/flag.h>
#include <libutil/viewconn.h>
#include "inputvalidator.h"

namespace basar
{
namespace db
{
namespace aspect
{
	class AccessorPropertyTable_YIterator;
	class AccessorPropertyTableRef;
}
}
}

namespace log4cplus
{
    class Logger;
}

namespace tender
{
namespace viewConn
{
class TenderMaintainVC : public libutil::viewConn::BaseVC
{
	Q_OBJECT

public:
	TenderMaintainVC( QWidget * parent, const basar::Int16 area );
	virtual ~TenderMaintainVC();

	void injectContractInfo( tender::viewConn::ITenderContractInfoPtr contractInfo );
    void injectArticleInfo( tender::viewConn::ITenderArticleInfoPtr articleInfo );
	void setPOPFunctionality( basar::I18nString popFunctionality );
	void setCloneDeliveriesFlag(bool flag);

	virtual basar::gui::tie::WidgetReturnEnum show();

	void matchFromTender( libtender::domMod::tender::ITenderPtr tender );
	void matchToTender( libtender::domMod::tender::ITenderPtr tender ) const;
    void matchFromTenderPositions( basar::db::aspect::AccessorPropertyTableRef positions);
	void matchToTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator tenderPosition ) const;
	void matchFromTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator tenderPosition );
	void matchFromScheduledDeliveryCollection( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection );
	void matchFromScheduledDeliveryPositionCollection( basar::db::aspect::AccessorPropertyTableRef scheduledDeliveryPositionCollection );
	void matchToArticleSearch( basar::db::aspect::AccessorPropertyTable_YIterator articleSearchYit ) const;
	void matchFromArticle( basar::db::aspect::AccessorPropertyTable_YIterator article );
	void matchToCustomerSearch( basar::db::aspect::AccessorPropertyTable_YIterator customerSearchYit ) const;
	void matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator customer );
    void matchToPharmacyGroupSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroupSearchYit ) const;
    void matchFromPharmacyGroup( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroup ); 
	void matchToPartnerSearch( basar::db::aspect::AccessorPropertyTable_YIterator partnerSearchYit ) const;
	void matchFromPartner( basar::db::aspect::AccessorPropertyTable_YIterator partner );
	void matchToPharmacyChainSearch ( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyChainSearchYit ) const;
	void matchFromPharmacyChain( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyChain );

	basar::db::aspect::AccessorPropertyTable_YIterator getCurrentTenderPosition();
	basar::db::aspect::AccessorPropertyTable_YIterator getCurrentScheduledDelivery();
	std::string getArticleToBeAdded();
	void resetTenderPosition();
	void enableTenderPositionEditing( bool isModifying );
	void resetDeliveries();
	void reset();
	void resetCustomer();
    void resetPharmacyGroup(); 
	void resetArticle();
	void prepareForCloning();
	void selectDelivery( basar::db::aspect::AccessorPropertyTable_YIterator scheduledDelivery );
	bool isTenderHeadFilled();
	bool isCustomerSelected();
	void checkContractInfo();
	void disableAllExceptState( bool disable );
	void tblDeliverySelectionChanged();
    void setupScheduleTab();
    void setTenderState( libtender::domMod::tender::TenderStateEnum );
	void ValidateAndSetPositionsStatus();

	void displayArticleAlreadySelected();
	void displayArticleExistsInDelivery();
	void displayConractQuantityUndershot( int quantity );
	void displayTenderHeaderMustBeFilled();
	void displayOpenDeliveriesWarning();
	void displayStateInfo();
	void displayPartnerNotValid();
	void displayPharmacyChainNotValid();
	void displayPharmacyGroupNotValid();
	void displayCustomerNotValid();
	void displayArticleNotExisting();
	void displayPurchaseOrderProposalExisting();
	void displaySaveTender();
	void displayCustomerListNotSaved();
	void displayDeliveryOutOfContractPeriod();
	void displayContractInfoIsNotFilled();
	void displayCustomerListInvalid();
	void displayContractPeriodOutOfDate( std::string additionalText = "" );
	void displayTenderMustExist();
	void displayCustomerGroupIDNotValid();
	void displayCustomerNotSelected();
    void displayTenderStateWarning( basar::VarString );

private slots:
	void OnSaveClicked();
	void OnCloseClicked();
	void OnSaveItemClicked();
	void OnSaveAndNewItemClicked();
	void OnCancelModifyItemClicked();
	void OnModifyItemClicked();
	void OnDeleteItemClicked();
	void OnNewItemClicked();
	void OnTblItemsDataSelectionChanged();
	void OnTblDeliveriesDataSelectionChanged();
	void OnStartSearch();
	void OnCheckCustomer();
	void OnCheckPartner();

    void OnInputCheckCodeQtyValid();
    void OnInputCheckCodeQtyInvalid();

	void OnAddDeliveryClicked();
    void OnCreateOrdersClicked();
	void OnModifyDeliveryClicked();
	void OnDeleteDeliveryClicked();
	void OnCloneDeliveryClicked();
	void OnTabChanged( int index );

protected:
	virtual void reject();

private:
	void wireEvents();
	void initTenderPositionLabels();
	void initDeliveryLabels();
	void initDeliveryPositionLabels();
    void setupTenderPositionTable();
	void setupDeliveryTable();
	void setupDeliveryPositionTable();
	basar::Int32 initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );
	basar::Int32 initDeliveriesMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );

    void resetTenderPositionForCH();
    void resetTenderPositionForOthers();
	bool isDeliveryModifiable( basar::db::aspect::AccessorPropertyTable_YIterator deliveryYit );
	bool isContractDateValid();
	bool isStateClosed();
	bool isItemSelected();
	

	Ui::TenderMaintainVC									ui;
	tender::viewConn::ITenderContractInfo*					m_uiContractInfo;
    tender::viewConn::ITenderArticleInfo*					m_uiArticleInfo;
	const log4cplus::Logger									m_Logger;
	libutil::misc::Flag										m_EventsWiring;
	QStringList												m_Labels;
	std::list<basar::VarString>								m_OriginalAttributes;
	QStringList												m_DeliveryLabels;
	std::list<basar::VarString>								m_DeliveryOriginalAttributes;
	QStringList												m_DeliveryPositionLabels;
	std::list<basar::VarString>								m_DeliveryPositionOriginalAttributes;
	basar::gui::tie::MatcherRef								m_TenderPositionMatcher;
	QAction *												m_StartSearchAction;
	basar::gui::tie::MatcherRef								m_ScheduledDeliveryCollectionMatcher;
	bool													m_AreDeliveriesLoaded;
	bool													m_POPFunctionality;
	bool													m_CloneDeliveriesFlag;
    basar::Int16                                            m_Area;
};

} // end namespace viewConn
} // end namespace tender

#endif
