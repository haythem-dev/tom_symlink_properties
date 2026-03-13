//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "transferorderposvc.h"

#include "definitions_gui.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"
#include "trafficlightcolorsenum.h"

#include "tablecolortransformer.h"
#include "articleattributetransformer.h"
#include "flagtransformer.h"
#include "itemcolorformatter.h"
#include "mainorderdatetransformer.h"
#include "discountflags_transformer.h"
#include "iuserrights.h"

#include "libabbauw/libabbauw_properties_definitions.h"

#include <libbasarlogin_manager.h>
#include <libbasarqtwidget_dateedit.h>
#include <QtWidgets/QHeaderView>
#include "waitcursor.h"

#include <libutil/misc/datetime/datetime.h>
#include <libutil/viewconn.h>

namespace CSC_Batches
{
namespace viewConn
{

TransferOrderPosVC::TransferOrderPosVC(QWidget *parent)
	: QDialog(parent), m_Logger(  libabbauw::LoggerPool::loggerViewConn ), m_EventsWiring( "TransferOrderPosVC::EventsWiring" )
{
	ui.setupUi( this );
	Qt::WindowFlags flags = windowFlags() | Qt::WindowMinMaxButtonsHint;
	setWindowFlags( flags );
	wireEvents();
	labelWindowTitle();
	setupTableWidget();
	setupOrderFrame();

	ui.twOrderPositions->setItemDelegate( new ItemColorFormatter( this, 0 ) );
	ui.twOrderPositions->setItemDelegate( new ItemColorFormatter( this, 20 ) );
}

TransferOrderPosVC::~TransferOrderPosVC()
{
}

void TransferOrderPosVC::injectUserRights( infrastructure::rights::IUserRightsPtr userRights )
{
    m_UserRights = userRights;
}

basar::VarString TransferOrderPosVC::getCountryGroup()
{
	basar::login::Manager::getInstance();
	basar::Int16 branchNo = basar::login::Manager::getInstance().getAreaID();
	// get user´s branch he logged in for to get area-specific database
	basar::I18nString branchStr;
	branchStr.format("%d", branchNo);
	basar::ConstString UADM_GROUP = "GROUP";
	// get GROUP parameter
	basar::I18nString group = "";
	basar::login::Manager::getInstance().getParaValue(branchStr, UADM_GROUP, group);
	return group;
}

void TransferOrderPosVC::handleCountryBasedColumns()
{
	basar::VarString GROUP_STR_SWITZERLAND = "CH";
	basar::VarString GROUP_STR_SWITZERLAND_PREWHOLE = "PCH";
	basar::VarString group = getCountryGroup();

	if (!(GROUP_STR_SWITZERLAND == group || GROUP_STR_SWITZERLAND_PREWHOLE == group))
	{
		ui.twOrderPositions->hideColumn(0);    // column in positions table for position status
		ui.twOrderPositions->hideColumn(22);   // column in positions table for Discount Calc From Flag
		ui.twOrderPositions->hideColumn(23);   // column in positions table for Discount Apply To Flag
	}
}

void TransferOrderPosVC::ValidateAndSetPositionsStatus()
{
	// only for switzerland AGP and GEP and discount flags exists to be validated
	basar::VarString GROUP_STR_SWITZERLAND = "CH";
	basar::VarString GROUP_STR_SWITZERLAND_PREWHOLE = "PCH";
	basar::VarString group = getCountryGroup();

	if (((GROUP_STR_SWITZERLAND == group) || (GROUP_STR_SWITZERLAND_PREWHOLE == group)) && (m_OrderPosMatcher.isNull() == false))
	{
		basar::db::aspect::AccessorPropertyTableRef positions = m_OrderPosMatcher.getRight();
		if (positions.isNull() == false)
		{
			int rowindex = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit;
			for (yit = positions.begin(); yit != positions.end(); yit++)
			{
				bool valid = true;
				int dcf = 0; // discount calculated from flag
				int dat = 0; // discount applied to flag
				double agp = 0.0; // amedis group price = agp
				double gep = 0.0; // grosso einkauf price = gep
				double pct = 0.0; // manual discount for a tender position
				if ((yit.isContainedAndSet(libabbauw::properties::DISCOUNTCALCFROM) == false) ||
					(yit.isContainedAndSet(libabbauw::properties::DISCOUNTAPPLYTO) == false) ||
					(yit.isContainedAndSet(libabbauw::properties::AGP) == false) ||
					(yit.isContainedAndSet(libabbauw::properties::GEP) == false) ||
					(yit.isContainedAndSet(libabbauw::properties::DISCOUNTPCT) == false))
				{
					valid = false;
				}
				if (valid == true)
				{
					// get the values
					dcf = yit.getInt16(libabbauw::properties::DISCOUNTCALCFROM);
					dat = yit.getInt16(libabbauw::properties::DISCOUNTAPPLYTO);
					agp = yit.getDecimal(libabbauw::properties::AGP).toFloat64();
					gep = yit.getDecimal(libabbauw::properties::GEP).toFloat64();
					pct = yit.getDecimal(libabbauw::properties::DISCOUNTPCT).toFloat64();
					// get the table column for the position status
					QTableWidgetItem* pItem = ui.twOrderPositions->item(rowindex, 0);
					if (pItem != NULL)
					{
						if (pct != 0.00)
						{
							if (agp > 0.00)
							{
								if ((agp < gep) && (dcf == 0) && (dat == 1))
								{
									// this combination is not valid
									QIcon icon(":/widget/status_exclamation.png");
									if (pItem != NULL)
									{
										pItem->setIcon(icon);
										pItem->setToolTip(tr("Attention: Discount based on GEP will be applied on AGP, but current GEP is higher than AGP!"));
									}
								}
								else if ((gep < agp) && (dat == 0) && (dcf == 1))
								{
									// this combination is not valid
									QIcon icon(":/widget/status_exclamation.png");
									if (pItem != NULL)
									{
										pItem->setIcon(icon);
										pItem->setToolTip(tr("Attention: Discount based on AGP will be applied on GEP, but current AGP is higher than GEP!"));
									}
								}
								else
								{
									// this combination is valid
									QIcon icon(":/widget/status_okay.png");
									if (pItem != NULL)
									{
										pItem->setIcon(icon);
										pItem->setToolTip("");
									}
								}
							}
							else
							{
								if ((dcf == 1) || (dat == 1))
								{
									// use of AGP in discount flags, but AGP is zero
									QIcon icon(":/widget/status_exclamation.png");
									if (pItem != NULL)
									{
										pItem->setIcon(icon);
										pItem->setToolTip(tr("Attention: Use of AGP in discount flags, but current AGP is zero!"));
									}
								}
							}
						}
					}
				}
				rowindex++;
			}
		}
	}
}

void TransferOrderPosVC::init( basar::db::aspect::AccessorPropertyTableRef orderpositions, bool isReservedQtyDifferent, bool isOrderViewActive )
{
    std::list< basar::VarString > listAttributes;
    initHeaderLabels( listAttributes );
	initMatcher( orderpositions, listAttributes );

    m_IsReserveQtyDifferent = isReservedQtyDifferent;
    m_IsOrderViewActive =     isOrderViewActive;

    if(m_IsOrderViewActive)
    {
        ui.lblView->setText( tr("Order View") );
    }
    else
    {
        ui.lblView->setText( tr("Release View") );
    }

    setSendBWEnabled();
	handleCountryBasedColumns();
}

void TransferOrderPosVC::setupPermissions()
{
    const bool enabled = m_UserRights->isUserAllowedToChangeTransferOrders();
    const bool placeOrderWishEnabled = m_UserRights->isUserAllowedToPlaceOrderWishes();
	
    ui.btnCancelOrderPos->setEnabled( ui.twOrderPositions->rowCount() > 0 && ui.btnCancelOrderPos->isEnabled() && enabled );
	ui.btnSplitRedYellow->setEnabled( ui.twOrderPositions->rowCount() > 0 && ui.btnSplitRedYellow->isEnabled() && enabled );
    ui.btnSplitNarcoticsRefrigerated->setEnabled( ui.twOrderPositions->rowCount() > 0 && ui.btnSplitNarcoticsRefrigerated->isEnabled() && enabled );
	ui.btnPlaceOrderWish->setEnabled( ui.twOrderPositions->rowCount() > 0 && ui.btnPlaceOrderWish->isEnabled() && enabled && placeOrderWishEnabled );
    ui.btnPlaceOrderWishShortfall->setEnabled( ui.twOrderPositions->rowCount() > 0 && ui.btnPlaceOrderWishShortfall->isEnabled() && enabled && placeOrderWishEnabled );
}

void TransferOrderPosVC::wireEvents()
{
    if( m_EventsWiring.isSet() ){ return; }
    
	connect( ui.btnClose,                          SIGNAL( clicked()                                  ),
	         this,                                 SLOT  ( onBtnCloseClicked() )                      );
	connect( ui.btnPrintOrder,                     SIGNAL( clicked()                                  ),
	         this,                                 SLOT  ( onBtnPrintOrderClicked() )                 );
	connect( ui.btnStatusOR,                       SIGNAL( clicked()                                  ),
	         this,                                 SLOT  ( onBtnStatusBVOClicked() )                  );
	connect( ui.btnPlaceOrderWish,                 SIGNAL( clicked()                                  ),
	         this,                                 SLOT  ( onBtnPlaceOrderWishClicked() )             );
    connect( ui.btnPlaceOrderWishShortfall,        SIGNAL( clicked()                                  ),
	         this,                                 SLOT  ( onBtnPlaceOrderWishShortfallClicked() )    );
	connect( ui.btnCancelOrderPos,                 SIGNAL( clicked()                                  ),
	         this,                                 SLOT  ( onBtnCancelOrderPosClicked() )             );
	connect( ui.btnSplitRedYellow,                 SIGNAL( clicked()                                  ),
	         this,                                 SLOT  ( onBtnSplitRedYellowClicked() )             );
    connect( ui.btnSplitNarcoticsRefrigerated,     SIGNAL( clicked()                                  ),
             this,                                 SLOT  ( onBtnSplitNarcoticsRefrigeratedClicked() ) );
	connect( ui.twOrderPositions,                  SIGNAL( dataSelectionChanged()                     ),
	         this,                                 SLOT  ( onTableDataSelectionChanged() )            );

    m_EventsWiring.set();
}

basar::gui::tie::WidgetReturnEnum TransferOrderPosVC::show()
{
	setAttribute( Qt::WA_Moved, false );
    QWidget::setWindowModality( Qt::ApplicationModal );
	QDialog::show();

	clearTableWidget();
    QApplication::setOverrideCursor( Qt::WaitCursor );
	QApplication::processEvents();
	
	return basar::gui::tie::WidgetReturnEnum();
}

void TransferOrderPosVC::waitForEvents()
{
	setAttribute( Qt::WA_Moved, false );
    QApplication::restoreOverrideCursor();
    exec();
}

bool TransferOrderPosVC::shutdown()
{
    libutil::gui::SignalBlocker signalblocker( ui.twOrderPositions );
    bool ret = close();
    QApplication::processEvents();
    return ret;
}

void TransferOrderPosVC::matchFromTransferOrder( basar::db::aspect::AccessorPropertyTable_YIterator orderYit )
{
	basar::gui::tie::MatcherRef matcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpTransferOrder, orderYit.getPropertyTable() );
	matcher.RightToLeft( matcher.getLeft().begin(), orderYit );
}

void TransferOrderPosVC::matchFromTransferOrderPositions()
{
	const basar::Int32 rowCount = static_cast<basar::Int32>(m_OrderPosMatcher.getRight().size());

	ui.twOrderPositions->clearContents();
	ui.twOrderPositions->setRowCount(rowCount);

	basar::Int32 matchedHits = m_OrderPosMatcher.RightToLeft(0, rowCount);
	ui.twOrderPositions->setRowCount(matchedHits);
	if (matchedHits > 0)
	{
		ui.lblOrderPosCount->setText(QString::number(rowCount));

		adjustTableSize();
		adjustTableTextAlignment();

		ui.twOrderPositions->setFocus();
		ui.twOrderPositions->selectRow(0);
	}

	ui.lblOrderPosCount->setVisible(matchedHits > 0);
	ui.lblOrderPosFound->setVisible(matchedHits > 0);

	ValidateAndSetPositionsStatus();
}

void TransferOrderPosVC::matchFromPharmacy( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyYit )
{
    basar::gui::tie::MatcherRef matcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpTransferOrder, pharmacyYit.getPropertyTable() );
	matcher.RightToLeft( matcher.getLeft().begin(), pharmacyYit );
    if( ! ui.lblTelephonyInfo->text().isEmpty() && ui.lblTelephonyInfo->text()[0] == '!' )
    {
        ui.lblTelephonyInfo->setText( "<font color='red'>" + ui.lblTelephonyInfo->text() );
    }

