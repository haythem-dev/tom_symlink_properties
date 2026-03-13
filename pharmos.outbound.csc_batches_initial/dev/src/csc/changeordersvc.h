#ifndef CSC_BATCHES_VIEWCONN_CHANGEORDERSVC_H
#define CSC_BATCHES_VIEWCONN_CHANGEORDERSVC_H

#include "ichangeordersvc.h"
#include "libabbauw/iordertypecollectionparameterptr.h"

#include <QtWidgets/QDialog>
#include <libbasarguitie_iviewconnector.h>

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include "ui_changeordersvc.h"
#pragma warning (pop)

#include <libbasarguitie_matcher.h>
#include <libutil/flag.h>

namespace CSC_Batches
{
	namespace viewConn
	{

		class ChangeOrdersVC : public QDialog, public IChangeOrdersVC, public basar::gui::tie::IViewConnector
		{
			Q_OBJECT

		public:
			ChangeOrdersVC(QWidget* parent = 0);
			~ChangeOrdersVC();

			void                                                        injectParameter(libabbauw::domMod::parameter::IOrderTypeCollectionParameterPtr);

			// from IChangeOrdersVC interface
			virtual void                                                init(basar::db::aspect::AccessorPropertyTableRef);
			virtual basar::gui::tie::WidgetReturnEnum	                show();
			virtual void								                hide() {}

			virtual bool								                shutdown();
			virtual void                                                waitForEvents();

			virtual void                                                info(const basar::I18nString&);
			virtual void                                                error(const basar::I18nString&);
			virtual const basar::gui::tie::MessageBoxButtonEnum         question(const basar::I18nString&);

			virtual void                                                setSavingAllowed(bool allowed);

			virtual void                                                matchToManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator);
			virtual void												matchFromManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator);

			virtual void                                                matchToPharmacy(basar::db::aspect::AccessorPropertyTable_YIterator);
			virtual void												matchFromPharmacy(basar::db::aspect::AccessorPropertyTable_YIterator);

			virtual void												matchFromOrderDiscount(basar::db::aspect::AccessorPropertyTable_YIterator);
			virtual void                                                matchToOrderDiscount(basar::db::aspect::AccessorPropertyTable_YIterator);
			virtual basar::VarString				     			    getOrderDiscount();

			virtual void                                                matchToOrders();
			virtual void										        matchFromOrders();

		private:
			void                                                        resetForm();
			void                                                        clearFields();
			void                                                        clearTable();
			void                                                        alignTableColumns();
			void                                                        wireEvents();
			void                                                        setupComboBoxes();
			void                                                        labelWindowTitle();
			void                                                        initHeaderLables(QStringList& lstLabels);
			void                                                        initTable(QStringList& lstLabels);
			void                                                        initMatcher(basar::db::aspect::AccessorPropertyTableRef);

		private slots:
			void                                                        onBtnApplylClicked();
			void														onBtnCancelClicked();
			void                                                        onBtnSaveClicked();
			void														onBtnSearchPharmacyClicked();
			void														onBtnSearchManufacturerClicked();
			void														onBtnDeleteDeliveryDateClicked();
			void														onBtnDeleteDeferredPaymentDateClicked();
			void                                                        onPharmacyNoChanged();
			void                                                        onMNoChanged();
			void														onOrderTypeChanged();
			void														onOrderDiscountChanged();

		private:
			Ui::ChangeOrdersVCClass											ui;
			log4cplus::Logger												m_Logger;
			libutil::misc::Flag												m_EventsWiring;
			libabbauw::domMod::parameter::IOrderTypeCollectionParameterPtr	m_OrderTypeCollectionParameter;

			std::list< basar::VarString >									m_ListAttributes;
			basar::gui::tie::MatcherRef										m_OrderHeadCollMatcher;
		};

	} // end namespace viewConn
} // end namespace CSC_Batches

#endif // CSC_BATCHES_VIEWCONN_CHANGEORDERSVC_H
