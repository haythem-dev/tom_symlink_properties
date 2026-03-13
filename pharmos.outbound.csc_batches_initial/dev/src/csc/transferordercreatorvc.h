#ifndef GUARD_CSC_BATCHES_VIEWCONN_TRANSFERORDERCREATORVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_TRANSFERORDERCREATORVC_H

#include "itransferordercreatorvc.h"
#include <libbasarguitie_iviewconnector.h>

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include <QtCore/QDateTime>
#include <QtWidgets/QDialog>
#include "ui_transferordercreatorvc.h"
#pragma warning (pop)

#include "libabbauw/iordertypecollectionparameterptr.h"

#include <libbasarguitie_matcher.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarproperty_propertydescription.h>
#include <libutil/flag.h>

namespace CSC_Batches
{
namespace viewConn
{
class TransferOrderCreatorVC : public QDialog, public ITransferOrderCreatorVC, public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	TransferOrderCreatorVC( QWidget *parent = 0 );
	~TransferOrderCreatorVC();

    void                                                                injectParameter( libabbauw::domMod::parameter::IOrderTypeCollectionParameterPtr );

    // from ITransferOrderCreatorVC interface
    void                                                                init( basar::db::aspect::AccessorPropertyTable_YIterator head, basar::db::aspect::AccessorPropertyTableRef pos );

			virtual basar::VarString                                            getCountryCode();
	virtual basar::gui::tie::WidgetReturnEnum	                        show();
	virtual void								                        hide(){}
    virtual void                                                        waitForEvents();
    virtual bool								                        shutdown();

    virtual void                                                        matchFromManufacturer( basar::db::aspect::AccessorPropertyTable_YIterator );
			virtual void                                                        matchToManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator);
    virtual void                                                        matchFromPurchaseOrderSupplier( basar::db::aspect::AccessorPropertyTable_YIterator );

			virtual void                                                        matchFromOrderNoMF(basar::db::aspect::AccessorPropertyTable_YIterator);
			virtual void                                                        matchToOrderNoMF (basar::db::aspect::AccessorPropertyTable_YIterator);

    virtual void                                                        matchFromPharmacy( basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual void                                                        matchToPharmacy( basar::db::aspect::AccessorPropertyTable_YIterator );

			virtual void                                                        matchForHeadertext(basar::db::aspect::AccessorPropertyTable_YIterator);
    virtual void                                                        matchFromArticle( basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual void                                                        matchToArticle( basar::db::aspect::AccessorPropertyTable_YIterator );

    virtual void                                                        matchFromTransferOrderHeadInPreparation();
    virtual void                                                        matchToTransferOrderHeadInPreparation();

    virtual void                                                        matchFromTransferOrderPosInPreparationCollection();

    virtual void                                                        matchFromPosition( basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual void                                                        matchToPosition( basar::db::aspect::AccessorPropertyTable_YIterator );

    virtual basar::db::aspect::AccessorPropertyTable_YIterator          getSelectedPosition();

    virtual void                                                        setNextPositionNo( basar::Int32 );
    virtual void                                                        setSavingAllowed( bool isEnabled );
    virtual void                                                        setCreatingTransferOrderAllowed( bool isEnabled );

    virtual void                                                        switchToEditingMode( bool );

    virtual void                                                        info( const basar::I18nString& );
    virtual void                                                        showOrderNoInfoMsg( const basar::Int32 );
    virtual void                                                        error( const basar::I18nString& );
    virtual const basar::gui::tie::MessageBoxButtonEnum                 question( const basar::I18nString& );

private slots:
//btn
    void                                                                onBtnSearchArticleClicked();
    void                                                                onBtnResetClicked();
    void                                                                onBtnSaveClicked();
    void                                                                onBtnCreateTransferOrder();
    void                                                                onBtnCloseClicked();
    void                                                                onBtnSearchPharmacy();
    void                                                                onBtnAddPosClicked();
    void                                                                onBtnChangePosClicked();
    void                                                                onBtnRemovePosClicked();
    void                                                                onBtnDiscardClicked();
//txt
    void                                                                onPharmacyNoChanged();
			void                                                                onOrderNoMFChanged();
			void                                                                onHeadertextChanged();
    void                                                                onArticleChanged();
    void                                                                onArticleSearchTyped( const QString & );
    void                                                                onDiscountPCTEditingFinished();
    void                                                                onPosEditCheck( const QString& );
    void                                                                onDefPaymentDtAddDays( const QString& );
    void                                                                onDiscountAllPositionsChanged(const QString& br);
	void																onDiscountAllPositionsTypeChanged(int index);
    void                                                                onDiscountAllPositionsFinished();
//cbx
    void                                                                cboOrderTypeChanged( int );
    void                                                                onDateChanged( const QDate & );

//chk
    void                                                                onDeliveryDateChecked( bool check );
    void                                                                onDeferredPaymentDateChecked( bool check );
			void                                                                onManualHeadertextChecked(bool check);

//table
    void                                                                onTableDataSelectionChanged();
    void                                                                onSelectPositionShortcut();

private:
			QString														        replaceKey(QString text, QString key, QString value);

    void                                                                closeEvent( QCloseEvent* );
    void                                                                wireEvents();
    void                                                                wireShortcuts();
    void                                                                labelWindowTitle();

    void                                                                clearHeadInputFields();
    void                                                                resetPositionLineEdits();
    void                                                                resetArticleLineEdits();

    void                                                                initHeaderLabels();
    void                                                                initOrderHeadFrame();
    void                                                                initHeadMatcher();
    void                                                                initPosMatcher( std::list<basar::VarString>& listPropTable );
    void                                                                adjustTableSize();
    void                                                                initTable();

    void                                                                setupOrderHeadFrame();
    void                                                                setupComboBoxes();
    void                                                                setupOrderPosFrame();

    void                                                                setClearBtnEnableState( );

    void                                                                changedDateToHead( const QDate&, const basar::property::PropertyDescription& );
    void                                                                correctPropertyState( const basar::property::PropertyDescription& );

    bool                                                                eventFilter( QObject* target, QEvent* event );
    void                                                                handleCountryBasedColumns();
    void                                                                ValidateAndSetPositionsStatus();

private:
    Ui::TransferOrderCreatorVCClass                                     ui;
    log4cplus::Logger                                                   m_Logger;
    libutil::misc::Flag                                                 m_EventsWiring;
    
    basar::gui::tie::MatcherRef                                         m_OrderHeadMatcher;
    basar::gui::tie::MatcherRef                                         m_OrderPosMatcher;
    
    basar::db::aspect::AccessorPropertyTable_YIterator                  m_Head;
    basar::db::aspect::AccessorPropertyTableRef                         m_Positions;
    libabbauw::domMod::parameter::IOrderTypeCollectionParameterPtr      m_OrderTypeCollectionParameter;

    QStringList                                                         m_LstLabels;
    std::list<basar::VarString>                                         m_LstOriginalAttributes;

    QString                                                             m_lastMatchCode;
    QString                                                             m_lastPZN;

	basar::VarString													m_StandardAART;
	basar::VarString													m_StandardDiscountType;
    basar::VarString                                                    m_countrygroup;
			basar::VarString                                                    m_headertext_pattern;
};

} // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_TRANSFERORDERCREATORVC_H