    QString hint = tr( "AE-SpID" ).append( ": " );
    if( pharmacyYit.isEnd() || pharmacyYit.getInt16( libabbauw::properties::CUSTOMERLOCKED ) > 0 )
    {
        ui.lblPharmacyHint->setText( hint + "<font color='red'>" + tr("locked" ) );
    }
    else
    {
        ui.lblPharmacyHint->setText( hint + tr("open" ) );
    }
}

void TransferOrderPosVC::showFileStillOpenedInfo()
{
	info( tr("Please close the last created PDF so you can print it again!" ).toLocal8Bit().constData() );
}

void TransferOrderPosVC::info( const basar::I18nString& text )
{
	basar::gui::tie::infoMsgBox( this, text.c_str(), tr("Info").toLocal8Bit().constData() );
}

void TransferOrderPosVC::error( const basar::I18nString& text )
{
	basar::gui::tie::criticalMsgBox( this, text.c_str(), tr("Error").toLocal8Bit().constData() );
}

const basar::gui::tie::MessageBoxButtonEnum TransferOrderPosVC::question( const basar::I18nString& text )
{
	return basar::gui::tie::questionMsgBox( this, text.c_str(), tr("Question").toLocal8Bit().constData() );
}

void TransferOrderPosVC::confirmOrderSplit( basar::Int32 orderno, basar::Int32 splittedPositions )
{
    ui.btnSplitRedYellow->setEnabled( false );
    ui.btnSplitNarcoticsRefrigerated->setEnabled( false );
	QString msg = tr( "%1 positions have been splitted into order number %2." ).arg( splittedPositions ).arg( orderno );
	info( msg.toLocal8Bit().constData() );
}

