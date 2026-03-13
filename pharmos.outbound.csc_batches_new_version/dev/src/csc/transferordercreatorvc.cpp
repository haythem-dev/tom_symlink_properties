//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmnvc.h"
#include "transferordercreatorvc.h"
#include "definitions_gui.h"
#include "discountflags_transformer.h"

#include "libabbauw/iordertypecollectionparameter.h"

#include "libabbauw/loggerpool/libabbauw_loggerpool.h"
#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/libabbauw_definitions.h"

#include <libbasarlogin_manager.h>
#include <QtWidgets/QShortcut>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QMessageBox>
#include <set>

#pragma warning (push)
#pragma warning(disable: 4244)
#include <QtGui/qevent.h>
#pragma warning (pop)

namespace CSC_Batches
{
	namespace viewConn
	{

		TransferOrderCreatorVC::TransferOrderCreatorVC(QWidget* parent)
			: QDialog(parent), m_Logger(libabbauw::LoggerPool::loggerViewConn), m_EventsWiring("TransferOrderCreatorVC::EventsWiring")
		{
			basar::VarString GROUP_STR_SWITZERLAND = "CH";
			basar::VarString GROUP_STR_SWITZERLAND_PREWHOLE = "PCH";

			basar::VarString GROUP_STR_GERMANY1 = "DE1";
			basar::VarString GROUP_STR_GERMANY2 = "DE2";

			basar::login::Manager::getInstance();
			basar::Int16 branchNo = basar::login::Manager::getInstance().getAreaID();
			// get user´s branch he logged in for to get area-specific database
			basar::I18nString branchStr;
			branchStr.format("%d", branchNo);
			basar::ConstString UADM_GROUP = "GROUP";
			// get GROUP parameter
			basar::I18nString group = "";
			basar::login::Manager::getInstance().getParaValue(branchStr, UADM_GROUP, group);

			if (GROUP_STR_SWITZERLAND == group || GROUP_STR_SWITZERLAND_PREWHOLE == group)
			{
				m_countrygroup = "CH";
			}

			if (GROUP_STR_GERMANY1 == group || GROUP_STR_GERMANY2 == group)
			{
				m_countrygroup = group;
			}

			basar::I18nString headertext_pattern = "#mfno#:#pharmacyno#/ #ordernomf# : #mfname#";
			basar::login::Manager::getInstance().getParaValue(group, "HEADERTEXT_PATTERN", headertext_pattern);

			m_headertext_pattern = headertext_pattern;

			ui.setupUi(this);
			wireEvents();
			wireShortcuts();

			ui.cbDiscountType->setFocusPolicy(Qt::ClickFocus);
			ui.cbDiscountTypeAllPositions->setFocusPolicy(Qt::ClickFocus);

			labelWindowTitle();
			setupOrderHeadFrame();
			setupOrderPosFrame();
			initOrderHeadFrame();
			initHeaderLabels();

			installEventFilter(this);
			ui.btnSearchArticle->installEventFilter(this);

		}

		TransferOrderCreatorVC::~TransferOrderCreatorVC()
		{
		}
		void TransferOrderCreatorVC::handleCountryBasedColumns()
		{
			if (m_countrygroup != "CH")
			{
				// extra comboboxes, labels and table columns for CH discount flags will not be shown in other countries
				ui.cbDiscountApplyTo->hide();               // combobox for Discount Apply To Flag
				ui.cbDiscountCalcFrom->hide();              // combobox for Discount Calc From Flag
				ui.lblDiscountApplyTo->hide();              // label for Discount Apply To Flag
				ui.lblDiscountCalcFrom->hide();             // label for Discount Calc From Flag
				ui.twBatchOrderPositions->hideColumn(0);    // column in positions table for position status
				ui.twBatchOrderPositions->hideColumn(9);    // column in positions table for Discount Calc From Flag
				ui.twBatchOrderPositions->hideColumn(10);   // column in positions table for Discount Apply To Flag
			}
			else
			{
				//ui.lblOrderNoMF->hide();
				//ui.txtOrderNoMF->hide();
			}
		}

