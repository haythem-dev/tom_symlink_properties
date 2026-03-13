#include "cmnvc.h"
#include "batchordereditionvc.h"
#include "definitions_gui.h"

#include <libbasarqtwidget_dateedit.h>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QShortcut>
#include <QtGui/QKeySequence>

#include "tablecolortransformer.h"
#include "articleattributetransformer.h"
#include "flagtransformer.h"
#include "ignorezerotransformer.h"
#include "itemcolorformatter.h"
#include "orderrequesttransformer.h"

#include "libabbauw/iuserfiltersettings.h"
#include "libabbauw/idaystocolorparameter.h"
#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/trafficlightcolorsenum.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include "iuserrights.h"
#include <libbasarlogin_manager.h>
#include <set>

namespace CSC_Batches
{
	namespace viewConn
	{

		BatchOrderEditionVC::BatchOrderEditionVC(QWidget* parent)
			: QDialog(parent), m_Logger(libabbauw::LoggerPool::loggerViewConn), m_EventsWiring("BatchOrderEditionVC::EventsWiring"),
			m_NumberOfGreenOrders(0), m_TotalNumberOfOrders(0)
		{
			// Build up base screen for batchorderedition (Auftragsübersichtsmaske)
			ui.setupUi(this);

			Qt::WindowFlags flags = windowFlags() | Qt::WindowMinMaxButtonsHint;
			setWindowFlags(flags);
			wireEvents();
			labelWindowTitle();
			disableButtons();

			// Leaves the background color out for the colored column
			ui.twBatchOrders->setItemDelegate(new ItemColorFormatter(this, 18));
		}

		BatchOrderEditionVC::~BatchOrderEditionVC()
		{
		}

		void BatchOrderEditionVC::injectFilterParameter(libabbauw::domMod::parameter::IUserFilterSettingsPtr userFilterSettings)
		{
			m_UserFilterSettings = userFilterSettings;
		}

		void BatchOrderEditionVC::injectCalcParameter(libabbauw::domMod::parameter::IDaysToColorParameterPtr daysToColorParameter)
		{
			m_DaysToColorParameter = daysToColorParameter;
		}

		void BatchOrderEditionVC::injectUserRights(infrastructure::rights::IUserRightsPtr userRights)
		{
			m_UserRights = userRights;
		}

		// userFilterSettings: Filtersettings from UserSettingsScreen are handed over here 
		void BatchOrderEditionVC::init(basar::db::aspect::AccessorPropertyTableRef orders, bool isReservedQtyDifferent)
		{
			initHeaderLabels();
			initMatcher(orders);

			setupShortcuts();
			setupLineEdits();
			setupDateWidgets();
			setupTableWidget();
			setupComboBoxes();
			setupTableWidgetSorter();

			m_IsReserveQtyDifferent = isReservedQtyDifferent;
			setSendBWEnabled();

			adjustTableTextAlignment();
		}

		void BatchOrderEditionVC::setupPermissions()
		{
			const bool enabled = m_UserRights->isUserAllowedToChangeTransferOrders();
			const bool placeOW = m_UserRights->isUserAllowedToPlaceOrderWishes();

			ui.btnChangeSelectedOrders->setEnabled(ui.btnChangeSelectedOrders->isEnabled() && enabled);
			ui.btnReleaseGreenOrders->setEnabled(ui.btnReleaseGreenOrders->isEnabled() && enabled);
			ui.btnReleaseOrders->setEnabled(ui.btnReleaseOrders->isEnabled() && enabled);

			ui.btnSendORFiltered->setEnabled(ui.btnSendORFiltered->isEnabled() && placeOW);
			ui.btnSendORSelected->setEnabled(ui.btnSendORSelected->isEnabled() && placeOW);
		}

		// Explicit description of signal/slot concept
		void BatchOrderEditionVC::wireEvents()
		{
			if (m_EventsWiring.isSet()) { return; }

			connect(ui.btnSearch, SIGNAL(clicked()),						this, SLOT(onBtnSearchClicked()));
			connect(ui.btnClose, SIGNAL(clicked()),							this, SLOT(onBtnCloseClicked()));
			connect(ui.btnResetFilter, SIGNAL(clicked()),					this, SLOT(onBtnResetFilterClicked()));
			connect(ui.btnResetSort, SIGNAL(clicked()),						this, SLOT(onBtnResetSortClicked()));
			connect(ui.btnChangeView, SIGNAL(clicked()),					this, SLOT(onBtnChangeViewClicked()));
			connect(ui.btnChangeSelectedOrders, SIGNAL(clicked()),			this, SLOT(onBtnChangeOrderClicked()));
			connect(ui.btnChangeFilteredOrders, SIGNAL(clicked()),			this, SLOT(onBtnChangeOrderFilteredClicked()));
			connect(ui.btnSendORFiltered, SIGNAL(clicked()),				this, SLOT(onBtnSendORFilteredClicked()));
			connect(ui.btnSendORSelected, SIGNAL(clicked()),				this, SLOT(onBtnSendORSelectedClicked()));
			connect(ui.btnShowPositions, SIGNAL(clicked()),					this, SLOT(onBtnShowPositionsClicked()));
			connect(ui.btnStatusOR, SIGNAL(clicked()),						this, SLOT(onBtnStatusBVOClicked()));
			connect(ui.btnReleaseOrders, SIGNAL(clicked()),					this, SLOT(onBtnReleaseOrdersClicked()));
			connect(ui.btnReleaseGreenOrders, SIGNAL(clicked()),			this, SLOT(onBtnReleaseGreenOrdersClicked()));
			connect(ui.twBatchOrders, SIGNAL(doubleClicked(QModelIndex)),	this, SLOT(onTableDoubleClicked(QModelIndex)));
			connect(ui.twBatchOrders, SIGNAL(dataSelectionChanged()),		this, SLOT(onTableDataSelectionChanged()));

			m_EventsWiring.set();
		}