void TransferOrderPosVC::labelWindowTitle()
{
	QString windowTitle;
	windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_TRANSFERORDERPOS ).append( DLG_TITLESHORT2 );
	windowTitle.append( tr( "Transfer Order Positions" ) );
	windowTitle.append( tr( " area: " ) );
	basar::I18nString areaName = basar::login::Manager::getInstance().getAreaName();
	windowTitle.append( QString::fromLocal8Bit( areaName.c_str() ) );
	setWindowTitle( windowTitle );
}

void TransferOrderPosVC::clearTableWidget()
{
	ui.twOrderPositions->clearContents();
	ui.twOrderPositions->setRowCount( 0 );
}

void TransferOrderPosVC::initHeaderLabels( std::list< basar::VarString >& listAttributes )
{
    QStringList orderPositionsHeader;

	orderPositionsHeader.push_back( tr("Sts")								 ); // 0
	listAttributes.push_back( libabbauw::properties::POSITIONSTATUS          ); 

    orderPositionsHeader.push_back( tr("PZN")                                ); // 1
	listAttributes.push_back( libabbauw::properties::PZN                     );

    orderPositionsHeader.push_back( tr("Unit")                               ); // 2
	listAttributes.push_back( libabbauw::properties::UNIT                    );

    orderPositionsHeader.push_back( tr("Item Name")                          ); // 3
	listAttributes.push_back( libabbauw::properties::ARTICLENAME             );

    orderPositionsHeader.push_back( tr("PF")                                 ); // 4
	listAttributes.push_back( libabbauw::properties::PHARMACEUTICALFORM      );

    orderPositionsHeader.push_back( tr("PNo")                                ); // 5
	listAttributes.push_back( libabbauw::properties::POSNO                   );

    orderPositionsHeader.push_back( tr("Storage Slot")                       ); // 6
	listAttributes.push_back( libabbauw::properties::STORAGELOCATIONSLOT     );

    orderPositionsHeader.push_back( tr("Exp")                                ); // 7
	listAttributes.push_back( libabbauw::properties::EXPIRYDATE              );

    orderPositionsHeader.push_back( tr("Note")                               ); // 8 - BTM/K
    listAttributes.push_back( libabbauw::properties::NOTICE                  );

    orderPositionsHeader.push_back( tr("DPCT")                               ); // 9
	listAttributes.push_back( libabbauw::properties::DISCOUNTPCT             );

	orderPositionsHeader.push_back( tr("DTYPE")                              ); // 10
	listAttributes.push_back( libabbauw::properties::DISCOUNTTYPE            );

    orderPositionsHeader.push_back( tr("ND")                                 ); // 11
	listAttributes.push_back( libabbauw::properties::NONCHARGEDQTY           );

    orderPositionsHeader.push_back( tr("Pos Value")                          ); // 12
	listAttributes.push_back( libabbauw::properties::WHOLESALERPURCHASEPRICE );

	orderPositionsHeader.push_back( tr("Delivery note")                      ); // 13
	listAttributes.push_back( libabbauw::properties::DELIVERYNOTE			 );

    orderPositionsHeader.push_back( tr("OR Time")                            ); // 14
	listAttributes.push_back( libabbauw::properties::ORDERPROPOSALTIME       );

    orderPositionsHeader.push_back( tr("OR Qty")                             ); // 15
	listAttributes.push_back( libabbauw::properties::ORDERPROPOSALQTY        );

    orderPositionsHeader.push_back( tr("Purchase Order No")                  ); // 16
	listAttributes.push_back( libabbauw::properties::PURCHASEORDERNO         );

    orderPositionsHeader.push_back( tr("next main order")                    ); // 17
	listAttributes.push_back( libabbauw::properties::NEXT_MAIN_ORDER_DATE    );

    orderPositionsHeader.push_back( tr("successor")                          ); // 18
	listAttributes.push_back( libabbauw::properties::SUCCESSOR               );

    orderPositionsHeader.push_back( tr("OQTY")                               ); // 19
	listAttributes.push_back( libabbauw::properties::ORDEREDQTY              );

    orderPositionsHeader.push_back( tr("Short Fall Qty")                     ); // 20
	listAttributes.push_back( libabbauw::properties::SHORTFALLQTY            );

    orderPositionsHeader.push_back( tr("Short Fall Qty Acc")                 ); // 21
	listAttributes.push_back( libabbauw::properties::SHORTFALLQTYACCUMULATED );

	orderPositionsHeader.push_back( tr("DCF"));									// 22 - Discount calculated from
	listAttributes.push_back( libabbauw::properties::DISCOUNTCALCFROM        );

	orderPositionsHeader.push_back( tr("DAT"));									// 23 - Discount applied to
	listAttributes.push_back( libabbauw::properties::DISCOUNTAPPLYTO         );

    ui.twOrderPositions->setColumnCount( orderPositionsHeader.count() );
	ui.twOrderPositions->setHorizontalHeaderLabels( orderPositionsHeader );
}

void TransferOrderPosVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab, const std::list< basar::VarString >& listAttributes )
{
	m_OrderPosMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twOrderPositions, propTab );

	using namespace libabbauw::properties;
	basar::gui::tie::DecimalTransformer decimalTransformer( WHOLESALERPURCHASEPRICE, tr("Pos Value").toLocal8Bit().data(), 2 ); 
	basar::gui::tie::DecimalTransformer decimalTransformer2( DISCOUNTPCT, tr("DPCT").toLocal8Bit().data(), 2 );
	basar::gui::tie::DateTimeTransformer dateTimeTransformer( ORDERPROPOSALTIME, tr("OR Time").toLocal8Bit().data(), "dd.MM.yyyy" );
	TableColorTransformer tableColorTransformer( COLOR, tr("Short Fall Qty").toLocal8Bit().data(), ui.twOrderPositions );
    ArticleAttributeTransformer attribTransformer( NOTICE, tr("Note").toLocal8Bit().data(), ui.twOrderPositions );
    FlagTransformer flagTransformer( tr( "successor" ).toLocal8Bit().constData(), SUCCESSOR, ui.twOrderPositions, true );
    CSC_Batches::viewConn::MainOrderDateTransformer modt( NEXT_MAIN_ORDER_DATE, tr("next main order").toLocal8Bit().constData(), 
                                                          ui.twOrderPositions );
	DiscountFlagsTransformer dcfTransformer(DISCOUNTCALCFROM.getName(), tr("DCF").toLocal8Bit().constData());
	DiscountFlagsTransformer datTransformer(DISCOUNTAPPLYTO.getName(), tr("DAT").toLocal8Bit().constData());

	m_OrderPosMatcher.transformProperties( ui.twOrderPositions->horizontalHeaderLabels(), listAttributes );
	m_OrderPosMatcher.push_back( decimalTransformer    );
	m_OrderPosMatcher.push_back( decimalTransformer2   );
	m_OrderPosMatcher.push_back( tableColorTransformer );
	m_OrderPosMatcher.push_back( dateTimeTransformer   );
    m_OrderPosMatcher.push_back( attribTransformer     );
    m_OrderPosMatcher.push_back( flagTransformer       );
    m_OrderPosMatcher.push_back( modt                  );
	m_OrderPosMatcher.push_back( dcfTransformer        );
	m_OrderPosMatcher.push_back( datTransformer        );
}

