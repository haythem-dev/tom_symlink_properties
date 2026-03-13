#ifndef GUARD_CSC_BATCHES_VIEWCONN_TRANSFERORDERPOSVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_TRANSFERORDERPOSVC_H

#include "itransferorderposvc.h"
#include <libbasarguitie_iviewconnector.h>

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include <QtWidgets/QDialog>
#include "ui_transferorderposvc.h"
#pragma warning (pop)

#include "iuserrightsptr.h"
#include <libbasarguitie_matcher.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libutil/flag.h>

namespace CSC_Batches
{
namespace viewConn
{

class TransferOrderPosVC : public QDialog, public ITransferOrderPosVC, public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	TransferOrderPosVC( QWidget *parent = 0 );
    ~TransferOrderPosVC();

    void                                                                injectUserRights( infrastructure::rights::IUserRightsPtr );

    // from ITransferOrderPosVC interface
	virtual void                                                        init( basar::db::aspect::AccessorPropertyTableRef orderPositions, bool isReserverdQtyDifferent, bool isOrderViewActive);
	virtual basar::gui::tie::WidgetReturnEnum	                        show();
	virtual void								                        hide(){}
    virtual void                                                        waitForEvents();
    virtual bool								                        shutdown();

	virtual void                                                        showFileStillOpenedInfo();
    virtual void                                                        info( const basar::I18nString& );
    virtual void                                                        error( const basar::I18nString& );
    virtual const basar::gui::tie::MessageBoxButtonEnum                 question( const basar::I18nString& );
    virtual void                                                        confirmOrderSplit( basar::Int32 orderno, basar::Int32 splittedPositions );

    virtual void														matchFromTransferOrder( basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual void														matchFromTransferOrderPositions();
    virtual void                                                        matchFromPharmacy( basar::db::aspect::AccessorPropertyTable_YIterator );

	virtual basar::db::aspect::AccessorPropertyTable_YIterator          getSelectedOrderPosition();
    virtual void                                                        setDeletePositionAllowed( bool );
    virtual void                                                        setSplitRedYellowAllowed( bool );
    virtual void                                                        setSplitNarcoticsRefrigeratedAllowed( bool );

private slots:
	void                                                                onTableDataSelectionChanged();
	void                                                                onBtnPlaceOrderWishClicked();
    void                                                                onBtnPlaceOrderWishShortfallClicked();
	void                                                                onBtnPrintOrderClicked();
	void														        onBtnCloseClicked();
	void														        onBtnStatusBVOClicked();
	void                                                                onBtnCancelOrderPosClicked();
	void                                                                onBtnSplitRedYellowClicked();
    void                                                                onBtnSplitNarcoticsRefrigeratedClicked();

private:
	void                                                                setupPermissions();
	void                                                                closeEvent( QCloseEvent* );
	void                                                                wireEvents();
    void                                                                initHeaderLabels( std::list< basar::VarString >& );
    void														        initMatcher( basar::db::aspect::AccessorPropertyTableRef, 
                                                                                     const std::list< basar::VarString >& );
	void                                                                clearTableWidget();
	void                                                                setupTableWidget();
	void                                                                adjustTableSize();
    void														        adjustTableTextAlignment();
    void														        labelWindowTitle();
	void                                                                setupOrderFrame();
	basar::Int32                                                        countPositionsWithColor( basar::Int16 color );
    basar::Int32                                                        countPositionsWithNarcoticsRefrigerated();
    basar::Int32                                                        countOrderablePositions();
    bool                                                                isOrderable( const basar::db::aspect::AccessorPropertyTable_YIterator ) const;
    bool                                                                isPositionCancelable();
    void                                                                setSendBWEnabled();
    void                                                                enableBWButtons();
    basar::VarString                                                    getCountryGroup();
    void                                                                handleCountryBasedColumns();
    void                                                                ValidateAndSetPositionsStatus();

private:
	Ui::TransferOrderPosVCClass ui;
	log4cplus::Logger                                                   m_Logger;
    libutil::misc::Flag                                                 m_EventsWiring;
    basar::gui::tie::MatcherRef                                         m_OrderPosMatcher;
    infrastructure::rights::IUserRightsPtr                              m_UserRights;
    bool                                                                m_IsReserveQtyDifferent;
    bool                                                                m_IsOrderViewActive;
};

} // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_TRANSFERORDERPOSVC_H