		void BatchOrderEditionVC::labelWindowTitle()
		{
			QString windowTitle;
			windowTitle.append(DLG_TITLESHORT1).append(DLG_NO_TRANSFERORDERS).append(DLG_TITLESHORT2);
			windowTitle.append(tr("Batch Transfer Orders"));
			windowTitle.append(tr(" area: "));
			basar::I18nString areaName = basar::login::Manager::getInstance().getAreaName();
			windowTitle.append(QString::fromLocal8Bit(areaName.c_str()));
			setWindowTitle(windowTitle);
		}

		void BatchOrderEditionVC::prepareDateWidgets()
		{
			QDate minDate(2000, 1, 1);
			QDate maxDate(2099, 12, 31);

			ui.dtDeliveryDateFrom->setMinimumDate(minDate);
			ui.dtDeliveryDateFrom->setMaximumDate(maxDate);
			ui.dtDeliveryDateTo->setMinimumDate(minDate);
			ui.dtDeliveryDateTo->setMaximumDate(maxDate);
			ui.dtOrderDateFrom->setMinimumDate(minDate);
			ui.dtOrderDateFrom->setMaximumDate(maxDate);
			ui.dtOrderDateTo->setMinimumDate(minDate);
			ui.dtOrderDateTo->setMaximumDate(maxDate);

			preinitDateWidgets();
		}

		// Set calendar values for filter from user settings screen
		void BatchOrderEditionVC::preinitDateWidgets()
		{
			basar::Date startDate = m_UserFilterSettings->getFilterStartDate();
			basar::Date endDate = m_UserFilterSettings->getFilterEndDate();

			QDate fromDate(startDate.getYear(), startDate.getMonth(), startDate.getDay());
			QDate toDate(endDate.getYear(), endDate.getMonth(), endDate.getDay());

			ui.dtDeliveryDateFrom->setDate(fromDate);
			ui.dtDeliveryDateTo->setDate(toDate);
			ui.dtOrderDateFrom->setDate(fromDate);
			ui.dtOrderDateTo->setDate(toDate);

			ui.dtDeferredPaymentDateTo->setDate(QDate(2099, 12, 31));
		}

		void BatchOrderEditionVC::disableButtons()
		{
			// disable all buttons which operate on selected orders by default
			ui.btnReleaseOrders->setEnabled(false);
			ui.btnReleaseGreenOrders->setEnabled(false);
			ui.btnShowPositions->setEnabled(false);
			ui.btnChangeSelectedOrders->setEnabled(false);
		}

		void BatchOrderEditionVC::onClearSelectionShortcut()
		{
			ui.twBatchOrders->clearSelection();
		}

		// For CTRL+G function: Select visible green orders
		void BatchOrderEditionVC::onSelectGreenOrdersShortcut()
		{
			ui.twBatchOrders->clearSelection();

			for (int i = 0; i < ui.twBatchOrders->rowCount(); ++i)
			{
				// QColor(47, 176, 25) equals green color - keep in sync with tablecolortransformer!
				if (ui.twBatchOrders->item(i, 19)->background() == QColor(47, 176, 25))
					ui.twBatchOrders->selectRow(i);
			}
		}

		// Connects Shortcuts with software function
		void BatchOrderEditionVC::setupShortcuts()
		{
			QShortcut* clearSelection = new QShortcut(QKeySequence("Ctrl+D"), ui.twBatchOrders);
			QObject::connect(clearSelection, SIGNAL(activated()), this, SLOT(onClearSelectionShortcut()));

			QShortcut* selectGreen = new QShortcut(QKeySequence("Ctrl+G"), ui.twBatchOrders);
			QObject::connect(selectGreen, SIGNAL(activated()), this, SLOT(onSelectGreenOrdersShortcut()));

			QShortcut* focusPharmacyNo = new QShortcut(QKeySequence("Alt+I"), ui.twBatchOrders);
			QObject::connect(focusPharmacyNo, SIGNAL(activated()), ui.txtPharmacyNo, SLOT(setFocus()));

			QShortcut* focusOrderNo = new QShortcut(QKeySequence("Alt+A"), ui.twBatchOrders);
			QObject::connect(focusOrderNo, SIGNAL(activated()), ui.txtOrderNoFrom, SLOT(setFocus()));

			QShortcut* focusStatus = new QShortcut(QKeySequence("Alt+S"), ui.twBatchOrders);
			QObject::connect(focusStatus, SIGNAL(activated()), ui.cboStatus, SLOT(setFocus()));

			QShortcut* focusMfName = new QShortcut(QKeySequence("Alt+H"), ui.twBatchOrders);
			QObject::connect(focusMfName, SIGNAL(activated()), ui.txtManufacturerName, SLOT(setFocus()));

			QShortcut* selectFirstRow = new QShortcut(Qt::Key_Home, ui.twBatchOrders);
			QObject::connect(selectFirstRow, SIGNAL(activated()), this, SLOT(onKeyHome()));

			QShortcut* selectLastRow = new QShortcut(Qt::Key_End, ui.twBatchOrders);
			QObject::connect(selectLastRow, SIGNAL(activated()), this, SLOT(onKeyEnd()));
		}

		void BatchOrderEditionVC::setupTableWidget()
		{
			ui.twBatchOrders->setSelectionBehavior(QAbstractItemView::SelectRows);
			// don't allow to edit data in table
			ui.twBatchOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);
			ui.twBatchOrders->verticalHeader()->hide();

			ui.twBatchOrders->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

