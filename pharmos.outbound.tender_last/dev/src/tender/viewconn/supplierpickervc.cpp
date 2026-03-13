//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <supplierpickervc.h>

#include "loggerpool/tenderloggerpool.h"

#include "libtender/domainmodule/libtender_properties_definitions.h"

//-------------------------------------------------------------------------------------------------//
// using declaration section
//-------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {
        /////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        SupplierPickerVC::SupplierPickerVC( QWidget* parent )
        :BaseVC( parent ),
         m_EventsWiring( "SupplierPickerVC : EventsWiring" )
        {
            ui.setupUi( this );
            wireEvents();
            
            initSupplierHeaderLabels();
            
            setupSupplierTable();
            setupSearchCriteria();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        SupplierPickerVC::~SupplierPickerVC()
        {

        }

        void SupplierPickerVC::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }

            connect( ui.btnCancel,   SIGNAL( clicked()                      ),
            this,                    SLOT  ( on_DialogClose_Requested()     ));

            connect( ui.btnSearch,   SIGNAL( clicked()                      ),
            this,                    SLOT  ( on_Search_Requested()          ));
            
            connect( ui.twSupplier,   SIGNAL( activated(const QModelIndex &) ),
            this,                    SLOT  ( on_TakeOver_Requested()        ));

            connect( ui.btnTakeOver, SIGNAL( clicked()                      ),
            this,                    SLOT  ( on_TakeOver_Requested()        ));

            m_EventsWiring.set();
        }

        void SupplierPickerVC::matchFromSupplierCollection( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( SupplierPickerVC, matchFromSupplierCollection )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

            ui.twSupplier->setDisabled( true );
            basar::Int32 rowCount = initMatcher(propTab);
            libutil::gui::SignalBlocker signalblocker( ui.twSupplier );
            ui.twSupplier->setRowCount( rowCount );
            m_SupplierMatcher.RightToLeft( 0, ui.twSupplier->rowCount() );
            signalblocker.unblock();

            ui.twSupplier->resizeColumnsToContents();

            if( rowCount )
            {
                ui.twSupplier->setEnabled( true );
                ui.twSupplier->selectRow(0);
                ui.twSupplier->setFocus();
                ui.btnTakeOver->setEnabled( true );
            }
        }

        void SupplierPickerVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
        {
            METHODNAME_DEF( SupplierPickerVC, matchToSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            if( !ui.leSupplierNo->text().isEmpty() )
            {
                yitSearch.setInt32( properties::SUPPLIER_NO, ui.leSupplierNo->text().toInt() );
            }

            if( !ui.leSupplierName->text().isEmpty() )
            {
                yitSearch.setString( properties::SUPPLIER_NAME, ui.leSupplierName->text().toStdString() );
            }
        }

        void SupplierPickerVC::focusInEvent( QFocusEvent* event )
        {
            event;
            ui.leSupplierNo->setFocus();
        }

        void SupplierPickerVC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearchCriteria )
        {
            METHODNAME_DEF( SupplierPickerVC, setSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            if( yitSearchCriteria.isContainedAndSet(properties::SUPPLIER_NO) )
            {
                ui.leSupplierNo->setText( QString::number(yitSearchCriteria.getInt32(properties::SUPPLIER_NO )) );
            }
            else
            {
                ui.leSupplierNo->setText( "" );
            }
        }

        basar::db::aspect::AccessorPropertyTable_YIterator SupplierPickerVC::getPickedSupplier()
        {
            METHODNAME_DEF( SupplierPickerVC, getPickedSupplier )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            return m_PickedSupplier;
        }

        basar::gui::tie::WidgetReturnEnum SupplierPickerVC::show()
        {
            METHODNAME_DEF( SupplierPickerVC, show )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_PickedSupplier.clear();

            return basar::gui::tie::getWidgetReturnType( exec() );
        }

        void SupplierPickerVC::clear()
        {
            METHODNAME_DEF( SupplierPickerVC, clear )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            ui.twSupplier->setRowCount( 0 );
            ui.twSupplier->setDisabled( true );
            
            ui.leSupplierNo->setText  ( "" );
            ui.leSupplierName->setText( "" );

            ui.btnTakeOver->setDisabled( true );
        }

        void SupplierPickerVC::setupSupplierTable()
        {
            METHODNAME_DEF( SupplierPickerVC, setupSupplierTable )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            ui.twSupplier->setABCVerticalHeaderPolicy  ();
            ui.twSupplier->setColumnCount              ( m_LstLabels.size()                 );
            ui.twSupplier->setHorizontalHeaderLabels   ( m_LstLabels                        );
            ui.twSupplier->setResizeRowsToContents     ( true                               );
            ui.twSupplier->setResizeColumnsToContents  ( true                               );
            ui.twSupplier->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn              );
            ui.twSupplier->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twSupplier->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twSupplier->setSelectionMode            ( QAbstractItemView::SingleSelection );
            ui.twSupplier->setSortingEnabled           ( false );
            ui.twSupplier->setDisabled                 ( true  );

        }

        void SupplierPickerVC::initSupplierHeaderLabels()
        {
            METHODNAME_DEF( SupplierPickerVC, initSupplierHeaderLabels )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_LstLabels.push_back            ( QApplication::translate("SupplierPickerVC", "Supplier No.") );
            m_LstOriginalAttributes.push_back( properties::SUPPLIER_NO );

            m_LstLabels.push_back            ( QApplication::translate("SupplierPickerVC", "Supplier Name") );
            m_LstOriginalAttributes.push_back( properties::SUPPLIER_NAME );

            m_LstLabels.push_back            ( QApplication::translate("SupplierPickerVC", "Street") );
            m_LstOriginalAttributes.push_back( properties::SUPPLIER_STREET );
            
            m_LstLabels.push_back            ( QApplication::translate("SupplierPickerVC", "Location") );
            m_LstOriginalAttributes.push_back( properties::SUPPLIER_LOCATION );
        }

        basar::Int32 SupplierPickerVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( SupplierPickerVC, initMatcher )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_SupplierMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twSupplier, propTab );
            m_SupplierMatcher.transformProperties( ui.twSupplier->horizontalHeaderLabels(), m_LstOriginalAttributes );

            basar::Int32 count = 0;
            basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
            while (!yit.isEnd())
            {
                ++count;
                ++yit;
            }
            return count;
        }

        void SupplierPickerVC::on_DialogClose_Requested()
        {
            METHODNAME_DEF( SupplierPickerVC, on_DialogClose_Requested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            this->reject();
        }

        void SupplierPickerVC::on_Search_Requested()
        {
            METHODNAME_DEF( SupplierPickerVC, on_Search_Requested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            if( ui.leSupplierNo->text().isEmpty() && ui.leSupplierName->text().isEmpty() )
            {
                showWarningMessage( QApplication::translate("SupplierPickerVC", "No search criteria.").toLocal8Bit().constData() );
                ui.leSupplierNo->setFocus();

                return;
            }

            if( ui.leSupplierNo->text().isEmpty() &&
                3 > ui.leSupplierName->text().count() )
            {
                showInfoMessage( QApplication::translate("SupplierPickerVC", "Please enter at least 3 characters into the supplier name field.").toLocal8Bit().constData() );
                ui.leSupplierName->setFocus();
                ui.leSupplierName->selectAll();

                return;
            }

            libutil::gui::WaitCursor bc( this );
            basar::appl::SystemEventManager::getInstance().fire("SearchSupplierRequested");
        }

        void SupplierPickerVC::on_TakeOver_Requested()
        {
            METHODNAME_DEF( SupplierPickerVC, on_TakeOver_Requested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_PickedSupplier = m_SupplierMatcher.getCurrentRight();
            this->reject();
        }

        void SupplierPickerVC::displayNoSupplierFound()
        {
            showInfoMessage( QApplication::translate("SupplierPickerVC", "No supplier found.").toLocal8Bit().constData() );

            ui.btnTakeOver->setDisabled( true );

            if( !ui.leSupplierNo->text().isEmpty() )
            {
                ui.leSupplierNo->setFocus();
                ui.leSupplierNo->selectAll();
                return;
            }

            if( !ui.leSupplierName->text().isEmpty() )
            {
                ui.leSupplierName->setFocus();
                ui.leSupplierName->selectAll();
            }
        }

        void SupplierPickerVC::setupSearchCriteria()
        {
            METHODNAME_DEF( SupplierPickerVC, setupSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            ui.leSupplierNo->setAccessibleName( properties::SUPPLIER_NO.getName().c_str() );

            QRegExp rxSupplierNo( "^[1-9][0-9]{1,7}$" );
            QValidator *supplierNoValidator = new QRegExpValidator( rxSupplierNo, ui.leSupplierNo );
            ui.leSupplierNo->setValidator( supplierNoValidator );
            
            ui.leSupplierName->setAccessibleName( properties::SUPPLIER_NAME.getName().c_str() );

            QRegExp rxSupplierName( "^([a-zA-Z0-9]+\\s)*$" );
            QValidator *supplierNameValidator = new QRegExpValidator( rxSupplierName, ui.leSupplierName );
            ui.leSupplierName->setValidator( supplierNameValidator );
        }

        void SupplierPickerVC::reject()
        {
            clear();
            QDialog::reject();
        }

    } // end namespace viewConn
} // end namespace tender
