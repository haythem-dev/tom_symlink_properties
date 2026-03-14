#include "cmnvc.h"
#include "changeordersvc.h"
#include "definitions_gui.h"

#include "libabbauw/iordertypecollectionparameter.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libbasarlogin_manager.h>
#include <libbasarqtwidget_dateedit.h>
#include <set>

namespace CSC_Batches
{
	namespace viewConn
	{
		ChangeOrdersVC::ChangeOrdersVC(QWidget* parent)
			: QDialog(parent), m_Logger(libabbauw::LoggerPool::loggerViewConn), m_EventsWiring("ChangeOrdersVC::EventsWiring")
		{
			ui.setupUi(this);
			Qt::WindowFlags flags = windowFlags() | Qt::WindowMinMaxButtonsHint;
			setWindowFlags(flags);
			wireEvents();
			labelWindowTitle();
			resetForm();
		}

		ChangeOrdersVC::~ChangeOrdersVC()
		{
		}

		void ChangeOrdersVC::injectParameter(libabbauw::domMod::parameter::IOrderTypeCollectionParameterPtr orderTypeCollection)
		{
			m_OrderTypeCollectionParameter = orderTypeCollection;
		}

		void ChangeOrdersVC::wireEvents()
		{
			if (m_EventsWiring.isSet()) { return; }

			//btn
			QObject::connect(ui.btnApply, SIGNAL(clicked()), this, SLOT(onBtnApplylClicked()));
			QObject::connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(onBtnCancelClicked()));
			QObject::connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(onBtnSaveClicked()));
			QObject::connect(ui.btnSearchPharmacy, SIGNAL(clicked()), this, SLOT(onBtnSearchPharmacyClicked()));
			QObject::connect(ui.btnSearchManufacturer, SIGNAL(clicked()), this, SLOT(onBtnSearchManufacturerClicked()));
			QObject::connect(ui.btnDeleteDeliveryDate, SIGNAL(clicked()), this, SLOT(onBtnDeleteDeliveryDateClicked()));
			QObject::connect(ui.btnDeleteDeferredPaymentDate, SIGNAL(clicked()), this, SLOT(onBtnDeleteDeferredPaymentDateClicked()));

			//txt
			QObject::connect(ui.txtPharmacyNumber, SIGNAL(editingFinished()), this, SLOT(onPharmacyNoChanged()));
			QObject::connect(ui.txtManufacturerNo, SIGNAL(editingFinished()), this, SLOT(onMNoChanged()));
			QObject::connect(ui.txtOrderDiscount, SIGNAL(editingFinished()), this, SLOT(onOrderDiscountChanged()));

			//cbo
			QObject::connect(ui.cboOrderType, SIGNAL(currentIndexChanged(int)), this, SLOT(onOrderTypeChanged()));

			//checkbox
			QObject::connect(ui.chkDeliveryDate, SIGNAL(clicked(bool)), ui.dtDeliveryDate, SLOT(setEnabled(bool)));
			QObject::connect(ui.chkDeferredPaymentDate, SIGNAL(clicked(bool)), ui.dtDeferredPaymentDate, SLOT(setEnabled(bool)));