			ui.lblOrderFound->setVisible(false);
			ui.lblOrderCount->setVisible(false);

			ui.twBatchOrders->show();
		}

		void BatchOrderEditionVC::setupDateWidgets()
		{
			// Match data from database to controls (BASAR)
			ui.dtOrderDateFrom->setAccessibleName(libabbauw::properties::ORDERDATE_FROM.getName().c_str());
			ui.dtOrderDateTo->setAccessibleName(libabbauw::properties::ORDERDATE_TO.getName().c_str());
			ui.dtDeliveryDateFrom->setAccessibleName(libabbauw::properties::EDIDELIVERYDATE_FROM.getName().c_str());
			ui.dtDeliveryDateTo->setAccessibleName(libabbauw::properties::EDIDELIVERYDATE_TO.getName().c_str());
			ui.dtDeferredPaymentDateFrom->setAccessibleName(libabbauw::properties::DEFERREDPAYMENTDATE_FROM.getName().c_str());
			ui.dtDeferredPaymentDateTo->setAccessibleName(libabbauw::properties::DEFERREDPAYMENTDATE_TO.getName().c_str());
		}

		void BatchOrderEditionVC::setupLineEdits()
		{
			// allow only integers in these line edit controls
			ui.txtPZN->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,8}"), ui.txtPZN));
			ui.txtMno->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,6}"), ui.txtMno));
			ui.txtPharmacyNo->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,7}"), ui.txtPharmacyNo));
			ui.txtOrderNoFrom->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), ui.txtOrderNoFrom));
			ui.txtOrderNoTo->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), ui.txtOrderNoTo));
			ui.txtPositionsFrom->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), ui.txtPositionsFrom));
			ui.txtPositionsTo->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), ui.txtPositionsTo));
			ui.txtManufacturerName->setValidator(new QRegExpValidator(QRegExp("[.\\w, ]*"), ui.txtManufacturerName));
			ui.txtPharmacyName->setValidator(new QRegExpValidator(QRegExp("[.\\w, ]*"), ui.txtPharmacyName));
			ui.txtLocation->setValidator(new QRegExpValidator(QRegExp("[.\\w, ]*"), ui.txtPharmacyName));
			ui.txtDaysToColor->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,3}"), ui.txtDaysToColor));

			// allow only decimal values in these line edit controls
			ui.txtOrderValueFrom->setValidator(new QDoubleValidator(0.0, 99999999.0, 2, ui.txtOrderValueFrom));
			ui.txtOrderValueTo->setValidator(new QDoubleValidator(0.0, 99999999.0, 2, ui.txtOrderValueTo));

			// Setup connection between Basar and textboxes for matching 
			using namespace libabbauw::properties;
			ui.txtMno->setAccessibleName(MANUFACTURERNO.getName().c_str());
			ui.txtManufacturerName->setAccessibleName(MANUFACTURERNAME.getName().c_str());
			ui.txtPZN->setAccessibleName(PZN.getName().c_str());
			ui.txtPharmacyNo->setAccessibleName(PHARMACYNO.getName().c_str());
			ui.txtPharmacyName->setAccessibleName(PHARMACYNAME.getName().c_str());
			ui.txtLocation->setAccessibleName(LOCATION.getName().c_str());
			ui.txtOrderNoFrom->setAccessibleName(ORDERNO_FROM.getName().c_str());
			ui.txtOrderNoTo->setAccessibleName(ORDERNO_TO.getName().c_str());
			ui.txtPositionsFrom->setAccessibleName(POS_FROM.getName().c_str());
			ui.txtPositionsTo->setAccessibleName(POS_TO.getName().c_str());
			ui.txtOrderValueFrom->setAccessibleName(ORDERVALUE_FROM.getName().c_str());
			ui.txtOrderValueTo->setAccessibleName(ORDERVALUE_TO.getName().c_str());

			resetDaysToColor();
		}

		void BatchOrderEditionVC::setupComboBoxes()
		{
			using libabbauw::domMod::parameter::IOrderTypeCollectionParameter;
			IOrderTypeCollectionParameter::OrderTypeCollection orderTypeColl = m_UserFilterSettings->getOrderTypesForFiltering();

			// fill order type filter combobox
			ui.cboOrderType->addItem("");

			// SR-24100195 
			std::set<std::string> sortedItems;

			for (IOrderTypeCollectionParameter::OrderTypeCollection::const_iterator it = orderTypeColl.begin(); it != orderTypeColl.end(); ++it)
			{
				sortedItems.insert(it->c_str());
			}

			std::set<std::string>::iterator its;

			// handle "UW" first
			for (its = sortedItems.begin(); its != sortedItems.end(); ++its)
			{
				std::string value = its->c_str();
				// take all starting with "UW"
				if (value.rfind("UW", 0) == 0)
					ui.cboOrderType->addItem(its->c_str());
			}

			// now add the other ones
			for (its = sortedItems.begin(); its != sortedItems.end(); ++its)
			{
				std::string value = its->c_str();
				// take all not starting with "UW"
				if (value.rfind("UW", 0) != 0)
					ui.cboOrderType->addItem(its->c_str());
			}

			// TODO: remove hard coded values
			ui.cboStatus->addItem("");
			ui.cboStatus->addItem("ZU");
			ui.cboStatus->addItem("FE");
			ui.cboStatus->addItem("ER");
			ui.cboStatus->addItem("ST");

			ui.cboNonStockItem->addItem("");
			//ui.cboNonStockItem->addItem( tr("only NSP"), QVariant( true ) );
			ui.cboNonStockItem->addItem("nur KL", QVariant(true));
			//ui.cboNonStockItem->addItem( tr("without NSP"), QVariant( false ) );
			ui.cboNonStockItem->addItem("ohne KL", QVariant(false));

			ui.cboLargeScaleOrder->addItem("");
			//ui.cboLargeScaleOrder->addItem( tr("only LSO"), QVariant( true ) );
			ui.cboLargeScaleOrder->addItem("nur GM", QVariant(true));
			//ui.cboLargeScaleOrder->addItem( tr("without LSO"), QVariant( false ) );
			ui.cboLargeScaleOrder->addItem("ohne GM", QVariant(false));
		}

		void BatchOrderEditionVC::setupTableWidgetSorter()
		{
			m_TableWidgetSorter.setTableWidget(ui.twBatchOrders);
			m_TableWidgetSorter.setListAttributes(m_ListAttributes);

			m_TableWidgetSorter.addSortableColumn(libabbauw::properties::MANUFACTURERNO);
			m_TableWidgetSorter.addSortableColumn(libabbauw::properties::PHARMACYNO);
			m_TableWidgetSorter.addSortableColumn(libabbauw::properties::ORDERNO);
			m_TableWidgetSorter.addSortableColumn(libabbauw::properties::STATUS);
			m_TableWidgetSorter.addSortableColumn(libabbauw::properties::ORDERDATE);
			m_TableWidgetSorter.addSortableColumn(libabbauw::properties::EDIDELIVERYDATE);
			m_TableWidgetSorter.addSortableColumn(libabbauw::properties::DEFERREDPAYMENTDATE);
			m_TableWidgetSorter.addSortableColumn(libabbauw::properties::ORDERVALUE);
		}

		void BatchOrderEditionVC::clearLineEditFilters()
		{
			ui.txtPZN->clear();
			ui.txtManufacturerName->clear();
			ui.txtMno->clear();
			ui.txtOrderNoFrom->clear();
			ui.txtOrderNoTo->clear();
			ui.txtOrderValueFrom->clear();
			ui.txtOrderValueTo->clear();
			ui.txtPharmacyName->clear();
			ui.txtPharmacyNo->clear();
			ui.txtLocation->clear();
			ui.txtPositionsFrom->clear();
			ui.txtPositionsTo->clear();
		}

		void BatchOrderEditionVC::clearComboBoxes()
		{
			ui.cboOrderType->setCurrentIndex(0);
			ui.cboStatus->setCurrentIndex(0);
			ui.cboNonStockItem->setCurrentIndex(0);
			ui.cboLargeScaleOrder->setCurrentIndex(0);
		}

		void BatchOrderEditionVC::clearTableWidget()
		{
			ui.twBatchOrders->clearContents();
			ui.twBatchOrders->setRowCount(0);
			adjustTableSize();
		}

		void BatchOrderEditionVC::initHeaderLabels()
		{
			QStringList batchOrdersHeaders;

			batchOrdersHeaders.push_back(tr("MNo"));
			m_ListAttributes.push_back(libabbauw::properties::MANUFACTURERNO);

			batchOrdersHeaders.push_back(tr("Manufacturer Name"));
			m_ListAttributes.push_back(libabbauw::properties::MANUFACTURERNAME);

			batchOrdersHeaders.push_back(tr("Pharmacy No"));
			m_ListAttributes.push_back(libabbauw::properties::PHARMACYNO);

			batchOrdersHeaders.push_back(tr("Pharmacy Name"));
			m_ListAttributes.push_back(libabbauw::properties::PHARMACYNAME);

			batchOrdersHeaders.push_back(tr("Location"));
			m_ListAttributes.push_back(libabbauw::properties::LOCATION);

			batchOrdersHeaders.push_back(tr("CI")); // Customer info
			m_ListAttributes.push_back(libabbauw::properties::HAS_TELEPHONY_INFO);

			batchOrdersHeaders.push_back(tr("Order No"));
			m_ListAttributes.push_back(libabbauw::properties::ORDERNO);

			batchOrdersHeaders.push_back(tr("Order Type"));
			m_ListAttributes.push_back(libabbauw::properties::ORDERTYPE);

			batchOrdersHeaders.push_back(tr("Status"));
			m_ListAttributes.push_back(libabbauw::properties::STATUS);

			batchOrdersHeaders.push_back(tr("Order Date"));
			m_ListAttributes.push_back(libabbauw::properties::ORDERDATE);

			batchOrdersHeaders.push_back(tr("Delivery Date"));
			m_ListAttributes.push_back(libabbauw::properties::EDIDELIVERYDATE);

			batchOrdersHeaders.push_back(tr("Def. Payment Date"));
			m_ListAttributes.push_back(libabbauw::properties::DEFERREDPAYMENTDATE);

			batchOrdersHeaders.push_back(tr("Note")); // BTM/K
			m_ListAttributes.push_back(libabbauw::properties::NOTICE);

			//batchOrdersHeaders.push_back( tr("NSI")                                 );
			batchOrdersHeaders.push_back("KL");
			m_ListAttributes.push_back(libabbauw::properties::NONSTOCKITEM);

			batchOrdersHeaders.push_back(tr("OR"));
			m_ListAttributes.push_back(libabbauw::properties::ORDERREQUEST);

			batchOrdersHeaders.push_back(tr("Order Value"));
			m_ListAttributes.push_back(libabbauw::properties::ORDERVALUE);

			//batchOrdersHeaders.push_back( tr("LSO")                                 );
			batchOrdersHeaders.push_back("GM");
			m_ListAttributes.push_back(libabbauw::properties::LARGESCALEORDER);

			batchOrdersHeaders.push_back(tr("Positions"));
			m_ListAttributes.push_back(libabbauw::properties::POSQTY);

			batchOrdersHeaders.push_back(tr("Short Fall Pos"));
			m_ListAttributes.push_back(libabbauw::properties::TRAFFIC_LIGHT_INFO);

			ui.twBatchOrders->setColumnCount(batchOrdersHeaders.count());
			ui.twBatchOrders->setHorizontalHeaderLabels(batchOrdersHeaders);
		}

		void BatchOrderEditionVC::adjustTableSize()
		{
			int i = 0;
			ui.twBatchOrders->setColumnWidth(i++, ui.line1->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line2->x() - ui.line1->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line3->x() - ui.line2->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line4->x() - ui.line3->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line5->x() - ui.line4->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line6->x() - ui.line5->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line7->x() - ui.line6->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line8->x() - ui.line7->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line9->x() - ui.line8->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line10->x() - ui.line9->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line11->x() - ui.line10->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line12->x() - ui.line11->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line13->x() - ui.line12->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line14->x() - ui.line13->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line15->x() - ui.line14->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line16->x() - ui.line15->x());
			ui.twBatchOrders->setColumnWidth(i++, ui.line17->x() - ui.line16->x());
			ui.twBatchOrders->setColumnWidth(i++, (ui.line18->x() - ui.line17->x()) + 3);

			ui.twBatchOrders->horizontalHeader()->setStretchLastSection(true);	// auto adjust last column

			ui.twBatchOrders->verticalHeader()->setSectionResizeMode(QHeaderView::Custom);
			for (int ibo = 0; ibo < ui.twBatchOrders->rowCount(); ibo++)
			{
				ui.twBatchOrders->setRowHeight(ibo, 23);
			}
		}

		// align column layout (e.g. right aligned)
		void BatchOrderEditionVC::adjustTableTextAlignment()
		{
			ui.twBatchOrders->setTextAlignmentColumn(0, Qt::AlignRight | Qt::AlignVCenter); // MANUFACTURERNO
			ui.twBatchOrders->setTextAlignmentColumn(2, Qt::AlignRight | Qt::AlignVCenter); // PHARMACYNO
			ui.twBatchOrders->setTextAlignmentColumn(6, Qt::AlignRight | Qt::AlignVCenter); // ORDERNO
			ui.twBatchOrders->setTextAlignmentColumn(7, Qt::AlignHCenter | Qt::AlignVCenter); // ORDERTYPE
			ui.twBatchOrders->setTextAlignmentColumn(8, Qt::AlignHCenter | Qt::AlignVCenter); // DISCOUNTPCTRANGE
			ui.twBatchOrders->setTextAlignmentColumn(9, Qt::AlignHCenter | Qt::AlignVCenter); // STATUS
			ui.twBatchOrders->setTextAlignmentColumn(10, Qt::AlignHCenter | Qt::AlignVCenter); // ORDERDATE
			ui.twBatchOrders->setTextAlignmentColumn(11, Qt::AlignHCenter | Qt::AlignVCenter); // EDIDELIVERYDATE
			ui.twBatchOrders->setTextAlignmentColumn(12, Qt::AlignHCenter | Qt::AlignVCenter); // DEFERREDPAYMENTDATE
			ui.twBatchOrders->setTextAlignmentColumn(14, Qt::AlignHCenter | Qt::AlignVCenter); // NOTE
			ui.twBatchOrders->setTextAlignmentColumn(15, Qt::AlignHCenter | Qt::AlignVCenter); // ORDERREQUEST
			ui.twBatchOrders->setTextAlignmentColumn(16, Qt::AlignRight | Qt::AlignVCenter); // ORDERVALUE
			ui.twBatchOrders->setTextAlignmentColumn(17, Qt::AlignRight | Qt::AlignVCenter); // LARGESCALEORDER
			ui.twBatchOrders->setTextAlignmentColumn(18, Qt::AlignRight | Qt::AlignVCenter); // POSITIONS
			ui.twBatchOrders->setTextAlignmentColumn(19, Qt::AlignHCenter | Qt::AlignVCenter); // SHORTFALLPOS
		}

		//void BatchOrderEditionVC::adjustTableData()
		//{                                                                                           
		//	// Avoid emitting of signals while filling the list 
		//	libutil::gui::SignalBlocker signalblocker( ui.twBatchOrders );
		//	adjustTableTextAlignment();
		//}

		void BatchOrderEditionVC::initMatcher(basar::db::aspect::AccessorPropertyTableRef propTab)
		{
			m_OrdersMatcher = basar::gui::tie::Manager::getInstance().createMatcher(ui.twBatchOrders, propTab);

			basar::gui::tie::DecimalTransformer decimalTransformer(libabbauw::properties::ORDERVALUE, tr("Order Value").toLocal8Bit().data(), 2);
			TableColorTransformer tableColorTransformer(libabbauw::properties::COLOR, tr("Short Fall Pos").toLocal8Bit().data(), ui.twBatchOrders);
			ArticleAttributeTransformer attribTransformer(libabbauw::properties::NOTICE, tr("Note").toLocal8Bit().data(), ui.twBatchOrders);
			//FlagTransformer flagTransformer( tr("NSP").toLocal8Bit().data(), libabbauw::properties::NONSTOCKITEM, ui.twBatchOrders );
			FlagTransformer flagTransformer("KL", libabbauw::properties::NONSTOCKITEM, ui.twBatchOrders);
			FlagTransformer flagTransformer2(tr("CI").toLocal8Bit().data(), libabbauw::properties::HAS_TELEPHONY_INFO, ui.twBatchOrders);
			OrderRequestTransformer orTransformer2(tr("OR").toLocal8Bit().data(), libabbauw::properties::ORDERREQUEST);
			//IgnoreZeroTransformer ignoreZeroTransformer( libabbauw::properties::LARGESCALEORDER, tr("LSO").toLocal8Bit().data() );
			IgnoreZeroTransformer ignoreZeroTransformer(libabbauw::properties::LARGESCALEORDER, "GM");

			m_OrdersMatcher.transformProperties(ui.twBatchOrders->horizontalHeaderLabels(), m_ListAttributes);
			m_OrdersMatcher.push_back(decimalTransformer);
			m_OrdersMatcher.push_back(tableColorTransformer);
			m_OrdersMatcher.push_back(attribTransformer);
			m_OrdersMatcher.push_back(flagTransformer);
			m_OrdersMatcher.push_back(flagTransformer2);
			m_OrdersMatcher.push_back(orTransformer2);
			m_OrdersMatcher.push_back(ignoreZeroTransformer);
		}

		void BatchOrderEditionVC::setTableDefaultSorting()
		{
			m_TableWidgetSorter.setSortOrder(m_UserFilterSettings->getOrderSort());
		}

		void BatchOrderEditionVC::resetDaysToColor()
		{
			ui.txtDaysToColor->setText(QString::number(m_DaysToColorParameter->getDaysToColor()));
		}

		void BatchOrderEditionVC::resizeEvent(QResizeEvent* e)
		{
			adjustTableSize();
			QDialog::resizeEvent(e);
		}

		void BatchOrderEditionVC::onBtnSearchClicked()
		{
			CSC_Batches::gui::WaitCursor waitcursor(this);
			clearTableWidget();
			basar::appl::SystemEventManager::getInstance().fire("SearchBatchOrderRequested");
		}

		void BatchOrderEditionVC::closeEvent(QCloseEvent*)
		{
			onBtnResetFilterClicked();
			onBtnResetSortClicked();
		}

		void BatchOrderEditionVC::onBtnCloseClicked()
		{
			close();
		}

		void BatchOrderEditionVC::onBtnResetFilterClicked()
		{
			preinitDateWidgets();
			clearLineEditFilters();
			clearComboBoxes();
		}

		void BatchOrderEditionVC::onBtnResetSortClicked()
		{
			setTableDefaultSorting();
		}

		void BatchOrderEditionVC::onBtnShowPositionsClicked()
		{
			if (ui.twBatchOrders->selectionModel()->selectedRows().count() != 1)
			{
				info(tr("Please select exactly one order to show positions from!").toLocal8Bit().data());
				return;
			}

			CSC_Batches::gui::WaitCursor waitcursor(this);
			basar::appl::SystemEventManager::getInstance().fire("ShowOrderPositionsRequested");
		}

		void BatchOrderEditionVC::onBtnChangeViewClicked()
		{
			const bool isOrderView = ui.btnChangeView->isChecked();
			if (isOrderView)
			{
				ui.btnChangeView->setText(tr("Order View"));
			}
			else
			{
				ui.btnChangeView->setText(tr("Release View"));
			}
			ui.txtDaysToColor->setEnabled(isOrderView);
			ui.lblDeliveryDate->setEnabled(isOrderView);
			ui.lblDays->setEnabled(isOrderView);

			setSendBWEnabled();

			resetDaysToColor();
			onBtnSearchClicked();
		}

		void BatchOrderEditionVC::onBtnChangeOrderClicked()
		{
			CSC_Batches::gui::WaitCursor waitcursor(this);
			basar::appl::SystemEventManager::getInstance().fire("ChangeOrderRequested");
		}

		void BatchOrderEditionVC::onBtnChangeOrderFilteredClicked()
		{
			CSC_Batches::gui::WaitCursor waitcursor(this);
			basar::appl::SystemEventManager::getInstance().fire("ChangeOrderFilteredRequested");
		}

		void BatchOrderEditionVC::onBtnSendORSelectedClicked()
		{
			CSC_Batches::gui::WaitCursor waitcursor(this);

			if (checkIfCMIOrderSelected())
			{
				//QString msg = tr("One or more transfer orders with order type DG03 selected.\nNo order request for DG03 transfer orders allowed!");
				info("Ein oder mehrere Überweiser mit Auftragsart DG03 selektiert.\nBestellwünsche für Auftragsart DG03 sind nicht erlaubt!");
				return;
			}

			basar::appl::SystemEventManager::getInstance().fire("SendOrderRequestsSelectedRequested");
		}

		void BatchOrderEditionVC::onBtnSendORFilteredClicked()
		{
			CSC_Batches::gui::WaitCursor waitcursor(this);

			if (checkIfCMIOrderFiltered())
			{
				//info(tr("One or more transfer orders with order type DG03 filtered.\n""No order request for DG03 transfer orders allowed!").toLocal8Bit().data());
				info("Ein oder mehrere Überweiser mit Auftragart DG03 gefiltert.\n""Bestellwünsche für Auftragart DG03 sind nicht erlaubt!");
				return;
			}

			basar::appl::SystemEventManager::getInstance().fire("SendOrderRequestsFilteredRequested");
		}

		void BatchOrderEditionVC::onBtnStatusBVOClicked()
		{
			CSC_Batches::gui::WaitCursor waitcursor(this);
			basar::appl::SystemEventManager::getInstance().fire("ShowBVOStatusAll");
		}

		void BatchOrderEditionVC::onBtnReleaseOrdersClicked()
		{
			int numberOfSelectedOrders = ui.twBatchOrders->selectionModel()->selectedRows().count();
			if (numberOfSelectedOrders < 1)
				info(tr("Please select at least one order to be released!").toLocal8Bit().data());
			else
			{
				QString msg;
				if (numberOfSelectedOrders == 1)
					msg = tr("Do you want to release the selected order?");
				else
					msg = tr("Do you want to release the %1 selected orders?").arg(numberOfSelectedOrders);

				QMessageBox msgBox(QMessageBox::Question, tr("Question"), msg);
				QPushButton* releaseAllOrdersBtn = msgBox.addButton(tr("Yes"), QMessageBox::ActionRole);
				QPushButton* releaseAsDeferredOrdersBtn = msgBox.addButton(tr("Yes, but release as deferred"), QMessageBox::ActionRole);
				msgBox.addButton(tr("No"), QMessageBox::ActionRole);

				msgBox.exec();

				if (msgBox.clickedButton() == releaseAllOrdersBtn || msgBox.clickedButton() == releaseAsDeferredOrdersBtn)
				{
					// Avoid multiple release of the same orders
					ui.btnReleaseGreenOrders->setEnabled(false);
					ui.btnReleaseOrders->setEnabled(false);
					CSC_Batches::gui::WaitCursor waitcursor(this);
					if (msgBox.clickedButton() == releaseAllOrdersBtn)
					{
						basar::appl::SystemEventManager::getInstance().fire("ReleaseOrdersRequested");
					}
					else
					{
						basar::appl::SystemEventManager::getInstance().fire("ReleaseOrdersPostponeRequested");
					}
					ui.btnReleaseGreenOrders->setEnabled(true);
					ui.btnReleaseOrders->setEnabled(true);
				}
			}
		}

		void BatchOrderEditionVC::onBtnReleaseGreenOrdersClicked()
		{
			QString msg = tr("Do yout want to release %1 green orders?").arg(m_NumberOfGreenOrders);
			if (question(msg.toLocal8Bit().constData()) == basar::gui::tie::ButtonYes)
			{
				ui.btnReleaseGreenOrders->setEnabled(false);
				ui.btnReleaseOrders->setEnabled(false);
				libutil::gui::SignalBlocker signalblocker(this);
				CSC_Batches::gui::WaitCursor waitcursor(this);
				basar::appl::SystemEventManager::getInstance().fire("ReleaseGreenOrdersRequested");
				ui.btnReleaseGreenOrders->setEnabled(true);
				ui.btnReleaseOrders->setEnabled(true);
			}
		}

		void BatchOrderEditionVC::onTableDoubleClicked(QModelIndex index)
		{
			ui.twBatchOrders->setCurrentCell(index.row(), index.column(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
			basar::appl::SystemEventManager::getInstance().fire("ShowOrderPositionsRequested");
		}

		// For selecting sets of orders
		void BatchOrderEditionVC::onTableDataSelectionChanged()
		{
			bool releaseAllowed = true;

			YIterators yits = getSelectedOrders();
			bool enable = !yits.empty();          // at least one order is selected

			for (YIterators::const_iterator it = yits.begin(); it != yits.end() && releaseAllowed; ++it)
			{
				const bool purpleOrder = ((it->getInt16(libabbauw::properties::COLOR) & libabbauw::domMod::transferOrder::PURPLE) == libabbauw::domMod::transferOrder::PURPLE);
				const bool whiteOrder = (it->getInt16(libabbauw::properties::COLOR) == libabbauw::domMod::transferOrder::NO_COLOR);
				const bool isOMOrder = it->getString(libabbauw::properties::ORDERTYPE).substr(0, 2) == "OM";
				const bool status_ZU = it->getString(libabbauw::properties::STATUS) == "ZU";
				if (purpleOrder || (whiteOrder && !isOMOrder) || !status_ZU)
					releaseAllowed = false;
			}

			ui.btnReleaseOrders->setEnabled(enable & releaseAllowed);
			ui.btnShowPositions->setEnabled(enable);
			ui.btnChangeSelectedOrders->setEnabled(enable);
			setSendBWEnabled();
			setupPermissions();
		}

		void BatchOrderEditionVC::onKeyHome()
		{
			ui.twBatchOrders->selectRow(0);
		}

		void BatchOrderEditionVC::onKeyEnd()
		{
			ui.twBatchOrders->selectRow(ui.twBatchOrders->rowCount() - 1);
		}

		void BatchOrderEditionVC::matchToSearchCriteria(basar::db::aspect::AccessorPropertyTable_YIterator yitSearch)
		{
			basar::gui::tie::MatcherRef orderSearchMatcher = basar::gui::tie::Manager::getInstance().createMatcher(
				ui.grpFilter,
				yitSearch.getPropertyTable()
			);
			orderSearchMatcher.LeftToRight();

			QString text = ui.cboStatus->currentText();
			if (!text.isEmpty())
				yitSearch.setString(libabbauw::properties::STATUS, text.toLocal8Bit().constData());

			text = ui.cboOrderType->currentText();
			if (!text.isEmpty())
				yitSearch.setString(libabbauw::properties::ORDERTYPE, text.toLocal8Bit().constData());

			if (!m_TableWidgetSorter.getSortOrder().empty())
				yitSearch.setString(libabbauw::properties::SORT, m_TableWidgetSorter.getSortOrder().toString());

			if (!ui.cboNonStockItem->currentData().isNull())      // ignore first item which deaktivates filter
				yitSearch.setInt16(libabbauw::properties::NONSTOCKITEM, ui.cboNonStockItem->currentData().toBool());

			if (!ui.cboLargeScaleOrder->currentData().isNull())   // ignore first item which deaktivates filter
				yitSearch.setInt32(libabbauw::properties::LARGESCALEORDER, ui.cboLargeScaleOrder->currentData().toBool());

			if (ui.txtDaysToColor->isEnabled() && !ui.txtDaysToColor->text().isEmpty())
				yitSearch.setInt32(libabbauw::properties::DAYS_TO_COLOR, ui.txtDaysToColor->text().toInt());
		}

		void BatchOrderEditionVC::matchFromTransferOrders()
		{
			libutil::gui::SignalBlocker signalblocker(ui.twBatchOrders);

			ui.twBatchOrders->clearContents();
			ui.twBatchOrders->setRowCount(static_cast<int>(m_OrdersMatcher.getRight().size()));

			basar::Int32 matchedHits = m_OrdersMatcher.RightToLeft(0, ui.twBatchOrders->rowCount());
			ui.twBatchOrders->setRowCount(matchedHits);
			disableButtons();

			if (matchedHits > 0)
			{
				size_t count = m_OrdersMatcher.getCurrentRight().getPropertyTable().size();
				ui.lblOrderCount->setText(tr("%1 from %2").arg(static_cast<basar::Int32>(count)).arg(m_TotalNumberOfOrders));
			}

			ui.btnReleaseGreenOrders->setText(tr("Release %1 &green orders").arg(m_NumberOfGreenOrders));
			ui.btnReleaseGreenOrders->setEnabled(m_NumberOfGreenOrders > 0);
			ui.lblOrderCount->setVisible(matchedHits > 0);
			ui.lblOrderFound->setVisible(matchedHits > 0);
			ui.btnChangeFilteredOrders->setEnabled(matchedHits > 0);

			adjustTableSize();
			ui.twBatchOrders->clearSelection();
			setupPermissions();
			ui.twBatchOrders->setFocus();
		}

		basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderEditionVC::getSelectedOrder()
		{
			return m_OrdersMatcher.getSelectedRight();
		}

		BatchOrderEditionVC::YIterators BatchOrderEditionVC::getSelectedOrders()
		{
			YIterators yiterators;
			for (basar::db::aspect::AccessorPropertyTable_YIterator it = m_OrdersMatcher.getSelectedRight(); !it.isEnd(); it = m_OrdersMatcher.getNextSelectedRight())
			{
				yiterators.push_back(it);
			}

			return yiterators;
		}

		bool BatchOrderEditionVC::isOrderViewActive()
		{
			return ui.btnChangeView->isChecked();
		}

		basar::gui::tie::WidgetReturnEnum BatchOrderEditionVC::show()
		{
			setAttribute(Qt::WA_Moved, false);

			QWidget::setWindowModality(Qt::ApplicationModal);
			QDialog::show();
			clearTableWidget();
			setupPermissions();

			prepareDateWidgets();
			setTableDefaultSorting();
			QApplication::processEvents();

			return basar::gui::tie::WidgetReturnEnum();
		}

		void BatchOrderEditionVC::waitForEvents()
		{
			setAttribute(Qt::WA_Moved, false);

			exec();
		}

		bool BatchOrderEditionVC::shutdown()
		{
			m_OrdersMatcher.reset();
			return close();
		}

		void BatchOrderEditionVC::info(const basar::I18nString& text)
		{
			basar::gui::tie::infoMsgBox(this, text.c_str(), tr("Info").toLocal8Bit().constData());
		}

		void BatchOrderEditionVC::error(const basar::I18nString& text)
		{
			basar::gui::tie::criticalMsgBox(this, text.c_str(), tr("Error").toLocal8Bit().constData());
		}

		const basar::gui::tie::MessageBoxButtonEnum BatchOrderEditionVC::question(const basar::I18nString& text)
		{
			return basar::gui::tie::questionMsgBox(this, text.c_str(), tr("Question").toLocal8Bit().constData());
		}

		void BatchOrderEditionVC::showOrderChangedNotification()
		{
			QString msg = tr("This order has already been changed by someone else. Your current order view is outdated.\n"
				"Please refresh to get the correct order state!");
			info(msg.toLocal8Bit().constData());
		}

		void BatchOrderEditionVC::setNumberOfGreenOrders(basar::Int32 greenOrders)
		{
			m_NumberOfGreenOrders = greenOrders;
		}

		void BatchOrderEditionVC::setTotalNumberOfOrders(basar::Int32 total)
		{
			m_TotalNumberOfOrders = total;
		}

		void BatchOrderEditionVC::setSendBWEnabled()
		{
			//BW creation, can only be enabled, in the order view.
			//Except when the reserveqty for order and release view are the same,
			//then it's also enabled in release view!

			if (!m_IsReserveQtyDifferent)
			{
				enableBWButtons();
			}
			else if (isOrderViewActive())
			{
				enableBWButtons();
			}
			else
			{
				ui.btnSendORFiltered->setEnabled(false);
				ui.btnSendORSelected->setEnabled(false);
			}
		}

		void BatchOrderEditionVC::enableBWButtons()
		{
			ui.btnSendORFiltered->setEnabled(true);

			if (ui.twBatchOrders->selectionModel()->selectedRows().count() > 0)
			{
				ui.btnSendORSelected->setEnabled(true);
			}
			else
			{
				ui.btnSendORSelected->setEnabled(false);
			}
		}

		bool BatchOrderEditionVC::checkIfCMIOrderSelected()
		{
			//check if any DG03 (CMI) orders are selected
			YIterators yits = getSelectedOrders();
			for (YIterators::const_iterator it = yits.begin(); it != yits.end(); ++it)
			{
				if ((it->getString(libabbauw::properties::ORDERTYPE) == "DG03"))
				{
					return true;
				}
			}

			return false;
		}

		bool BatchOrderEditionVC::checkIfCMIOrderFiltered()
		{
			//check if any DG03 (CMI) orders are selected
			for (basar::db::aspect::AccessorPropertyTable_YIterator it = m_OrdersMatcher.getRight().begin(); !it.isEnd(); ++it)
			{
				if ((it.getString(libabbauw::properties::ORDERTYPE) == "DG03"))
				{
					return true;
				}
			}

			return false;
		}


	} // end namespace viewConn
} // end namespace CSC_Batches
