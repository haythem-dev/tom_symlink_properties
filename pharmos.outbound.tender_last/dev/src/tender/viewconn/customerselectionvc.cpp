#include "customerselectionvc.h"
#include "loggerpool/tenderloggerpool.h"
#include "libtender/domainmodule/libtender_properties_definitions.h"

#include <util.h>

namespace tender
{
    namespace viewConn
    {
    	CustomerSelectionVC::CustomerSelectionVC(QWidget* parent) : BaseVC(parent),
    	 m_EventsWiring("CustomerSelectionVC : EventsWiring")
    	{
    		ui.setupUi(this);
    		wireEvents();

            initCustomerHeaderLabels();

    		setupCustomerTable();

			ui.btnTakeOver->setDisabled(true);
		}
    
		CustomerSelectionVC::~CustomerSelectionVC()
		{ }

		void CustomerSelectionVC::wireEvents()
		{
			if(m_EventsWiring.isSet())
			{
				return;
			}

			connect(ui.btnCancel,   SIGNAL( clicked()                      ),
			this,                   SLOT  ( on_DialogClose_Requested()     ));

			connect( ui.twCustomer, SIGNAL( activated(const QModelIndex &) ),
            this,                   SLOT  ( on_TakeOver_Requested()        ));

			connect(ui.btnTakeOver, SIGNAL( clicked()                      ),
			this,                   SLOT  ( on_TakeOver_Requested()        ));

			m_EventsWiring.set();
		}

		void CustomerSelectionVC::matchFromCustomerCollection(basar::db::aspect::AccessorPropertyTableRef propTab)
		{
			METHODNAME_DEF(CustomerSelectionVC, matchFromCustomerCollection)
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun);

			ui.twCustomer->setDisabled( true );

			basar::Int32 rowCount = initMatcher(propTab);

			libutil::gui::SignalBlocker signalblocker(ui.twCustomer);
            ui.twCustomer->setRowCount(rowCount);
			m_CustomerMatcher.RightToLeft(0, ui.twCustomer->rowCount());
			signalblocker.unblock();

			ui.twCustomer->resizeColumnsToContents();

			if(rowCount)
            {
                ui.twCustomer->setEnabled( true );

                ui.twCustomer->selectRow(0);
                ui.twCustomer->setFocus();

				ui.btnTakeOver->setEnabled( true );
            }
		}

		void CustomerSelectionVC::setSelectedCustomer(basar::db::aspect::AccessorPropertyTable_YIterator selected)
		{
			METHODNAME_DEF( CustomerSelectionVC, setSelectedCustomer )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_PickedCustomer = selected;
		}

		basar::db::aspect::AccessorPropertyTable_YIterator CustomerSelectionVC::getSelectedCustomer()
		{
			METHODNAME_DEF( CustomerSelectionVC, getSelectedCustomer )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			return m_PickedCustomer;
		}

		basar::gui::tie::WidgetReturnEnum CustomerSelectionVC::show()
        {
            METHODNAME_DEF( CustomerSelectionVC, show )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );
			
			if(!m_PickedCustomer.isNull() && !m_PickedCustomer.isEnd() && m_PickedCustomer.arePropertiesSet(properties::CUSTOMER_NO))
            {
                basar::Int32 customer =  m_PickedCustomer.getInt32(properties::CUSTOMER_NO);

                QList<QTableWidgetItem*> items = ui.twCustomer->findItems(QString::number(customer), Qt::MatchExactly);
                ui.twCustomer->selectRow(items.first()->row());
            }

            return basar::gui::tie::getWidgetReturnType(exec());
        }

		void CustomerSelectionVC::clear()
		{
			METHODNAME_DEF( CustomerSelectionVC, clear )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twCustomer->setRowCount( 0 );
			ui.twCustomer->setDisabled( true );

			ui.btnTakeOver->setDisabled( true );
		}

		void CustomerSelectionVC::setupCustomerTable()
		{
			METHODNAME_DEF(CustomerSelectionVC, setupCustomerTable)
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun);

		    ui.twCustomer->setABCVerticalHeaderPolicy  ();
            ui.twCustomer->setColumnCount              (m_LstLabels.size());
            ui.twCustomer->setHorizontalHeaderLabels   (m_LstLabels);
            ui.twCustomer->setResizeRowsToContents     (true);
            ui.twCustomer->setResizeColumnsToContents  (true);
            ui.twCustomer->setVerticalScrollBarPolicy  (Qt::ScrollBarAlwaysOn);
            ui.twCustomer->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            ui.twCustomer->setSelectionBehavior        (QAbstractItemView::SelectRows);
            ui.twCustomer->setSelectionMode            (QAbstractItemView::SingleSelection);
			ui.twCustomer->setSortingEnabled           (false);
			ui.twCustomer->setDisabled                 (true );
		}

		void CustomerSelectionVC::initCustomerHeaderLabels()
		{
			METHODNAME_DEF( CustomerSelectionVC, initArticleHeaderLabels )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			m_LstLabels.push_back            ( QApplication::translate("CustomerSelectionVC", "Customer No.") );
			m_LstOriginalAttributes.push_back( properties::CUSTOMER_NO		  ); 
																			  
			m_LstLabels.push_back            ( QApplication::translate("CustomerSelectionVC", "Pharmacy Name") );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_NAME	  );

			m_LstLabels.push_back            ( QApplication::translate("CustomerSelectionVC", "Branch No") );
			m_LstOriginalAttributes.push_back( properties::BRANCH_NO		  );
		}

		basar::Int32 CustomerSelectionVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( TenderOverviewVC, initMatcher )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_CustomerMatcher = basar::gui::tie::Manager::getInstance().createMatcher(ui.twCustomer, propTab);
            m_CustomerMatcher.transformProperties(ui.twCustomer->horizontalHeaderLabels(), m_LstOriginalAttributes);

			basar::Int32 count = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();

			while (!yit.isEnd())
			{
				++count;
				++yit;
			}

			return count;
        }

		void CustomerSelectionVC::on_DialogClose_Requested()
		{
			METHODNAME_DEF( CustomerSelectionVC, on_DialogClose_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			this->reject();
		}

		void CustomerSelectionVC::on_TakeOver_Requested()
		{
			METHODNAME_DEF( CustomerSelectionVC, on_TakeOver_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_PickedCustomer = m_CustomerMatcher.getCurrentRight();
			this->reject();
		}

		void CustomerSelectionVC::displayEmptyCustomerList()
		{
			showInfoMessage( tr("No customer found in customer list!").toLocal8Bit().constData() );
			
			ui.btnTakeOver->setDisabled(true);
		}

		void CustomerSelectionVC::reject()
		{
			clear();
			QDialog::reject();
		}

    } 
}