			m_EventsWiring.set();
		}

		void ChangeOrdersVC::labelWindowTitle()
		{
			QString windowTitle;
			windowTitle.append(DLG_TITLESHORT1).append(DLG_NO_CHANGEORDERS).append(DLG_TITLESHORT2);
			windowTitle.append(tr("Change Transfer Orders"));
			windowTitle.append(tr(" area: "));
			basar::I18nString areaName = basar::login::Manager::getInstance().getAreaName();
			windowTitle.append(QString::fromLocal8Bit(areaName.c_str()));
			setWindowTitle(windowTitle);
		}

		void ChangeOrdersVC::initMatcher(basar::db::aspect::AccessorPropertyTableRef orders)
		{
			m_OrderHeadCollMatcher = basar::gui::tie::Manager::getInstance().createMatcher(ui.twOrders, orders);
			m_OrderHeadCollMatcher.transformProperties(ui.twOrders->horizontalHeaderLabels(), m_ListAttributes);
		}

		void ChangeOrdersVC::initHeaderLables(QStringList& lstLables)
		{
			lstLables.push_back(tr("MNo"));
			m_ListAttributes.push_back(libabbauw::properties::MANUFACTURERNO);

			lstLables.push_back(tr("Manufacturer Name"));
			m_ListAttributes.push_back(libabbauw::properties::MANUFACTURERNAME);

			lstLables.push_back(tr("Order No"));
			m_ListAttributes.push_back(libabbauw::properties::ORDERNO);

			lstLables.push_back(tr("Pharmacy No"));
			m_ListAttributes.push_back(libabbauw::properties::PHARMACYNO);

			lstLables.push_back(tr("Pharmacy Name"));
			m_ListAttributes.push_back(libabbauw::properties::PHARMACYNAME);

			lstLables.push_back(tr("Order Type"));
			m_ListAttributes.push_back(libabbauw::properties::ORDERTYPE);

			lstLables.push_back(tr("Order Discount"));
			m_ListAttributes.push_back(libabbauw::properties::DISCOUNTPCTRANGE);

			lstLables.push_back(tr("Status"));
			m_ListAttributes.push_back(libabbauw::properties::STATUS);

			lstLables.push_back(tr("Positions"));
			m_ListAttributes.push_back(libabbauw::properties::POSQTY);

			lstLables.push_back(tr("Order Date"));
			m_ListAttributes.push_back(libabbauw::properties::ORDERDATE);

			lstLables.push_back(tr("Delivery Date"));
			m_ListAttributes.push_back(libabbauw::properties::EDIDELIVERYDATE);

			lstLables.push_back(tr("Def. Payment Date"));
			m_ListAttributes.push_back(libabbauw::properties::DEFERREDPAYMENTDATE);

			lstLables.push_back(tr("Order Value"));
			m_ListAttributes.push_back(libabbauw::properties::ORDERVALUE);
		}

		void ChangeOrdersVC::initTable(QStringList& lstLables)
		{
			ui.twOrders->setColumnCount(lstLables.size());
			ui.twOrders->setHorizontalHeaderLabels(lstLables);
			ui.twOrders->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
			ui.twOrders->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
			//ui.twOrders->setHorizontalScrollBarPolicy              ( Qt::ScrollBarAsNeeded                );
			ui.twOrders->setSelectionMode(QAbstractItemView::NoSelection);
			ui.twOrders->horizontalHeader()->setStretchLastSection(true);
			ui.twOrders->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

			int i = 0;
			ui.twOrders->horizontalHeader()->resizeSection(i++, 90);		// ManufacturerNo
			ui.twOrders->horizontalHeader()->resizeSection(i++, 170);		// Manufacturer
			ui.twOrders->horizontalHeader()->resizeSection(i++, 75);		// OrderNo
			ui.twOrders->horizontalHeader()->resizeSection(i++, 80);		// PharmacyNo
			ui.twOrders->horizontalHeader()->resizeSection(i++, 180);		// Pharmacy
			ui.twOrders->horizontalHeader()->resizeSection(i++, 80);		// OrderType
			ui.twOrders->horizontalHeader()->resizeSection(i++, 90);		// OrderDiscount
			ui.twOrders->horizontalHeader()->resizeSection(i++, 70);		// Status
			ui.twOrders->horizontalHeader()->resizeSection(i++, 70);		// Positions
			ui.twOrders->horizontalHeader()->resizeSection(i++, 90);		// Orderdate
			ui.twOrders->horizontalHeader()->resizeSection(i++, 101);		// Deliverydate
			ui.twOrders->horizontalHeader()->resizeSection(i++, 80);		// DeferredPaymentDate
			ui.twOrders->horizontalHeader()->resizeSection(i++, 90);		// Ordervalue

			ui.twOrders->verticalHeader()->hide();

			ui.twOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);
		}

		void ChangeOrdersVC::setupComboBoxes()
		{
			ui.cboOrderType->clear();
			ui.cboStatus->clear();

			using libabbauw::domMod::parameter::IOrderTypeCollectionParameter;
			IOrderTypeCollectionParameter::OrderTypeCollection orderTypeColl = m_OrderTypeCollectionParameter->getOrderTypes();

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

			ui.cboStatus->addItem("");
			ui.cboStatus->addItem("ZU");
			ui.cboStatus->addItem("ST");

			ui.cboOrderType->setValidator(new QRegExpValidator(QRegExp("[A-Z]{2}[0-9]{2}"), ui.cboOrderType));

			QDoubleValidator* discountPCTValidator = new QDoubleValidator(0, 999.99, 2, ui.txtOrderDiscount);
			discountPCTValidator->setNotation(QDoubleValidator::StandardNotation);
			ui.txtOrderDiscount->setValidator(discountPCTValidator);
		}

		basar::gui::tie::WidgetReturnEnum ChangeOrdersVC::show()
		{
			QWidget::setWindowModality(Qt::ApplicationModal);
			QDialog::show();

			resetForm();

			return basar::gui::tie::WidgetReturnEnum();
		}

		void ChangeOrdersVC::resetForm()
		{
			ui.cboOrderType->setCurrentIndex(0);
			clearFields();
			clearTable();

			ui.dtDeliveryDate->setDateRange(QDate(1000, 1, 1), QDate(9999, 12, 31));
			ui.dtDeferredPaymentDate->setDateRange(QDate(1000, 1, 1), QDate(9999, 12, 31));

			ui.dtDeliveryDate->setSpecialValueText(" ");
			ui.dtDeliveryDate->setDate(QDate::currentDate());
			ui.dtDeliveryDate->setEnabled(false);

			ui.dtDeferredPaymentDate->setSpecialValueText(" ");
			ui.dtDeferredPaymentDate->setDate(QDate::currentDate());
			ui.dtDeferredPaymentDate->setEnabled(false);

			setSavingAllowed(false); //disables SaveButton
		}

		void ChangeOrdersVC::clearFields()
		{
			ui.txtManufacturerNo->clear();
			ui.txtManufacturerName->clear();
			ui.txtPharmacyNumber->clear();
			ui.txtPharmacyName->clear();
			ui.cboOrderType->setCurrentIndex(0);
			ui.txtOrderDiscount->clear();
			ui.txtOrderDiscount->setEnabled(false);
			ui.cboStatus->setCurrentIndex(0);
			ui.chkDeliveryDate->setChecked(false);
		}

		void ChangeOrdersVC::clearTable()
		{
			ui.twOrders->clearContents();
			ui.twOrders->setRowCount(0);
		}

		void ChangeOrdersVC::alignTableColumns()
		{
			ui.twOrders->setTextAlignmentColumn(0, Qt::AlignRight | Qt::AlignVCenter);		// ManufacturerNo
			ui.twOrders->setTextAlignmentColumn(1, Qt::AlignLeft | Qt::AlignVCenter);		// Manufacturer
			ui.twOrders->setTextAlignmentColumn(2, Qt::AlignRight | Qt::AlignVCenter);		// OrderNo
			ui.twOrders->setTextAlignmentColumn(3, Qt::AlignRight | Qt::AlignVCenter);		// PharmacyNo
			ui.twOrders->setTextAlignmentColumn(4, Qt::AlignLeft | Qt::AlignVCenter);		// Pharmacy
			ui.twOrders->setTextAlignmentColumn(5, Qt::AlignCenter | Qt::AlignVCenter);		// OrderType
			ui.twOrders->setTextAlignmentColumn(6, Qt::AlignCenter | Qt::AlignVCenter);		// OrderDiscountRange
			ui.twOrders->setTextAlignmentColumn(7, Qt::AlignCenter | Qt::AlignVCenter);		// Status
			ui.twOrders->setTextAlignmentColumn(8, Qt::AlignRight | Qt::AlignVCenter);		// Positions
			ui.twOrders->setTextAlignmentColumn(9, Qt::AlignRight | Qt::AlignVCenter);		// Orderdate
			ui.twOrders->setTextAlignmentColumn(10, Qt::AlignRight | Qt::AlignVCenter);		// Deliverydate
			ui.twOrders->setTextAlignmentColumn(11, Qt::AlignRight | Qt::AlignVCenter);		// DeferredPaymentDate
			ui.twOrders->setTextAlignmentColumn(12, Qt::AlignLeft | Qt::AlignVCenter);		// Ordervalue
		}

		void ChangeOrdersVC::init(basar::db::aspect::AccessorPropertyTableRef orderColl)
		{
			QStringList lstLabels;
			initHeaderLables(lstLabels);
			initTable(lstLabels);
			initMatcher(orderColl);

			setupComboBoxes();
		}

		void ChangeOrdersVC::matchFromOrders()
		{
			libutil::gui::SignalBlocker signalblocker(ui.twOrders);
			clearTable();

			const basar::Int32 orderCount = static_cast<basar::Int32>(m_OrderHeadCollMatcher.getRight().size());
			ui.twOrders->setRowCount(orderCount);
			basar::Int32 matchedHits = m_OrderHeadCollMatcher.RightToLeft(0, ui.twOrders->rowCount());
			ui.twOrders->setRowCount(matchedHits);
			ui.lblOrderCount->setText(QString::number(orderCount));

			alignTableColumns();
		}

		void ChangeOrdersVC::matchToOrders()
		{
			for (basar::db::aspect::AccessorPropertyTable_YIterator yit = m_OrderHeadCollMatcher.getRight().begin(); !yit.isEnd(); ++yit)
			{
				if (!ui.txtManufacturerNo->text().isEmpty())
				{
					yit.setInt32(libabbauw::properties::MANUFACTURERNO, ui.txtManufacturerNo->text().toInt());
					yit.setString(libabbauw::properties::MANUFACTURERNAME, ui.txtManufacturerName->text().toLocal8Bit().constData());
				}

				if (!ui.txtPharmacyNumber->text().isEmpty())
				{
					yit.setInt32(libabbauw::properties::PHARMACYNO, ui.txtPharmacyNumber->text().toInt());
					yit.setString(libabbauw::properties::PHARMACYNAME, ui.txtPharmacyName->text().toLocal8Bit().constData());
				}

				if (!ui.cboOrderType->currentText().isEmpty())
				{
					yit.setString(libabbauw::properties::ORDERTYPE, ui.cboOrderType->currentText().toLocal8Bit().constData());
				}

				if (!ui.txtOrderDiscount->text().isEmpty())
				{
					yit.setString(libabbauw::properties::DISCOUNTPCTRANGE, ("-" + ui.txtOrderDiscount->text()).toLocal8Bit().constData());
				}

				if (!ui.cboStatus->currentText().isEmpty())
				{
					yit.setString(libabbauw::properties::STATUS, ui.cboStatus->currentText().toLocal8Bit().constData());
				}

				if (ui.chkDeliveryDate->isChecked())
				{
					//Minimum date = date should be deleted (0)
					if (ui.dtDeliveryDate->minimumDate() == ui.dtDeliveryDate->date())
					{
						yit.setDate(libabbauw::properties::EDIDELIVERYDATE, 0);
					}
					else
					{
						const QDate& date = ui.dtDeliveryDate->date();
						yit.setDate(libabbauw::properties::EDIDELIVERYDATE, basar::Date(date.year(), date.month(), date.day()));
					}
				}

				if (ui.chkDeferredPaymentDate->isChecked())
				{
					//Miniumum date = date should be deleted (0)
					if (ui.dtDeferredPaymentDate->minimumDate() == ui.dtDeferredPaymentDate->date())
					{
						yit.setDate(libabbauw::properties::DEFERREDPAYMENTDATE, 0);
					}
					else
					{
						const QDate& date = ui.dtDeferredPaymentDate->date();
						yit.setDate(libabbauw::properties::DEFERREDPAYMENTDATE, basar::Date(date.year(), date.month(), date.day()));
					}
				}
			}
		}

		void ChangeOrdersVC::matchFromManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			if (!yit.isEnd())
			{
				ui.txtManufacturerName->setText(yit.getString(libabbauw::properties::MANUFACTURERNAME).c_str());
				ui.txtManufacturerNo->setText(QString::number(yit.getInt32(libabbauw::properties::MANUFACTURERNO)));
			}
			else
			{
				info(tr("Manufacturer number not found!").toLocal8Bit().constData());
				ui.txtManufacturerNo->clear();
				ui.txtManufacturerName->clear();
			}
		}

		void ChangeOrdersVC::matchToManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			yit.setInt32(libabbauw::properties::MANUFACTURERNO, ui.txtManufacturerNo->text().toInt());
		}

		void ChangeOrdersVC::matchFromPharmacy(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			if (!yit.isEnd())
			{
				ui.txtPharmacyName->setText(yit.getString(libabbauw::properties::PHARMACYNAME).c_str());
				ui.txtPharmacyNumber->setText(QString::number(yit.getInt32(libabbauw::properties::PHARMACYNO)));
			}
			else
			{
				info(tr("Pharmacy number not found!").toLocal8Bit().constData());
				ui.txtPharmacyNumber->clear();
				ui.txtPharmacyName->clear();
			}
		}

		void ChangeOrdersVC::matchToPharmacy(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			yit.setInt32(libabbauw::properties::PHARMACYNO, ui.txtPharmacyNumber->text().toInt());
		}

		void ChangeOrdersVC::matchFromOrderDiscount(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			if (!yit.isEnd())
			{
				ui.txtOrderDiscount->setText(yit.getString(libabbauw::properties::DISCOUNTPCTRANGE).c_str());
			}
			else
			{
				ui.txtOrderDiscount->clear();
			}
		}

		void ChangeOrdersVC::matchToOrderDiscount(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			yit.setString(libabbauw::properties::DISCOUNTPCTRANGE, ui.txtOrderDiscount->text().toStdString());
		}

		basar::VarString ChangeOrdersVC::getOrderDiscount()
		{
			if (ui.txtOrderDiscount->text().isEmpty())
			{
				return basar::VarString();
			}
			else
			{
				return basar::VarString(ui.txtOrderDiscount->text().toStdString());
			}
		}

		void ChangeOrdersVC::onBtnApplylClicked()
		{
			CSC_Batches::gui::WaitCursor waitCursor(this);
			basar::appl::SystemEventManager::getInstance().fire("ApplyChangesRequested");
		}

		void ChangeOrdersVC::onBtnCancelClicked()
		{
			close();
		}

		void ChangeOrdersVC::onBtnSaveClicked()
		{
			CSC_Batches::gui::WaitCursor waitCursor(this);
			ui.btnSave->setEnabled(false);

			basar::appl::SystemEventManager::getInstance().fire("SaveOrderRequested");
		}

		bool ChangeOrdersVC::shutdown()
		{
			return close();
		}

		void ChangeOrdersVC::waitForEvents()
		{
			setAttribute(Qt::WA_Moved, false);
			exec();
		}

		void ChangeOrdersVC::onBtnSearchManufacturerClicked()
		{
			basar::appl::SystemEventManager::getInstance().fire("FindManufacturerRequested");
		}

		void ChangeOrdersVC::onBtnSearchPharmacyClicked()
		{
			basar::appl::SystemEventManager::getInstance().fire("FindPharmacyRequested");
		}

		void ChangeOrdersVC::onPharmacyNoChanged()
		{
			if (!ui.txtPharmacyNumber->text().isEmpty())
				basar::appl::SystemEventManager::getInstance().fire("CheckPharmacyNoRequested");
		}

		void ChangeOrdersVC::onMNoChanged()
		{
			if (!ui.txtManufacturerNo->text().isEmpty())
				basar::appl::SystemEventManager::getInstance().fire("CheckManufacturerNoRequested");
		}

		void ChangeOrdersVC::onOrderTypeChanged()
		{
			if (!ui.cboOrderType->currentText().isEmpty())
			{
				// TODO: RBV: use ini file or logpara for getting the order types
				if (ui.cboOrderType->currentText() == "GM03" || ui.cboOrderType->currentText() == "GM00")
				{
					ui.txtOrderDiscount->setEnabled(true);
					return;
				}
			}
			ui.txtOrderDiscount->setEnabled(false);
			ui.txtOrderDiscount->setText("");
		}

		void ChangeOrdersVC::onOrderDiscountChanged()
		{
			if (!ui.txtOrderDiscount->text().isEmpty())
			{
				bool valid = false;
				double test = locale().toDouble(ui.txtOrderDiscount->text(), &valid);

				if (!valid)
				{
					ui.txtOrderDiscount->setText("");
					info(tr("Entered DPCT is invalid!").toLocal8Bit().constData());
					ui.txtOrderDiscount->setFocus();
				}
				else if (test > 100.00)
				{
					ui.txtOrderDiscount->setText(locale().toString(100.00, 'f', 2));
					info(tr("DPCT was reduced to 100.00%!").toLocal8Bit().constData());
					ui.txtOrderDiscount->setFocus();
				}
				else if (ui.txtOrderDiscount->text()[0] == ",")
				{
					info(tr("Entered DPCT is invalid!").toLocal8Bit().constData());
					ui.txtOrderDiscount->setFocus();
				}
				else
				{
					basar::appl::SystemEventManager::getInstance().fire("OrderDiscountRequested");
				}
			}
		}

		void ChangeOrdersVC::onBtnDeleteDeliveryDateClicked()
		{
			if (ui.dtDeliveryDate->minimumDate() == ui.dtDeliveryDate->date())
			{
				ui.dtDeliveryDate->setDate(QDate::currentDate());
				ui.dtDeliveryDate->setDisabled(true);

				ui.chkDeliveryDate->setChecked(false);
				ui.chkDeliveryDate->setEnabled(true);
			}
			else
			{
				ui.dtDeliveryDate->setDate(ui.dtDeliveryDate->minimumDate());
				ui.dtDeliveryDate->setDisabled(true);

				ui.chkDeliveryDate->setChecked(true);
				ui.chkDeliveryDate->setDisabled(true);
			}
		}

		void ChangeOrdersVC::onBtnDeleteDeferredPaymentDateClicked()
		{
			if (ui.dtDeferredPaymentDate->minimumDate() == ui.dtDeferredPaymentDate->date())
			{
				ui.dtDeferredPaymentDate->setDate(QDate::currentDate());
				ui.dtDeferredPaymentDate->setDisabled(true);

				ui.chkDeferredPaymentDate->setChecked(false);
				ui.chkDeferredPaymentDate->setEnabled(true);
			}
			else
			{
				ui.dtDeferredPaymentDate->setDate(ui.dtDeliveryDate->minimumDate());
				ui.dtDeferredPaymentDate->setDisabled(true);

				ui.chkDeferredPaymentDate->setChecked(true);
				ui.chkDeferredPaymentDate->setDisabled(true);
			}
		}

		void ChangeOrdersVC::info(const basar::I18nString& text)
		{
			basar::gui::tie::infoMsgBox(this, text.c_str(), tr("Change Orders").toLocal8Bit().data());
		}

		void ChangeOrdersVC::error(const basar::I18nString& text)
		{
			basar::gui::tie::criticalMsgBox(this, text.c_str(), tr("Change Orders").toLocal8Bit().data());
		}

		const basar::gui::tie::MessageBoxButtonEnum ChangeOrdersVC::question(const basar::I18nString& text)
		{
			return basar::gui::tie::questionMsgBox(this, text.c_str(), tr("Change Orders").toLocal8Bit().data());
		}

		void ChangeOrdersVC::setSavingAllowed(bool isAllowed)
		{
			ui.btnSave->setEnabled(isAllowed);
		}
	} // end namespace viewConn
} // end namespace CSC_Batches