void TransferOrderPosVC::setupOrderFrame()
{
	ui.lblOrderNo->setAlignment       ( Qt::AlignRight );
	ui.lblManufacturerNo->setAlignment( Qt::AlignRight );
	ui.lblPharmacyNo->setAlignment(     Qt::AlignRight );

	ui.lblManufacturerName->setAccessibleName( libabbauw::properties::MANUFACTURERNAME.getName().c_str() );
	ui.lblManufacturerNo->setAccessibleName(   libabbauw::properties::MANUFACTURERNO.getName().c_str()   );
	ui.lblOrderNo->setAccessibleName(          libabbauw::properties::ORDERNO.getName().c_str()          );
	ui.lblPharmacyName->setAccessibleName(     libabbauw::properties::PHARMACYNAME.getName().c_str()     );
	ui.lblPharmacyNo->setAccessibleName(       libabbauw::properties::PHARMACYNO.getName().c_str()       );
    ui.lblPharmacyLocation->setAccessibleName( libabbauw::properties::LOCATION.getName().c_str()         );
    ui.lblHeaderText->setAccessibleName(       libabbauw::properties::HEADERTEXT.getName().c_str()       );
    ui.lblTelephonyInfo->setAccessibleName(    libabbauw::properties::TELEPHONY_INFO.getName().c_str()   );
}

void TransferOrderPosVC::adjustTableSize()
{
	ui.twOrderPositions->setColumnWidth(  0,  20 ); // status
	ui.twOrderPositions->setColumnWidth(  1,  60 ); // 60 pzn
	ui.twOrderPositions->setColumnWidth(  2,  60 ); // 60 unit
	ui.twOrderPositions->setColumnWidth(  3, 230 ); // 230 article name
	ui.twOrderPositions->setColumnWidth(  4,  40 ); // 40 DAF
	ui.twOrderPositions->setColumnWidth(  5,  40 ); // 40 position no
	ui.twOrderPositions->setColumnWidth(  6,  70 ); // 70 storage loc
	ui.twOrderPositions->setColumnWidth(  7,  50 ); // 50 expiry
    ui.twOrderPositions->setColumnWidth(  8,  50 ); // 50 narcotics/coldchain
	ui.twOrderPositions->setColumnWidth(  9,  70 ); // 70 discount percentage
	ui.twOrderPositions->setColumnWidth( 10,  40 ); // 40 discount type
	ui.twOrderPositions->setColumnWidth( 11,  40 ); // 40 NR
	ui.twOrderPositions->setColumnWidth( 12,  80 ); // 80 position value
	ui.twOrderPositions->setColumnWidth( 13,  80 ); // 80 delivery info
	ui.twOrderPositions->setColumnWidth( 14,  80 ); // 80 orderproposal date
	ui.twOrderPositions->setColumnWidth( 15,  80 ); // 80 orderproposal amount
    ui.twOrderPositions->setColumnWidth( 16,  80 ); // 80 purchasing order no
	ui.twOrderPositions->setColumnWidth( 17,  75 ); // 75 next main order date
    ui.twOrderPositions->setColumnWidth( 18,  65 ); // 65 chain
	ui.twOrderPositions->setColumnWidth( 19,  65 ); // 65 order amount
	ui.twOrderPositions->setColumnWidth( 20,  77 ); // 77 defect amount
	ui.twOrderPositions->setColumnWidth( 21,  80 ); // 80 defect accumulated amount
	ui.twOrderPositions->setColumnWidth( 22,  30 ); // discount calc from
	ui.twOrderPositions->setColumnWidth( 23,  30 ); // discount apply to
	ui.twOrderPositions->setColumnWidth( 24,  20 ); // 40 dummy to complete the line

	// only for switzerland AGP and GEP and discount flags exists to be validated
	basar::VarString GROUP_STR_SWITZERLAND = "CH";
	basar::VarString GROUP_STR_SWITZERLAND_PREWHOLE = "PCH";
	basar::VarString group = getCountryGroup();

	if ((GROUP_STR_SWITZERLAND == group) || (GROUP_STR_SWITZERLAND_PREWHOLE == group))
	{
		ui.twOrderPositions->setColumnWidth(4, 30); // 40 DAF
		ui.twOrderPositions->setColumnWidth(5, 30); // 40 position no
		ui.twOrderPositions->setColumnWidth(7, 40); // 50 expiry
		ui.twOrderPositions->setColumnWidth(9, 60); // 70 discount percentage
		ui.twOrderPositions->setColumnWidth(11, 30); // 40 NR
		ui.twOrderPositions->setColumnWidth(12, 75); // 80 position value
		ui.twOrderPositions->setColumnWidth(13, 65); // 80 delivery info
		ui.twOrderPositions->setColumnWidth(14, 75); // 80 orderproposal date
		ui.twOrderPositions->setColumnWidth(15, 75); // 80 orderproposal amount
		ui.twOrderPositions->setColumnWidth(16, 75); // 80 purchasing order no
		ui.twOrderPositions->setColumnWidth(19, 55); // 65 order amount
		ui.twOrderPositions->setColumnWidth(21, 60); // 80 defect accumulated amount
	}

	ui.twOrderPositions->horizontalHeader()->setSectionResizeMode( 3, QHeaderView::Stretch ); // use max space for article name / description column

    ui.twOrderPositions->verticalHeader()->setSectionResizeMode( QHeaderView::Custom );
    for( int i = 0; i < ui.twOrderPositions->rowCount(); i++ )
    {
        ui.twOrderPositions->setRowHeight( i, 23 );
    }
}