		void TransferOrderCreatorVC::ValidateAndSetPositionsStatus()
		{
			// only for switzerland AGP and GEP and discount flags exists to be validated
			if ((m_countrygroup == "CH") && (m_OrderPosMatcher.isNull() == false))
			{
				basar::db::aspect::AccessorPropertyTableRef positions = m_OrderPosMatcher.getRight();
				if (positions.isNull() == false)
				{
					int rowindex = 0;
					basar::db::aspect::AccessorPropertyTable_YIterator yit;

					for (yit = positions.begin(); yit != positions.end(); yit++)
					{
						if (!yit.getState().contains(basar::SS_DELETE))
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
								QTableWidgetItem* pItem = ui.twBatchOrderPositions->item(rowindex, 0);
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
			adjustTableSize();
		}

		void TransferOrderCreatorVC::injectParameter(libabbauw::domMod::parameter::IOrderTypeCollectionParameterPtr orderTypeCollection)
		{
			m_OrderTypeCollectionParameter = orderTypeCollection;
		}

		void TransferOrderCreatorVC::init(basar::db::aspect::AccessorPropertyTable_YIterator head, basar::db::aspect::AccessorPropertyTableRef pos)
		{
			m_Head = head;
			m_Positions = pos;

			m_StandardAART = libabbauw::lit::UW_ORDERTYPE + libabbauw::lit::UW_ORDERPICKINGTYPE + libabbauw::lit::UW_BOOKENTRYTYPE;
			m_StandardDiscountType = "BR"; //TODO DZ klären von wo die typen kommen sollen!

			initTable();
			initHeadMatcher();

			initPosMatcher(m_LstOriginalAttributes);
			adjustTableSize();

			ui.btnAddChange->setEnabled(false);
			ui.btnRemove->setEnabled(false);
			setClearBtnEnableState();
			setupComboBoxes();
			handleCountryBasedColumns();
		}

		void TransferOrderCreatorVC::wireEvents()
		{
			if (m_EventsWiring.isSet())
				return;

			// Button events
			connect(ui.btnSearchArticle, SIGNAL(clicked()), this, SLOT(onBtnSearchArticleClicked()));
			connect(ui.btnReset, SIGNAL(clicked()), this, SLOT(onBtnResetClicked()));
			connect(ui.btnSearchPharmacy, SIGNAL(clicked()), this, SLOT(onBtnSearchPharmacy()));
			connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(onBtnSaveClicked()));
			connect(ui.btnCreateTransferOrder, SIGNAL(clicked()), this, SLOT(onBtnCreateTransferOrder()));
			connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(onBtnCloseClicked()));
			connect(ui.btnRemove, SIGNAL(clicked()), this, SLOT(onBtnRemovePosClicked()));
			connect(ui.btnAddChange, SIGNAL(clicked()), this, SLOT(onBtnAddPosClicked()));
			connect(ui.btnDiscard, SIGNAL(clicked()), this, SLOT(onBtnDiscardClicked()));

			// TextBox events                                                                                           
			connect(ui.txtOrderNoMF, SIGNAL(editingFinished()), this, SLOT(onOrderNoMFChanged()));
			connect(ui.txtPharmacyNumber, SIGNAL(editingFinished()), this, SLOT(onPharmacyNoChanged()));
			connect(ui.txtHeadertext, SIGNAL(editingFinished()), this, SLOT(onHeadertextChanged()));
			connect(ui.txtMatchCode, SIGNAL(editingFinished()), this, SLOT(onArticleChanged()));
			connect(ui.txtPZN, SIGNAL(editingFinished()), this, SLOT(onArticleChanged()));
			connect(ui.txtPZN, SIGNAL(textEdited(const QString&)), this, SLOT(onArticleSearchTyped(const QString&)));
			connect(ui.txtMatchCode, SIGNAL(textEdited(const QString&)), this, SLOT(onArticleSearchTyped(const QString&)));
			connect(ui.txtQty, SIGNAL(textChanged(const QString&)), this, SLOT(onPosEditCheck(const QString&)));
			connect(ui.txtNonChargedQty, SIGNAL(textChanged(const QString&)), this, SLOT(onPosEditCheck(const QString&)));
			connect(ui.txtDiscountPCT, SIGNAL(textChanged(const QString&)), this, SLOT(onPosEditCheck(const QString&)));
			connect(ui.txtArticleNo, SIGNAL(textChanged(const QString&)), this, SLOT(onPosEditCheck(const QString&)));
			connect(ui.txtDiscountPCT, SIGNAL(editingFinished()), this, SLOT(onDiscountPCTEditingFinished()));
			connect(ui.txtDeferredPaymentDate, SIGNAL(textChanged(const QString&)), this, SLOT(onDefPaymentDtAddDays(const QString&)));
			connect(ui.txtDiscountAllPositions, SIGNAL(textChanged(const QString&)), this, SLOT(onDiscountAllPositionsChanged(const QString&)));
			connect(ui.txtDiscountAllPositions, SIGNAL(editingFinished()), this, SLOT(onDiscountAllPositionsChanged()));

			// ComboBox
			connect(ui.cboOrderType, SIGNAL(currentIndexChanged(int)), this, SLOT(cboOrderTypeChanged(int)));
			connect(ui.cbDiscountTypeAllPositions, SIGNAL(currentIndexChanged(int)), this, SLOT(onDiscountAllPositionsTypeChanged(int)));
			connect(ui.dtDeliveryDate, SIGNAL(dateChanged(const QDate&)), this, SLOT(onDateChanged(const QDate&)));
			connect(ui.dtDeferredPaymentDate, SIGNAL(dateChanged(const QDate&)), this, SLOT(onDateChanged(const QDate&)));

			// Checkbox
			connect(ui.chkDeliveryDate, SIGNAL(clicked(bool)), this, SLOT(onDeliveryDateChecked(bool)));
			connect(ui.chkDeferredPaymentDate, SIGNAL(clicked(bool)), this, SLOT(onDeferredPaymentDateChecked(bool)));
			connect(ui.chkHeadertext, SIGNAL(clicked(bool)), this, SLOT(onManualHeadertextChecked(bool)));

			// List
			connect(ui.twBatchOrderPositions, SIGNAL(dataSelectionChanged()), this, SLOT(onTableDataSelectionChanged()));

			m_EventsWiring.set();
		}

		void TransferOrderCreatorVC::wireShortcuts()
		{
			QShortcut* clearSelection = new QShortcut(QKeySequence("F2"), ui.grpAcquirePosition);

			QObject::connect(clearSelection, SIGNAL(activated()), this, SLOT(onSelectPositionShortcut()));
		}

		basar::VarString TransferOrderCreatorVC::getCountryCode()
		{
			return m_countrygroup;
		}

		basar::gui::tie::WidgetReturnEnum TransferOrderCreatorVC::show()
		{
			setAttribute(Qt::WA_Moved, false);
			QWidget::setWindowModality(Qt::ApplicationModal);
			QDialog::show();

			return basar::gui::tie::WidgetReturnEnum();
		}

		void TransferOrderCreatorVC::waitForEvents()
		{
			setAttribute(Qt::WA_Moved, false);

			// everything set, adjust fields
			if (ui.chkHeadertext->isChecked())
			{
				ui.txtHeadertext->setEnabled(true);
			}

			ui.txtHeadertext->home(false);

			exec();
		}

		bool TransferOrderCreatorVC::shutdown()
		{
			return close();
		}

		void TransferOrderCreatorVC::info(const basar::I18nString& text)
		{
			basar::gui::tie::infoMsgBox(this, text.c_str(), tr("Info").toLocal8Bit().constData());
		}

		void TransferOrderCreatorVC::error(const basar::I18nString& text)
		{
			basar::gui::tie::criticalMsgBox(this, text.c_str(), tr("Error").toLocal8Bit().constData());
		}

		const basar::gui::tie::MessageBoxButtonEnum TransferOrderCreatorVC::question(const basar::I18nString& text)
		{
			using namespace basar::gui::tie;
			return questionMsgBox(this, text.c_str(), ButtonYes | ButtonNo, ButtonNo, tr("Question").toLocal8Bit().constData());
		}

		void TransferOrderCreatorVC::labelWindowTitle()
		{
			QString windowTitle;
			windowTitle.append(DLG_TITLESHORT1).append(DLG_NO_TRANSFERORDERCREATOR).append(DLG_TITLESHORT2);
			windowTitle.append(tr("Acquisition of Transfer Order"));
			windowTitle.append(tr(" area: "));
			windowTitle.append(QString::fromLocal8Bit(basar::login::Manager::getInstance().getAreaName().c_str()));
			setWindowTitle(windowTitle);
		}

		void TransferOrderCreatorVC::clearHeadInputFields()
		{
			ui.txtManufacturerName->clear();
			ui.txtManufacturerNumber->clear();
			ui.txtOrderNoMF->clear();
			ui.txtPharmacyName->clear();
			ui.txtPharmacyNumber->clear();
			ui.txtPharmacyLocation->clear();
			ui.chkHeadertext->setChecked(false);
			ui.txtHeadertext->clear();
			ui.txtHeadertext->setEnabled(false);
			ui.cboOrderType->setCurrentIndex(ui.cboOrderType->findText(m_StandardAART.c_str()));
			ui.dtDeliveryDate->setDate(QDate::currentDate());
			ui.chkDeliveryDate->setChecked(false);
			ui.dtDeferredPaymentDate->setDate(QDate::currentDate());
			ui.chkDeferredPaymentDate->setChecked(false);
			ui.txtDeferredPaymentDate->clear();
			ui.txtDiscountAllPositions->clear();
			ui.cbDiscountTypeAllPositions->setCurrentIndex(ui.cbDiscountTypeAllPositions->findText(m_StandardDiscountType.c_str()));
			ui.cbDiscountType->setCurrentIndex(ui.cbDiscountType->findText(m_StandardDiscountType.c_str()));
			ui.cbDiscountCalcFrom->setCurrentIndex(0);
			ui.cbDiscountApplyTo->setCurrentIndex(1);
		}

		void TransferOrderCreatorVC::resetPositionLineEdits()
		{
			ui.txtQty->clear();
			ui.txtNonChargedQty->clear();
			ui.txtDiscountPCT->clear();
			ui.cbDiscountCalcFrom->setCurrentIndex(0);
			ui.cbDiscountApplyTo->setCurrentIndex(1);
			resetArticleLineEdits();
		}

		void TransferOrderCreatorVC::resetArticleLineEdits()
		{
			ui.txtArticleName->clear();
			ui.txtUnit->clear();
			ui.txtPharmaceuticalForm->clear();
			ui.txtMatchCode->clear();
			ui.txtArticleNo->clear();
			ui.txtPZN->clear();

			m_lastPZN.clear();
			m_lastMatchCode.clear();

			ui.txtPZN->setEnabled(true);
			ui.txtMatchCode->setEnabled(true);
		}

		void TransferOrderCreatorVC::initHeaderLabels()
		{
			m_LstLabels.push_back(tr("Sts")); // 0
			m_LstOriginalAttributes.push_back(libabbauw::properties::POSITIONSTATUS);

			m_LstLabels.push_back(tr("PNo")); // 1
			m_LstOriginalAttributes.push_back(libabbauw::properties::POSNO);

			m_LstLabels.push_back(tr("OQTY")); // 2
			m_LstOriginalAttributes.push_back(libabbauw::properties::ORDEREDQTY);

			m_LstLabels.push_back(tr("DPCT")); // 3
			m_LstOriginalAttributes.push_back(libabbauw::properties::DISCOUNTPCT);

			m_LstLabels.push_back(tr("ND")); // 4
			m_LstOriginalAttributes.push_back(libabbauw::properties::NONCHARGEDQTY);

			m_LstLabels.push_back(tr("PZN")); // 5
			m_LstOriginalAttributes.push_back(libabbauw::properties::PZN);

			m_LstLabels.push_back(tr("Item Name")); // 6
			m_LstOriginalAttributes.push_back(libabbauw::properties::ARTICLENAME);

			m_LstLabels.push_back(tr("Unit")); // 7
			m_LstOriginalAttributes.push_back(libabbauw::properties::UNIT);

			m_LstLabels.push_back(tr("PF")); // 8
			m_LstOriginalAttributes.push_back(libabbauw::properties::PHARMACEUTICALFORM);

			m_LstLabels.push_back(tr("DCF")); // 9
			m_LstOriginalAttributes.push_back(libabbauw::properties::DISCOUNTCALCFROM);

			m_LstLabels.push_back(tr("DAT")); // 10
			m_LstOriginalAttributes.push_back(libabbauw::properties::DISCOUNTAPPLYTO);
		}

		void TransferOrderCreatorVC::initOrderHeadFrame()
		{
			clearHeadInputFields();
		}

		void TransferOrderCreatorVC::initHeadMatcher()
		{
			m_OrderHeadMatcher = basar::gui::tie::Manager::getInstance().createMatcher(ui.grpTransferOrderHead, m_Head.getPropertyTable());

			basar::gui::tie::CheckBoxTransformer chkBoxTransformer("chkHeadertext", "1", "0", basar::gui::tie::CheckBoxTransformer::EvaluateEnum::EVAL_STATE_OFF, libabbauw::properties::ISMANUALHEADERTEXTFLAG.getName());
			m_OrderHeadMatcher.push_back(chkBoxTransformer);
		}

		void TransferOrderCreatorVC::initPosMatcher(std::list<basar::VarString>& listPropTable)
		{
			m_OrderPosMatcher = basar::gui::tie::Manager::getInstance().createMatcher(ui.twBatchOrderPositions, m_Positions);
			// add ignore flag for deleted positions, the rest are the default flags
			m_OrderPosMatcher.setMatchingPolicy(basar::gui::tie::INSERT | basar::gui::tie::SKIP | basar::gui::tie::CONVERTTYPE | basar::gui::tie::DISREGARD_DELETED);
			m_OrderPosMatcher.transformProperties(ui.twBatchOrderPositions->horizontalHeaderLabels(), listPropTable);

			basar::gui::tie::DecimalTransformer decimalTransformer(libabbauw::properties::DISCOUNTPCT, tr("DPCT").toLocal8Bit().data(), 2);
			m_OrderPosMatcher.push_back(decimalTransformer);

			DiscountFlagsTransformer dcfTransformer(libabbauw::properties::DISCOUNTCALCFROM.getName(), tr("DCF").toLocal8Bit().constData());
			m_OrderPosMatcher.push_back(dcfTransformer);

			DiscountFlagsTransformer datTransformer(libabbauw::properties::DISCOUNTAPPLYTO.getName(), tr("DAT").toLocal8Bit().constData());
			m_OrderPosMatcher.push_back(datTransformer);
		}

		void TransferOrderCreatorVC::adjustTableSize()
		{
			libutil::gui::SignalBlocker signalBlocker(ui.twBatchOrderPositions);

			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); //status
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); //PosNo
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents); //Qty
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents); //DPCT
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents); //ND
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents); //PZN
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch); //ArtName - Use all remaining size for item name
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents); //Unit
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents); //PF
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents); //DCF
			ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode(10, QHeaderView::ResizeToContents); //DAT


			ui.twBatchOrderPositions->resizeRowsToContents();
		}

		void TransferOrderCreatorVC::initTable()
		{
			ui.twBatchOrderPositions->setColumnCount(m_LstLabels.count());
			ui.twBatchOrderPositions->setHorizontalHeaderLabels(m_LstLabels);
			ui.twBatchOrderPositions->setResizeColumnsToContents(true);
			ui.twBatchOrderPositions->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
			ui.twBatchOrderPositions->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
			ui.twBatchOrderPositions->setSelectionBehavior(QAbstractItemView::SelectRows);
			ui.twBatchOrderPositions->setSelectionMode(QAbstractItemView::SingleSelection);
			ui.twBatchOrderPositions->setEditTriggers(QAbstractItemView::NoEditTriggers);

			ui.twBatchOrderPositions->verticalHeader()->hide();

			ui.twBatchOrderPositions->setTextAlignmentColumn(0, Qt::AlignCenter | Qt::AlignVCenter); //status
			ui.twBatchOrderPositions->setTextAlignmentColumn(1, Qt::AlignRight | Qt::AlignVCenter); //PosNo 
			ui.twBatchOrderPositions->setTextAlignmentColumn(2, Qt::AlignRight | Qt::AlignVCenter); //Qty
			ui.twBatchOrderPositions->setTextAlignmentColumn(3, Qt::AlignRight | Qt::AlignVCenter); //DPCT
			ui.twBatchOrderPositions->setTextAlignmentColumn(4, Qt::AlignRight | Qt::AlignVCenter); //ND
			ui.twBatchOrderPositions->setTextAlignmentColumn(5, Qt::AlignRight | Qt::AlignVCenter); //PZN
			ui.twBatchOrderPositions->setTextAlignmentColumn(6, Qt::AlignLeft | Qt::AlignVCenter); //ArtName
			ui.twBatchOrderPositions->setTextAlignmentColumn(7, Qt::AlignRight | Qt::AlignVCenter); //Unit
			ui.twBatchOrderPositions->setTextAlignmentColumn(8, Qt::AlignLeft | Qt::AlignVCenter); //PF
			ui.twBatchOrderPositions->setTextAlignmentColumn(9, Qt::AlignCenter | Qt::AlignVCenter); //DCF
			ui.twBatchOrderPositions->setTextAlignmentColumn(10, Qt::AlignCenter | Qt::AlignVCenter); //DAT
		}

		void TransferOrderCreatorVC::setupOrderHeadFrame()
		{
			ui.txtManufacturerNumber->setAccessibleName(libabbauw::properties::MANUFACTURERNO.getName().c_str());
			ui.txtManufacturerName->setAccessibleName(libabbauw::properties::MANUFACTURERNAME.getName().c_str());
			ui.txtOrderNoMF->setAccessibleName(libabbauw::properties::ORDERNOMF.getName().c_str());
			ui.txtPharmacyNumber->setAccessibleName(libabbauw::properties::PHARMACYNO.getName().c_str());
			ui.txtPharmacyName->setAccessibleName(libabbauw::properties::PHARMACYNAME.getName().c_str());
			ui.txtPharmacyLocation->setAccessibleName(libabbauw::properties::LOCATION.getName().c_str());
			ui.txtHeadertext->setAccessibleName(libabbauw::properties::HEADERTEXT.getName().c_str());

			ui.txtManufacturerNumber->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,5}"), ui.txtManufacturerNumber));
			ui.txtOrderNoMF->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,20}"), ui.txtOrderNoMF));
			ui.txtPharmacyNumber->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,8}"), ui.txtPharmacyNumber));
			ui.txtDeferredPaymentDate->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), ui.txtDeferredPaymentDate));
			ui.txtHeadertext->setValidator(new QRegExpValidator(QRegExp(".{0,30}"), ui.txtHeadertext));

			if (m_countrygroup != "CH")
			{
				ui.txtOrderNoMF->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9-_. ]{0,15}"), ui.txtOrderNoMF));
			}
			else
			{
				ui.txtOrderNoMF->setValidator(new QRegExpValidator(QRegExp(".{0,20}"), ui.txtOrderNoMF));
			}

			ui.txtHeadertext->home(false);
		}

		void TransferOrderCreatorVC::setupComboBoxes()
		{
			libutil::gui::SignalBlocker signalblocker(ui.cboOrderType);

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


			ui.cbDiscountCalcFrom->addItem("GEP");
			ui.cbDiscountCalcFrom->addItem("AGP");

			ui.cbDiscountApplyTo->addItem("GEP");
			ui.cbDiscountApplyTo->addItem("AGP");

			//By default UW00 should be displayed
			ui.cboOrderType->setCurrentIndex(ui.cboOrderType->findText(m_StandardAART.c_str()));

			//TODO DZ von wo Types
			ui.cbDiscountType->setAccessibleName(libabbauw::properties::DISCOUNTTYPE.getName().c_str());
			ui.cbDiscountType->addItem("BR");
			//ui.cbDiscountType->addItem("AR");

			//By default BR should be displayed
			ui.cbDiscountType->setCurrentIndex(ui.cbDiscountType->findText(m_StandardDiscountType.c_str()));

			//TODO DZ von wo Types
			ui.cbDiscountTypeAllPositions->addItem("BR");
			//ui.cbDiscountTypeAllPositions->addItem("AR");

			//By default BR should be displayed
			ui.cbDiscountTypeAllPositions->setCurrentIndex(ui.cbDiscountTypeAllPositions->findText(m_StandardDiscountType.c_str()));

			//By defaul GEP should be displayed
			ui.cbDiscountCalcFrom->setCurrentIndex(0);
			//By defaul AGP should be displayed
			ui.cbDiscountApplyTo->setCurrentIndex(1);

		}

		void TransferOrderCreatorVC::setupOrderPosFrame()
		{
			ui.txtPos->setAccessibleName(libabbauw::properties::POSNO.getName().c_str());
			ui.txtQty->setAccessibleName(libabbauw::properties::ORDEREDQTY.getName().c_str());
			ui.txtNonChargedQty->setAccessibleName(libabbauw::properties::NONCHARGEDQTY.getName().c_str());
			ui.txtDiscountPCT->setAccessibleName(libabbauw::properties::DISCOUNTPCT.getName().c_str());
			ui.txtArticleName->setAccessibleName(libabbauw::properties::ARTICLENAME.getName().c_str());
			ui.txtUnit->setAccessibleName(libabbauw::properties::UNIT.getName().c_str());
			ui.txtPharmaceuticalForm->setAccessibleName(libabbauw::properties::PHARMACEUTICALFORM.getName().c_str());
			ui.cbDiscountCalcFrom->setCurrentIndex(0);
			ui.cbDiscountApplyTo->setCurrentIndex(1);

			ui.txtQty->setValidator(new QIntValidator(1, 99999, ui.txtQty));
			ui.txtNonChargedQty->setValidator(new QIntValidator(1, 99999, ui.txtNonChargedQty));

			ui.txtMatchCode->setValidator(new QRegExpValidator(QRegExp(".{0,12}"), ui.txtMatchCode));
			ui.txtPZN->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,13}"), ui.txtPZN));

			QDoubleValidator* discountPCTValidator = new QDoubleValidator(0, 999.99, 2, ui.txtDiscountPCT);
			discountPCTValidator->setNotation(QDoubleValidator::StandardNotation);

			ui.txtDiscountPCT->setValidator(discountPCTValidator);
			ui.txtDiscountAllPositions->setValidator(discountPCTValidator);
		}

		void TransferOrderCreatorVC::matchFromManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator manufacturerYit)
		{
			libutil::gui::SignalBlocker signalBlocker(ui.txtManufacturerNumber);
			if (!manufacturerYit.isEnd())
			{
				ui.txtManufacturerNumber->setText(QString::number(manufacturerYit.getInt32(libabbauw::properties::MANUFACTURERNO)));
				ui.txtManufacturerName->setText(manufacturerYit.getString(libabbauw::properties::MANUFACTURERNAME).c_str());
			}
			else
			{
				ui.txtManufacturerNumber->clear();
				ui.txtManufacturerName->clear();
			}
		}

		void TransferOrderCreatorVC::matchToManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator manufacturerYit)
		{
			manufacturerYit.setInt32(libabbauw::properties::MANUFACTURERNO, ui.txtManufacturerNumber->text().toInt());
		}

		void TransferOrderCreatorVC::matchFromPurchaseOrderSupplier(basar::db::aspect::AccessorPropertyTable_YIterator purchaseOrderSupplierYit)
		{
			libutil::gui::SignalBlocker signalBlocker(ui.txtPurchaseOrderSupplierName);
			if (!purchaseOrderSupplierYit.isEnd())
			{
				ui.txtPurchaseOrderSupplierName->setText(purchaseOrderSupplierYit.getString(libabbauw::properties::PURCHASEORDERSUPPLIERNAME).c_str());
			}
			else
			{
				ui.txtPurchaseOrderSupplierName->clear();
			}
		}

		void TransferOrderCreatorVC::matchFromPharmacy(basar::db::aspect::AccessorPropertyTable_YIterator pharmacyYit)
		{
			libutil::gui::SignalBlocker signalBlocker(ui.txtPharmacyNumber);
			if (!pharmacyYit.isEnd())
			{
				ui.txtPharmacyNumber->setText(QString::number(pharmacyYit.getInt32(libabbauw::properties::PHARMACYNO)));
				ui.txtPharmacyName->setText(pharmacyYit.getString(libabbauw::properties::PHARMACYNAME).c_str());
				ui.txtPharmacyLocation->setText(pharmacyYit.getString(libabbauw::properties::LOCATION).c_str());
			}
			else
			{
				if (!ui.txtPharmacyNumber->text().isEmpty())
					info(tr("Pharmacy number not found!").toLocal8Bit().constData());

				ui.txtPharmacyNumber->clear();
				ui.txtPharmacyName->clear();
				ui.txtPharmacyLocation->clear();
				ui.txtPharmacyNumber->setFocus();
			}
		}

		void TransferOrderCreatorVC::matchToPharmacy(basar::db::aspect::AccessorPropertyTable_YIterator pharmacyYit)
		{
			pharmacyYit.setInt32(libabbauw::properties::PHARMACYNO, ui.txtPharmacyNumber->text().toInt());
		}

		void TransferOrderCreatorVC::matchFromOrderNoMF(basar::db::aspect::AccessorPropertyTable_YIterator manufacturerYit)
		{
			libutil::gui::SignalBlocker signalBlocker(ui.txtOrderNoMF);
			if (!manufacturerYit.isEnd())
			{
			}
			else
			{

			}
		}

		void TransferOrderCreatorVC::matchToOrderNoMF(basar::db::aspect::AccessorPropertyTable_YIterator manufacturerYit)
		{
			manufacturerYit.setInt32(libabbauw::properties::ORDERNOMF, ui.txtOrderNoMF->text().toInt());
		}

		void TransferOrderCreatorVC::matchForHeadertext(basar::db::aspect::AccessorPropertyTable_YIterator manufacturerYit)
		{
			libutil::gui::SignalBlocker signalBlocker(ui.txtHeadertext);

			if (ui.chkHeadertext->isChecked() == true)
			{
				return;
			}

			QString ordernomf = ui.txtOrderNoMF->text().toStdString().c_str();
			QString pharmacyNumber = ui.txtPharmacyNumber->text().toStdString().c_str();
			QString pharmacyName = ui.txtPharmacyName->text().toStdString().c_str();

			QString headerText = "";
			QString manuNumber = "";
			QString manuName = "";

			if (!manufacturerYit.isEnd())
			{
				manuNumber = QString::number(manufacturerYit.getInt32(libabbauw::properties::MANUFACTURERNO));
				manuName = manufacturerYit.getString(libabbauw::properties::MANUFACTURERNAME).c_str();
			}

			headerText = m_headertext_pattern.c_str();

			headerText = replaceKey(headerText, "mfno", manuNumber);
			headerText = replaceKey(headerText, "ordernomf", ordernomf);
			headerText = replaceKey(headerText, "mfname", manuName);
			headerText = replaceKey(headerText, "pharmacyno", pharmacyNumber);
			headerText = replaceKey(headerText, "pharmacyname", pharmacyName);

			try
			{
				// get widget which has currently the focus
				QWidget* focusWidget = QApplication::focusWidget();

				// set or clear text, ensure to use max 30 chars as problems with KSC transfer of more
				ui.txtHeadertext->setText(headerText.left(30));

				// show text from left side
				ui.txtHeadertext->home(false);

                // set focus to previous widget if possible
                if (focusWidget) 
                {
                    focusWidget->setFocus();
                }
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
			}
		}

		QString TransferOrderCreatorVC::replaceKey(QString text, QString key, QString value)
		{
			key = "#" + key + "#";

			if (value == NULL)
				value = "";

			text = text.replace(key, value);

			return text;
		}

		void TransferOrderCreatorVC::matchFromArticle(basar::db::aspect::AccessorPropertyTable_YIterator articleYit)
		{
			basar::gui::tie::MatcherRef matchArticle = basar::gui::tie::Manager::getInstance().createMatcher(
				ui.grpAcquirePosition, articleYit.getPropertyTable());
			matchArticle.RightToLeft(matchArticle.getLeft().begin(), articleYit);

			if (articleYit.isEnd())
			{
				info(tr("Article for manufacturer not found!").toLocal8Bit().constData());
				if (!ui.txtMatchCode->text().isEmpty())
				{
					ui.txtMatchCode->setFocus();
				}
				else
				{
					ui.txtPZN->setFocus();
				}
				resetArticleLineEdits();
			}
			else
			{
				if (articleYit.isContainedAndSet(libabbauw::properties::PZN))
				{
					ui.txtArticleNo->setText((articleYit.getString(libabbauw::properties::PZN).c_str()));
					if (!ui.txtMatchCode->text().isEmpty()) // override entered matchcode with found one
						ui.txtMatchCode->setText((articleYit.getString(libabbauw::properties::MATCHCODE).c_str()));
				}
			}
		}

		void TransferOrderCreatorVC::matchToArticle(basar::db::aspect::AccessorPropertyTable_YIterator articleYit)
		{
			basar::gui::tie::MatcherRef matchArticle = basar::gui::tie::Manager::getInstance().createMatcher(
				ui.grpAcquirePosition, articleYit.getPropertyTable());

			matchArticle.LeftToRight();

			if (ui.txtMatchCode->text().isEmpty())
			{
				//WORKAROUND BASAR Bug: clear old values
				articleYit.setPropertyState(libabbauw::properties::MATCHCODE, basar::SS_UNSET);
			}
			else
			{
				articleYit.setString(libabbauw::properties::MATCHCODE, ui.txtMatchCode->text().toLocal8Bit().constData());
			}

			if (ui.txtPZN->text().isEmpty())
			{    //WORKAROUND BASAR Bug
				articleYit.setPropertyState(libabbauw::properties::PZN, basar::SS_UNSET);
				articleYit.setPropertyState(libabbauw::properties::EAN, basar::SS_UNSET);
			}
			else // pzn/ean set
			{
				// FIXME: this better has to be done via an injected DM interface later
				if (ui.txtPZN->text().length() >= 13) // EAN
				{
					articleYit.setString(libabbauw::properties::EAN, ui.txtPZN->text().toLocal8Bit().constData());
				}
				else // normal PZN
				{
					articleYit.setString(libabbauw::properties::PZN, ui.txtPZN->text().toLocal8Bit().constData());
				}
			}

			if (ui.txtManufacturerNumber->text().isEmpty())
			{
				articleYit.setPropertyState(libabbauw::properties::MANUFACTURERNO, basar::SS_UNSET);
			}
			else
			{
				articleYit.setInt32(libabbauw::properties::MANUFACTURERNO, ui.txtManufacturerNumber->text().toInt());
			}
		}

		void  TransferOrderCreatorVC::matchFromTransferOrderHeadInPreparation()
		{
			libutil::gui::SignalBlocker signalblocker2(ui.txtPharmacyNumber);
			libutil::gui::SignalBlocker signalblocker3(ui.cboOrderType);
			libutil::gui::SignalBlocker signalblocker4(ui.dtDeferredPaymentDate);
			libutil::gui::SignalBlocker signalblocker5(ui.dtDeliveryDate);

			clearHeadInputFields();
			m_OrderHeadMatcher.RightToLeft(m_OrderHeadMatcher.getLeft().begin(), m_Head);

			// matcher doesn't work with combobox, so match manually here
			if (m_Head.isContainedAndSet(libabbauw::properties::ORDERTYPE))
			{
				ui.cboOrderType->setCurrentIndex(ui.cboOrderType->findText(m_Head.getString(libabbauw::properties::ORDERTYPE.getName()).c_str()));
			}

			// BASAR BUG: minimum date is invalid
			const bool isEdiDateSet = m_Head.isContainedAndSet(libabbauw::properties::EDIDELIVERYDATE) &&
				m_Head.getDate(libabbauw::properties::EDIDELIVERYDATE).isValid() &&
				m_Head.getDate(libabbauw::properties::EDIDELIVERYDATE).getDate() >= basar::cmnutil::Date::getCurrent().getDate();
			ui.chkDeliveryDate->setChecked(isEdiDateSet);
			ui.dtDeliveryDate->setEnabled(isEdiDateSet);
			if (isEdiDateSet)
			{
				const basar::Date& ediDate = m_Head.getDate(libabbauw::properties::EDIDELIVERYDATE);
				ui.dtDeliveryDate->setDate(QDate(ediDate.getYear(), ediDate.getMonth(), ediDate.getDay()));
			}

			// BASAR BUG: minimum date is invalid
			const bool isDeferredPaymentDateSet = m_Head.isContainedAndSet(libabbauw::properties::DEFERREDPAYMENTDATE) &&
				m_Head.getDate(libabbauw::properties::DEFERREDPAYMENTDATE).isValid() &&
				m_Head.getDate(libabbauw::properties::DEFERREDPAYMENTDATE).getDate() >= basar::cmnutil::Date::getCurrent().getDate();
			ui.chkDeferredPaymentDate->setChecked(isDeferredPaymentDateSet);
			ui.txtDeferredPaymentDate->setEnabled(isDeferredPaymentDateSet);
			ui.dtDeferredPaymentDate->setEnabled(isDeferredPaymentDateSet);
			if (isDeferredPaymentDateSet)
			{
				const basar::Date& paymentDate = m_Head.getDate(libabbauw::properties::DEFERREDPAYMENTDATE);
				ui.dtDeferredPaymentDate->setDate(QDate(paymentDate.getYear(), paymentDate.getMonth(), paymentDate.getDay()));
			}

			if (!ui.txtPharmacyNumber->text().isEmpty())
			{
				ui.txtQty->setFocus();
			}
		}

		void TransferOrderCreatorVC::changedDateToHead(const QDate& date, const basar::property::PropertyDescription& prop)
		{
			basar::Date bdate;
			bdate.setInvalid();

			if (m_Head.isContainedAndSet(prop))
			{
				bdate = m_Head.getDate(prop);
			}

			if (bdate.getDate() != basar::Date(date.year(), date.month(), date.day()).getDate())
			{
				m_Head.setDate(prop, basar::Date(date.year(), date.month(), date.day()));
				m_Head.setPropertyState(prop, basar::SS_UPDATE);
			}
		}

		void TransferOrderCreatorVC::correctPropertyState(const basar::property::PropertyDescription& prop)
		{
			if (m_Head.getState().contains(basar::SS_DELETE))
			{
				// Basar doesn't allow override SS_DELETE manually so this hack is necessary :(
				basar::VarString user = m_Head.getString(libabbauw::properties::USER);
				basar::Int16 branchno = m_Head.getInt16(libabbauw::properties::BRANCHNO);

				m_Head.setPropertyState(libabbauw::properties::USER, basar::SS_UNSET);
				m_Head.setString(libabbauw::properties::USER, user);

				m_Head.setPropertyState(libabbauw::properties::BRANCHNO, basar::SS_UNSET);
				m_Head.setInt16(libabbauw::properties::BRANCHNO, branchno);

				// changed values need to be in SS_INSERT or SS_UPDATE state to recognize 'em later in DM
				// state transitions don't work correctly within Basar and after a value has been set to SS_UNSET
				// it transitions to SS_CLEAN next which is wrong here
				if (m_Head.getState(prop).getState() != basar::SS_INSERT)
					m_Head.setPropertyState(prop, basar::SS_UPDATE);
			}
		}

		void  TransferOrderCreatorVC::matchToTransferOrderHeadInPreparation()
		{
			m_OrderHeadMatcher.LeftToRight(m_OrderHeadMatcher.getLeft().begin(), m_Head);

			// matcher doesn't work with combobox, so do it manually here
			QString text = ui.cboOrderType->currentText();
			if (!text.isEmpty())
			{
				m_Head.setString(libabbauw::properties::ORDERTYPE, text.toLocal8Bit().constData());

				//if( ui.cboOrderType->currentIndex() > 0 )
				if (ui.cboOrderType->currentIndex() != ui.cboOrderType->findText(m_StandardAART.c_str()))
				{
					correctPropertyState(libabbauw::properties::ORDERTYPE);
				}
			}

			if (ui.chkDeliveryDate->isChecked())
			{
				changedDateToHead(ui.dtDeliveryDate->date(), libabbauw::properties::EDIDELIVERYDATE);
				correctPropertyState(libabbauw::properties::EDIDELIVERYDATE);
			}
			else
			{
				basar::Date date; date.setInvalid();
				m_Head.setDate(libabbauw::properties::EDIDELIVERYDATE, date);
			}

			if (ui.chkDeferredPaymentDate->isChecked())
			{
				changedDateToHead(ui.dtDeferredPaymentDate->date(), libabbauw::properties::DEFERREDPAYMENTDATE);
				correctPropertyState(libabbauw::properties::DEFERREDPAYMENTDATE);
			}
			else
			{
				basar::Date date; date.setInvalid();
				m_Head.setDate(libabbauw::properties::DEFERREDPAYMENTDATE, date);
			}

			if (ui.txtPharmacyNumber->text().isEmpty())
			{
				m_Head.setPropertyState(libabbauw::properties::PHARMACYNO, basar::SS_UNSET);
			}
			else
			{
				correctPropertyState(libabbauw::properties::PHARMACYNO);
			}

			if (ui.txtManufacturerNumber->text().isEmpty())
			{
				m_Head.setPropertyState(libabbauw::properties::MANUFACTURERNO, basar::SS_UNSET);
			}
			else
			{
				correctPropertyState(libabbauw::properties::MANUFACTURERNO);
			}
		}

		void  TransferOrderCreatorVC::matchFromTransferOrderPosInPreparationCollection()
		{
			METHODNAME_DEF(TransferOrderCreatorVC, matchFromTransferOrderPosInPreparationCollection);
			libutil::gui::SignalBlocker signalblocker(ui.twBatchOrderPositions);

			ui.twBatchOrderPositions->clearContents();
			ui.twBatchOrderPositions->setRowCount(static_cast<int>(m_Positions.size()));

			basar::Int32 matchedHits = m_OrderPosMatcher.RightToLeft(0, ui.twBatchOrderPositions->rowCount());
			ui.twBatchOrderPositions->setRowCount(matchedHits);

			const basar::UInt32 numberOfPos = m_OrderPosMatcher.getLeft().size();
			if (numberOfPos != 1)
			{
				ui.lblAquisition->setText(tr("%1 positions acquired").arg(numberOfPos));
			}
			else
			{
				ui.lblAquisition->setText(tr("%1 position acquired").arg(numberOfPos));
			}

			ValidateAndSetPositionsStatus();

			resetPositionLineEdits();
		}

		void TransferOrderCreatorVC::matchFromPosition(basar::db::aspect::AccessorPropertyTable_YIterator posYit)
		{
			basar::gui::tie::MatcherRef posMatcher = basar::gui::tie::Manager::getInstance().createMatcher(
				ui.grpAcquirePosition, posYit.getPropertyTable());

			posMatcher.RightToLeft(posMatcher.getLeft().begin(), posYit);

			// transfer the discount flags from db to comboboxes
			basar::Int16 dcfui = posYit.getInt16(libabbauw::properties::DISCOUNTCALCFROM);
			basar::Int16 datui = posYit.getInt16(libabbauw::properties::DISCOUNTAPPLYTO);
			ui.cbDiscountCalcFrom->setCurrentIndex(dcfui);
			ui.cbDiscountApplyTo->setCurrentIndex(datui);
		}

		void TransferOrderCreatorVC::matchToPosition(basar::db::aspect::AccessorPropertyTable_YIterator posYit)
		{
			basar::gui::tie::MatcherRef posMatcher = basar::gui::tie::Manager::getInstance().createMatcher(
				ui.grpAcquirePosition, posYit.getPropertyTable());


			posMatcher.LeftToRight(posMatcher.getLeft().begin(), posYit);

			if (!ui.txtArticleNo->text().isEmpty())
			{
				posYit.setString(libabbauw::properties::PZN, ui.txtArticleNo->text().toLocal8Bit().constData());
			}

			// transfer the discount flags from comboboxes to db result
			if (posYit.isContainedAndSet(libabbauw::properties::DISCOUNTCALCFROM) == true)
			{
				basar::Int16 dcfui = ui.cbDiscountCalcFrom->currentIndex();
				posYit.setInt16(libabbauw::properties::DISCOUNTCALCFROM, dcfui);
				posYit.setPropertyState(libabbauw::properties::DISCOUNTCALCFROM.getName().c_str(), basar::SS_UPDATE);
			}
			if (posYit.isContainedAndSet(libabbauw::properties::DISCOUNTAPPLYTO) == true)
			{
				basar::Int16 datui = ui.cbDiscountApplyTo->currentIndex();
				posYit.setInt16(libabbauw::properties::DISCOUNTAPPLYTO, datui);
				posYit.setPropertyState(libabbauw::properties::DISCOUNTAPPLYTO.getName().c_str(), basar::SS_UPDATE);
			}
		}

		basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderCreatorVC::getSelectedPosition()
		{
			return m_OrderPosMatcher.getSelectedRight();
		}

		void TransferOrderCreatorVC::setNextPositionNo(basar::Int32 no)
		{
			ui.txtPos->setText(QString::number(no));
		}

		void TransferOrderCreatorVC::switchToEditingMode(bool editMode)
		{
			// disconnect all old functions
			ui.btnAddChange->disconnect();

			if (editMode)
			{
				ui.btnAddChange->setText(tr("&Change Position"));
				QObject::connect(ui.btnAddChange, SIGNAL(clicked()),
					this, SLOT(onBtnChangePosClicked()));
			}
			else
			{
				ui.btnAddChange->setText(tr("&Add Position"));
				QObject::connect(ui.btnAddChange, SIGNAL(clicked()),
					this, SLOT(onBtnAddPosClicked()));
			}
		}

		void TransferOrderCreatorVC::setSavingAllowed(bool isEnabled)
		{
			ui.btnSave->setEnabled(isEnabled);
		}

		void TransferOrderCreatorVC::setClearBtnEnableState()
		{
			const int calcFrom = ui.cbDiscountCalcFrom->currentIndex();
			const int applyTo = ui.cbDiscountApplyTo->currentIndex();

			const bool empty = ui.txtQty->text().isEmpty() && ui.txtDiscountPCT->text().isEmpty() &&
				ui.txtNonChargedQty->text().isEmpty() && ui.txtPZN->text().isEmpty() &&
				ui.txtMatchCode->text().isEmpty() && calcFrom != 0 && applyTo != 1;

			ui.btnDiscard->setEnabled(!empty);
		}

		void TransferOrderCreatorVC::setCreatingTransferOrderAllowed(bool isEnabled)
		{
			const bool inputFieldEmpty = ui.txtManufacturerNumber->text().isEmpty() || ui.txtPharmacyNumber->text().isEmpty();
			ui.btnCreateTransferOrder->setEnabled(isEnabled && !inputFieldEmpty);
		}

		void TransferOrderCreatorVC::closeEvent(QCloseEvent* e)
		{
			if (ui.btnSave->isEnabled())
			{
				QMessageBox::StandardButton resBtn = QMessageBox::question(this, tr("Save changes?"),
					tr("Do you want to save the changes you made?\n"),
					QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
					QMessageBox::Yes);

				if (resBtn == QMessageBox::Yes)
				{
					basar::appl::SystemEventManager::getInstance().fire("SaveTransferOrderInPreparationRequested");
				}
				else if (resBtn == QMessageBox::Cancel)
				{
					e->ignore();
				}
			}
		}

		void TransferOrderCreatorVC::onBtnCloseClicked()
		{
			basar::appl::SystemEventManager::getInstance().fire("DialogCloseRequested");
		}

		void TransferOrderCreatorVC::onBtnSearchArticleClicked()
		{
			basar::appl::SystemEventManager::getInstance().fire("FindArticleRequested");
		}

		void TransferOrderCreatorVC::onBtnAddPosClicked()
		{
			bool existing = false;
			for (basar::db::aspect::AccessorPropertyTable_YIterator position = m_Positions.begin(); !position.isEnd() && !existing; ++position)
			{
				if (QString::fromStdString(position.getString(libabbauw::properties::PZN)) == ui.txtArticleNo->text() && position.getState(libabbauw::properties::PZN) != basar::SS_DELETE)
				{
					QString msg = tr("The article %1 is already contained in this order! Add?").arg(ui.txtArticleName->text());
					if (question(msg.toLocal8Bit().constData()) == basar::gui::tie::ButtonNo)
					{
						resetPositionLineEdits();
						ui.txtQty->setFocus();
						onDiscountAllPositionsChanged(ui.txtDiscountAllPositions->text());
						onDiscountAllPositionsTypeChanged(ui.cbDiscountTypeAllPositions->currentIndex());
						return;
					}
					existing = true;
				}
			}

			basar::appl::SystemEventManager::getInstance().fire("AddPositionRequested");
			ui.btnAddChange->setEnabled(false);
			onDiscountAllPositionsChanged(ui.txtDiscountAllPositions->text());
			onDiscountAllPositionsTypeChanged(ui.cbDiscountTypeAllPositions->currentIndex());
			ui.txtQty->setFocus();

			ui.twBatchOrderPositions->scrollToBottom();
		}

		void TransferOrderCreatorVC::onBtnDiscardClicked()
		{
			resetPositionLineEdits();
			ui.twBatchOrderPositions->clearSelection();
			onDiscountAllPositionsChanged(ui.txtDiscountAllPositions->text());
			onDiscountAllPositionsTypeChanged(0);
			ui.txtQty->setFocus();
		}

		void TransferOrderCreatorVC::onBtnChangePosClicked()
		{
			basar::appl::SystemEventManager::getInstance().fire("ChangePositionRequested");
			ui.btnAddChange->setEnabled(false);
			ui.btnRemove->setEnabled(m_OrderPosMatcher.getCurrentRight() != m_Positions.end());
			onDiscountAllPositionsChanged(ui.txtDiscountAllPositions->text());
			onDiscountAllPositionsTypeChanged(ui.cbDiscountTypeAllPositions->currentIndex());
			ui.txtQty->setFocus();
		}

		void TransferOrderCreatorVC::onBtnRemovePosClicked()
		{
			int deletedRow = ui.twBatchOrderPositions->currentRow();
			basar::appl::SystemEventManager::getInstance().fire("RemovePositionRequested");
			ui.twBatchOrderPositions->setFocus();
			ui.btnRemove->setEnabled(m_OrderPosMatcher.getSelectedRight() != m_Positions.end());
			ui.btnAddChange->setEnabled(false);

			int twRowCount = ui.twBatchOrderPositions->rowCount();

			if (twRowCount >= 1)
			{
				if (deletedRow == twRowCount)
				{   // last entry deleted - select the one before instead
					ui.twBatchOrderPositions->selectRow(twRowCount - 1);
				}
				else
					ui.twBatchOrderPositions->selectRow(deletedRow);
			}
			else // list empty
				ui.txtQty->setFocus();

			onDiscountAllPositionsChanged(ui.txtDiscountAllPositions->text());
			onDiscountAllPositionsTypeChanged(ui.cbDiscountTypeAllPositions->currentIndex());
			ui.cbDiscountCalcFrom->setCurrentIndex(0);
			ui.cbDiscountApplyTo->setCurrentIndex(1);
		}

		void TransferOrderCreatorVC::onBtnResetClicked()
		{
			basar::appl::SystemEventManager::getInstance().fire("ResetTransferOrderInPreparationRequested");
			ui.btnAddChange->setEnabled(false);
			ui.btnRemove->setEnabled(false);
			ui.txtDiscountAllPositions->clear();
			ui.txtPharmacyNumber->setFocus();
			ui.cbDiscountType->setCurrentIndex(ui.cbDiscountType->findText(m_StandardDiscountType.c_str()));
			ui.cbDiscountTypeAllPositions->setCurrentIndex(ui.cbDiscountTypeAllPositions->findText(m_StandardDiscountType.c_str()));
			ui.cbDiscountCalcFrom->setCurrentIndex(0);
			ui.cbDiscountApplyTo->setCurrentIndex(1);
		}

		void TransferOrderCreatorVC::onBtnSaveClicked()
		{
			basar::appl::SystemEventManager::getInstance().fire("SaveTransferOrderInPreparationRequested");
		}

		void TransferOrderCreatorVC::onBtnCreateTransferOrder()
		{
			basar::appl::SystemEventManager::getInstance().fire("CreateTransferOrderRequested");
		}

		void TransferOrderCreatorVC::onBtnSearchPharmacy()
		{
			basar::appl::SystemEventManager::getInstance().fire("FindPharmacyRequested");
		}

		void TransferOrderCreatorVC::onPharmacyNoChanged()
		{
			if (!ui.txtPharmacyNumber->text().isEmpty())
			{
				basar::appl::SystemEventManager::getInstance().fire("CheckPharmacyNoRequested");
			}
			else
			{
				ui.txtPharmacyName->clear();
			}

			basar::appl::SystemEventManager::getInstance().fire("CheckSaveStateRequested");
			basar::appl::SystemEventManager::getInstance().fire("CheckCreateTransferOrderStateRequested");
		}

		void TransferOrderCreatorVC::onOrderNoMFChanged()
		{
			if (!ui.txtOrderNoMF->text().isEmpty())
			{

			}
			else
			{
				ui.txtOrderNoMF->clear();
			}

			basar::appl::SystemEventManager::getInstance().fire("CheckOrderNoMFRequested");
			basar::appl::SystemEventManager::getInstance().fire("CheckSaveStateRequested");
			basar::appl::SystemEventManager::getInstance().fire("CheckCreateTransferOrderStateRequested");
		}

		void TransferOrderCreatorVC::onHeadertextChanged()
		{
			if (!ui.txtHeadertext->text().isEmpty())
			{
				basar::appl::SystemEventManager::getInstance().fire("CheckHeadertextRequested");
			}
			else
			{
				ui.txtHeadertext->clear();
			}

			basar::appl::SystemEventManager::getInstance().fire("CheckSaveStateRequested");
			basar::appl::SystemEventManager::getInstance().fire("CheckCreateTransferOrderStateRequested");
		}

		void TransferOrderCreatorVC::onDiscountPCTEditingFinished()
		{
			// int validator does allow numbers as big as 999.99 so reduce it to 100.00% here
			bool valid = false;
			double test = locale().toDouble(ui.txtDiscountPCT->text(), &valid);

			if (!valid)
			{
				ui.txtDiscountPCT->setText("");
				info(tr("Entered DPCT is invalid!").toLocal8Bit().constData());
				ui.txtDiscountPCT->setFocus();
			}
			else if (test > 100.00)
			{
				ui.txtDiscountPCT->setText(locale().toString(100.00, 'f', 2));
				info(tr("DPCT was reduced to 100.00%!").toLocal8Bit().constData());
			}
		}

		void TransferOrderCreatorVC::onDefPaymentDtAddDays(const QString& daysToAdd)
		{
			ui.dtDeferredPaymentDate->setDate(QDate::currentDate().addDays(daysToAdd.toInt()));
		}

		void TransferOrderCreatorVC::onDiscountAllPositionsChanged(const QString& br)
		{
			ui.txtDiscountPCT->setText(br);
		}

		void TransferOrderCreatorVC::onDiscountAllPositionsTypeChanged(int index)
		{
			ui.cbDiscountType->setCurrentIndex(index);
		}

		void TransferOrderCreatorVC::onDiscountAllPositionsFinished()
		{
			// int validator does allow numbers as big as 999.99 so reduce it to 100.00% here
			bool valid = false;
			double test = locale().toDouble(ui.txtDiscountAllPositions->text(), &valid);

			if (!valid)
			{
				ui.txtDiscountAllPositions->setText("");
				info(tr("Entered DPCT is invalid!").toLocal8Bit().constData());
				ui.txtDiscountPCT->setFocus();
			}
			else if (test > 100.00)
			{
				ui.txtDiscountAllPositions->setText(locale().toString(100.00, 'f', 2));
				info(tr("DPCT was reduced to 100.00%!").toLocal8Bit().constData());
			}
		}

		void TransferOrderCreatorVC::onArticleChanged()
		{
			libutil::gui::SignalBlocker sb1(ui.txtMatchCode);
			libutil::gui::SignalBlocker sb2(ui.txtPZN);

			const bool textChanged = m_lastPZN != ui.txtPZN->text() || m_lastMatchCode != ui.txtMatchCode->text();

			if ((!ui.txtPZN->text().isEmpty() || ui.txtMatchCode->text().length() >= 3) && textChanged)
			{
				CSC_Batches::gui::WaitCursor cursor(this);
				basar::appl::SystemEventManager::getInstance().fire("CheckArticleRequested");
			}

			m_lastPZN = ui.txtPZN->text();
			m_lastMatchCode = ui.txtMatchCode->text();
		}

		void TransferOrderCreatorVC::onArticleSearchTyped(const QString&)
		{
			ui.txtPZN->setEnabled(true);
			ui.txtMatchCode->setEnabled(true);

			if (!ui.txtMatchCode->text().isEmpty())
			{
				QToolTip::showText(ui.txtMatchCode->mapToGlobal(QPoint(5, 5)), tr("Manufacturer specific article search: \nEnter at least 3 characters"));
				ui.txtPZN->setEnabled(false);

				if (ui.txtMatchCode->text().length() < 3)
					QToolTip::showText(ui.txtMatchCode->mapToGlobal(QPoint(5, 5)), tr("Enter at least 3 characters"));
			}
			else if (!ui.txtPZN->text().isEmpty())
			{
				ui.txtMatchCode->setEnabled(false);
			}
		}

		void TransferOrderCreatorVC::onPosEditCheck(const QString&)
		{
			// FIXME: this better has to be done via validator in DM
			bool isQtyValid = false;
			if (ui.txtQty->text().toInt() > 0)
				isQtyValid = true;
			else if (ui.txtQty->text().toInt() == 0 && ui.txtNonChargedQty->text().toInt() > 0)
				isQtyValid = true;

			ui.btnAddChange->setEnabled(!ui.txtArticleNo->text().isEmpty() && isQtyValid);
			setClearBtnEnableState();
		}

		void TransferOrderCreatorVC::cboOrderTypeChanged(int)
		{
			basar::appl::SystemEventManager::getInstance().fire("CheckSaveStateRequested");
			basar::appl::SystemEventManager::getInstance().fire("CheckCreateTransferOrderStateRequested");
		}

		void TransferOrderCreatorVC::onDateChanged(const QDate&)
		{
			basar::appl::SystemEventManager::getInstance().fire("CheckSaveStateRequested");
			basar::appl::SystemEventManager::getInstance().fire("CheckCreateTransferOrderStateRequested");
		}

		void TransferOrderCreatorVC::onDeliveryDateChecked(bool check)
		{
			if (check == false)
				ui.dtDeliveryDate->setDate(QDate::currentDate());

			basar::appl::SystemEventManager::getInstance().fire("CheckSaveStateRequested");
			ui.dtDeliveryDate->setEnabled(check);
		}

		void TransferOrderCreatorVC::onDeferredPaymentDateChecked(bool check)
		{
			if (check == false)
				ui.dtDeferredPaymentDate->setDate(QDate::currentDate());

			basar::appl::SystemEventManager::getInstance().fire("CheckSaveStateRequested");
			ui.dtDeferredPaymentDate->setEnabled(check);
			ui.txtDeferredPaymentDate->setEnabled(check);
		}

		void TransferOrderCreatorVC::onManualHeadertextChecked(bool check)
		{
			basar::appl::SystemEventManager::getInstance().fire("CheckSaveStateRequested");
			basar::appl::SystemEventManager::getInstance().fire("CheckManualHeadertextRequested");
			ui.txtHeadertext->setEnabled(check);
		}

		void TransferOrderCreatorVC::onTableDataSelectionChanged()
		{
			resetPositionLineEdits();

			if (m_OrderPosMatcher.getSelectedRight() != m_Positions.end())
			{
				ui.btnRemove->setEnabled(true);
				basar::appl::SystemEventManager::getInstance().fire("SwitchToPosEditModeRequested");

				if (m_OrderPosMatcher.getCurrentRight().isContainedAndSet(libabbauw::properties::PZN))
				{
					ui.txtArticleNo->setText(m_OrderPosMatcher.getCurrentRight().getString(libabbauw::properties::PZN).c_str());
				}
				ui.cbDiscountCalcFrom->setCurrentIndex(m_OrderPosMatcher.getCurrentRight().getInt16(libabbauw::properties::DISCOUNTCALCFROM));
				ui.cbDiscountApplyTo->setCurrentIndex(m_OrderPosMatcher.getCurrentRight().getInt16(libabbauw::properties::DISCOUNTAPPLYTO));
			}
			else
			{
				ui.btnRemove->setEnabled(false);
				basar::appl::SystemEventManager::getInstance().fire("SwitchToPosInputModeRequested");
			}
		}

		void TransferOrderCreatorVC::onSelectPositionShortcut()
		{
			// toggle focus between list and Qty input field
			if (ui.twBatchOrderPositions->hasFocus())
			{
				ui.txtQty->setFocus();
				ui.txtQty->selectAll();
			}
			else
			{
				// select last row for editing
				ui.twBatchOrderPositions->selectRow(ui.twBatchOrderPositions->rowCount() - 1);
				ui.twBatchOrderPositions->setFocus();
			}
		}

		void TransferOrderCreatorVC::showOrderNoInfoMsg(const basar::Int32 orderno)
		{
			info((tr("A new transferOrder with ordernumber '%1' was created").arg(orderno)).toLocal8Bit().constData());
		}

		bool TransferOrderCreatorVC::eventFilter(QObject* target, QEvent* event)
		{
			if (event->type() == QEvent::KeyPress)
			{
				switch (static_cast<QKeyEvent*>(event)->key())
				{
				case Qt::Key_Enter: // numlock enter has been pressed
					if (target != ui.btnSearchArticle || (target == ui.btnSearchArticle && ui.btnAddChange->isEnabled()))
					{
						focusNextChild();
						return true;
					}
					break;

				case Qt::Key_Escape:
					close();
					return true;
				}
			}

			return QObject::eventFilter(target, event);
		}

	} // end namespace viewConn
} // end namespace CSC_Batches