void TransferOrderPosVC::setupTableWidget()
{
	ui.twOrderPositions->setSelectionBehavior( QAbstractItemView::SelectRows );
	// don't allow to edit data in table
	ui.twOrderPositions->setEditTriggers( QAbstractItemView::NoEditTriggers );
    ui.twOrderPositions->verticalHeader()->hide();
	ui.twOrderPositions->horizontalHeader()->setSectionResizeMode( QHeaderView::Fixed );
	
    // this implicitly belongs to table
	ui.lblOrderPosFound->setVisible( false );
	ui.lblOrderPosCount->setVisible( false );

	ui.twOrderPositions->show();
}

void TransferOrderPosVC::adjustTableTextAlignment()
{
	ui.twOrderPositions->setTextAlignmentColumn(  0, Qt::AlignCenter  | Qt::AlignVCenter ); // status
	ui.twOrderPositions->setTextAlignmentColumn(  1, Qt::AlignRight   | Qt::AlignVCenter ); // pzn 
	ui.twOrderPositions->setTextAlignmentColumn(  2, Qt::AlignRight   | Qt::AlignVCenter ); // unit 
	ui.twOrderPositions->setTextAlignmentColumn(  3, Qt::AlignLeft    | Qt::AlignVCenter ); // articlename
	ui.twOrderPositions->setTextAlignmentColumn(  4, Qt::AlignHCenter | Qt::AlignVCenter ); // DAF
	ui.twOrderPositions->setTextAlignmentColumn(  5, Qt::AlignHCenter | Qt::AlignVCenter ); // PNR
	ui.twOrderPositions->setTextAlignmentColumn(  6, Qt::AlignRight   | Qt::AlignVCenter ); // storage loc
    ui.twOrderPositions->setTextAlignmentColumn(  7, Qt::AlignHCenter | Qt::AlignVCenter ); // expiry
	ui.twOrderPositions->setTextAlignmentColumn(  8, Qt::AlignHCenter | Qt::AlignVCenter ); // narcotics/coldchain
	ui.twOrderPositions->setTextAlignmentColumn(  9, Qt::AlignRight   | Qt::AlignVCenter ); // discount pct
	ui.twOrderPositions->setTextAlignmentColumn( 10, Qt::AlignHCenter | Qt::AlignVCenter ); // discount type
	ui.twOrderPositions->setTextAlignmentColumn( 11, Qt::AlignRight   | Qt::AlignVCenter ); // NR
	ui.twOrderPositions->setTextAlignmentColumn( 12, Qt::AlignRight   | Qt::AlignVCenter ); // pos value
	ui.twOrderPositions->setTextAlignmentColumn( 13, Qt::AlignHCenter | Qt::AlignVCenter ); // delivery info
	ui.twOrderPositions->setTextAlignmentColumn( 14, Qt::AlignRight   | Qt::AlignVCenter ); // orderproposal date
    ui.twOrderPositions->setTextAlignmentColumn( 15, Qt::AlignRight   | Qt::AlignVCenter ); // orderproposal amount
    ui.twOrderPositions->setTextAlignmentColumn( 16, Qt::AlignHCenter | Qt::AlignVCenter ); // purchasing order no
	ui.twOrderPositions->setTextAlignmentColumn( 17, Qt::AlignHCenter | Qt::AlignVCenter ); // next main order date
	ui.twOrderPositions->setTextAlignmentColumn( 18, Qt::AlignRight   | Qt::AlignVCenter ); // chain
	ui.twOrderPositions->setTextAlignmentColumn( 19, Qt::AlignRight   | Qt::AlignVCenter ); // order amount
	ui.twOrderPositions->setTextAlignmentColumn( 20, Qt::AlignRight   | Qt::AlignVCenter ); // defect amount
	ui.twOrderPositions->setTextAlignmentColumn( 21, Qt::AlignRight   | Qt::AlignVCenter ); // defect accumulated amount
	ui.twOrderPositions->setTextAlignmentColumn( 22, Qt::AlignHCenter | Qt::AlignVCenter ); // discount calc from
	ui.twOrderPositions->setTextAlignmentColumn( 23, Qt::AlignHCenter | Qt::AlignVCenter ); // discount apply to

}

basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderPosVC::getSelectedOrderPosition()
{
	return m_OrderPosMatcher.getSelectedRight();
}

void TransferOrderPosVC::setDeletePositionAllowed( bool allowed )
{
    ui.btnCancelOrderPos->setEnabled( ! getSelectedOrderPosition().isEnd() && allowed );
    setupPermissions();
}

void TransferOrderPosVC::setSplitRedYellowAllowed( bool allowed )
{
    ui.btnSplitRedYellow->setEnabled( allowed );
}

void TransferOrderPosVC::setSplitNarcoticsRefrigeratedAllowed( bool allowed )
{
    ui.btnSplitNarcoticsRefrigerated->setEnabled( allowed );
}

void TransferOrderPosVC::setSendBWEnabled()
{
    if( !m_IsReserveQtyDifferent )
    {
        enableBWButtons();
    }
    else if( m_IsOrderViewActive )
    {
        enableBWButtons();
    }
    else
    {
        ui.btnPlaceOrderWish->setEnabled(false);
        ui.btnPlaceOrderWishShortfall->setEnabled(false);
    }
}

void TransferOrderPosVC::enableBWButtons()
{
    ui.btnPlaceOrderWishShortfall->setEnabled(countOrderablePositions() > 0);
    ui.btnPlaceOrderWish->setEnabled( isOrderable( getSelectedOrderPosition() ) );
}

basar::Int32 TransferOrderPosVC::countPositionsWithColor( basar::Int16 color )
{
	basar::db::aspect::AccessorPropertyTableRef propTab = m_OrderPosMatcher.getRight();
	basar::Int32 count = 0;

	for( basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin(); yit != propTab.end(); ++yit )
	{
		if( yit.getInt16( libabbauw::properties::COLOR ) == color )
			count++;
	}

	return count;
}

basar::Int32 TransferOrderPosVC::countPositionsWithNarcoticsRefrigerated()
{
    basar::db::aspect::AccessorPropertyTableRef propTab = m_OrderPosMatcher.getRight();
	basar::Int32 count = 0;

	for( basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin(); yit != propTab.end(); ++yit )
	{
        if( ! yit.getString( libabbauw::properties::NOTICE ).empty() )
			count++;
	}

	return count;
}

bool TransferOrderPosVC::isOrderable( const basar::db::aspect::AccessorPropertyTable_YIterator yit ) const
{
    bool orderable = false;
    if( ! yit.isEnd() )
    {
        using namespace libabbauw::domMod::transferOrder;
	    const bool redPosition        = ( yit.getInt16( libabbauw::properties::COLOR ) & RED )  == RED;
        const bool orderProposalUnset = ! yit.getDateTime( libabbauw::properties::ORDERPROPOSALTIME ).isValid();
	    const bool orderedQtySet      = yit.getInt32( libabbauw::properties::ORDEREDQTY )        > 0;
		const bool nonChargedQtySet = yit.getInt32( libabbauw::properties::NONCHARGEDQTY )        > 0;
	    const bool knownArticle       = yit.getInt32( libabbauw::properties::ARTICLENO )         > 0;

        orderable = redPosition && orderProposalUnset && (orderedQtySet || nonChargedQtySet) && knownArticle;
    }
    
    return orderable;
}

bool TransferOrderPosVC::isPositionCancelable()
{
    //It shouldn't be allowed to delete the last position with AMGE > 0,
    //if there is at least one or more positions with AMGE = 0 (has already been split)!

    basar::Int32                                       positionsWithQty     = 0;
    basar::Int32                                       positionsWithoutQty  = 0;
    basar::db::aspect::AccessorPropertyTable_YIterator yitPos               = m_OrderPosMatcher.getRight().begin();
    
    //Positions with AMGE = 0 can always be deleted!
    if( 0 == m_OrderPosMatcher.getCurrentRight().getInt32(libabbauw::properties::ORDEREDQTY) )
    {
        return true;
    }

    while( !yitPos.isEnd() )
    {
        if( 0 < yitPos.getInt32(libabbauw::properties::ORDEREDQTY) )
        {
            ++positionsWithQty;
        }
        else
        {
            ++positionsWithoutQty;
        }

        ++yitPos;
    }

    if( 1 == positionsWithQty && 0 < positionsWithoutQty )
    {
        return false;
    }

    return true;
}

basar::Int32 TransferOrderPosVC::countOrderablePositions()
{
    basar::Int32 count = 0;
    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = m_OrderPosMatcher.getRight().begin(); ! yit.isEnd(); ++yit )
    {
        if( isOrderable( yit ) )
        {
            count++;
        }
    }

    return count;
}

void TransferOrderPosVC::onTableDataSelectionChanged()
{
    setSendBWEnabled();

    basar::appl::SystemEventManager::getInstance().fire( "CheckAllowedUserActionsRequested" );
}

void TransferOrderPosVC::onBtnCancelOrderPosClicked()
{
    if( !isPositionCancelable() )
    {
        info( tr("Please delete position with AMGE = 0 first!").toLocal8Bit().constData() );
        return;
    }

    basar::db::aspect::AccessorPropertyTable_YIterator yit = m_OrderPosMatcher.getSelectedRight();
	basar::Int32 position = yit.getInt32( libabbauw::properties::POSNO );
	QString msg = tr( "Do you want to delete order position '%1'?" ).arg( position );
	
	if( yit.getInt32( libabbauw::properties::ORDERPROPOSALQTY ) > 0 )
	{
		msg = tr( "An order proposal has already been sent for order position '%1'.\nDo you really want to delete this position?" ).arg( position );
		if( yit.getDateTime( libabbauw::properties::TRANSFER_TIME ).toStrDate() == "31.12.9999" )
		{
			// date is unset -> this means that order proposal is not booked yet thus can be deleted too
			msg.append( tr( "\nThe order proposal will be deleted too!" ) );
		}
		else
		{
			msg.append( tr( "\nThe program will try to delete the order proposal too!" ) );
		}
	}
	
	if( ui.twOrderPositions->rowCount() == 1 )
	{
		msg.append( tr("After deleting this order position the order doesn't contain any more positions, thus"
                       " the order will be deleted too!\n") );
	}
	
	if( question( msg.toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
		libutil::gui::SignalBlocker signalblocker( this );
	    CSC_Batches::gui::WaitCursor waitcursor( this );
		basar::appl::SystemEventManager::getInstance().fire( "RemoveOrderPosRequested" );

        if( ui.twOrderPositions->rowCount() > 0 )
            ui.twOrderPositions->selectRow( 0 );
	}
}

void TransferOrderPosVC::closeEvent( QCloseEvent* )
{
	clearTableWidget();
}

void TransferOrderPosVC::onBtnCloseClicked()
{
	close();
}

void TransferOrderPosVC::onBtnPrintOrderClicked()
{
    libutil::gui::SignalBlocker signalblocker( this );
	CSC_Batches::gui::WaitCursor waitcursor( this );
	basar::appl::SystemEventManager::getInstance().fire( "PrintOrderRequested" );
}

void TransferOrderPosVC::onBtnPlaceOrderWishClicked()
{
	basar::db::aspect::AccessorPropertyTable_YIterator yit = m_OrderPosMatcher.getSelectedRight();

	basar::Int32 qty    = yit.getInt32( libabbauw::properties::SHORTFALLQTY );
	QString articleName = yit.getString( libabbauw::properties::ARTICLENAME ).c_str();
	if( articleName.isEmpty() )
		articleName = tr( "UNKNOWN" );

	QString msg = tr( "Place an order wish for item '%1' with quantity '%2'?" ).arg( articleName ).arg( qty );
	if( question( msg.toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
    {
	    CSC_Batches::gui::WaitCursor waitcursor( this );
		basar::appl::SystemEventManager::getInstance().fire( "PlaceOrderWishRequested" );
    }
}

void TransferOrderPosVC::onBtnPlaceOrderWishShortfallClicked()
{
    QString msg = tr( "Place an order wish for %1 red item(s)?" ).arg( countOrderablePositions() );
	if( question( msg.toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
    {
        CSC_Batches::gui::WaitCursor waitcursor( this );
        basar::appl::SystemEventManager::getInstance().fire( "PlaceOrderWishShortfallRequested" );
    }
}

void TransferOrderPosVC::onBtnStatusBVOClicked()
{
	CSC_Batches::gui::WaitCursor waitcursor( this );
	basar::appl::SystemEventManager::getInstance().fire( "ShowORStatusOrder" );
}

void TransferOrderPosVC::onBtnSplitRedYellowClicked()
{
    const basar::Int32 remainingPositions = countPositionsWithColor( libabbauw::domMod::transferOrder::GREEN );
	const basar::Int32 splittedPositions  = static_cast<basar::Int32>( m_OrderPosMatcher.getRight().size() ) - remainingPositions;

	QString msg = tr( "Split order into positions which are available for immediate delivery and a new order containing those that aren't?\n" 
		              "%1 positions will remain in the current order and %2 positions will be splitted into a new order!"
					).arg( remainingPositions ).arg( splittedPositions );
	if( question( msg.toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
	    CSC_Batches::gui::WaitCursor waitcursor( this );
		basar::appl::SystemEventManager::getInstance().fire( "SplitRedYellowRequested" );
	}
}

void TransferOrderPosVC::onBtnSplitNarcoticsRefrigeratedClicked()
{
    const basar::Int32 remainingPositions = countPositionsWithNarcoticsRefrigerated();
	const basar::Int32 splittedPositions  = static_cast<basar::Int32>( m_OrderPosMatcher.getRight().size() ) - remainingPositions;

	QString msg = tr( "Split narcotics and refrigerated positions into a new order?\n" 
		              "%1 positions will remain in the current order and %2 positions will be splitted into a new order!"
					).arg( remainingPositions ).arg( splittedPositions );
	if( question( msg.toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
	    CSC_Batches::gui::WaitCursor waitcursor( this );
        basar::appl::SystemEventManager::getInstance().fire( "SplitNarcoticsRefrigeratedRequested" );
	}
}
} // end namespace viewConn
} // end namespace CSC_Batches
