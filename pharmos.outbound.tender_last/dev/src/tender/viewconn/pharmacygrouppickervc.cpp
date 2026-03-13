#include "pharmacygrouppickervc.h"

#include "loggerpool/tenderloggerpool.h"
#include "libtender/domainmodule/libtender_properties_definitions.h"


namespace tender
{
    namespace viewConn
    {
        PharmacyGroupPickerVC::PharmacyGroupPickerVC( QWidget* parent )
        :BaseVC( parent ),
         m_EventsWiring( "PharmacyGroupPickerVC : EventsWiring" )
        {
            ui.setupUi( this );
            wireEvents();
            
            initPharmacyGroupHeaderLabels();
            setupPharmacyGroupTable();
            setupSearchCriteria();
            
            ui.btnTakeOver->setDisabled( true );
        }

        PharmacyGroupPickerVC::~PharmacyGroupPickerVC()
        {
        
        }

        void PharmacyGroupPickerVC::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }
             
            connect( ui.btnCancel,          SIGNAL( clicked()                      ),
            this,                           SLOT  ( on_DialogClose_Requested()     ));
            
            connect( ui.btnSearch,          SIGNAL( clicked()                      ),
            this,                           SLOT  ( on_Search_Requested()          ));
            
            connect( ui.twPharmacyGroup,    SIGNAL( activated(const QModelIndex &) ),
            this,                           SLOT  ( on_TakeOver_Requested()        ));
            
            connect( ui.btnTakeOver,        SIGNAL( clicked()                      ),
            this,                           SLOT  ( on_TakeOver_Requested()        ));
            
            m_EventsWiring.set();
        }

        void PharmacyGroupPickerVC::matchFromPharmacyGroupCollection( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, matchFromPharmacyGroupCollection )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            ui.twPharmacyGroup->setDisabled( true );
            basar::Int32 rowCount = initMatcher(propTab);
            libutil::gui::SignalBlocker signalblocker( ui.twPharmacyGroup );
            ui.twPharmacyGroup->setRowCount( rowCount );
            m_PharmacyGroupMatcher.RightToLeft( 0, ui.twPharmacyGroup->rowCount() );
            
            signalblocker.unblock();
            
            ui.twPharmacyGroup->resizeColumnsToContents();
            
            if( rowCount )
            {
                ui.twPharmacyGroup->setEnabled( true );
            
                ui.twPharmacyGroup->selectRow(0);
                ui.twPharmacyGroup->setFocus();
            
                ui.btnTakeOver->setEnabled( true );
            }
        }

        void PharmacyGroupPickerVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, matchToSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            if( !ui.lePharmacyGroupId->text().isEmpty() )
            {
                yitSearch.setString( properties::PHARMACY_GROUPID, ui.lePharmacyGroupId->text().toLocal8Bit().data() );
            }

            if( !ui.lePharamacyGroupName->text().isEmpty() )
            {
                yitSearch.setString( properties::PHARMACY_GROUP_NAME, ui.lePharamacyGroupName->text().toLocal8Bit().data() );
            }
        }

        void PharmacyGroupPickerVC::focusInEvent( QFocusEvent* event )
        {
            event;
            ui.lePharmacyGroupId->setFocus();
        }
        
        void PharmacyGroupPickerVC::setPharmacyGroupId( basar::db::aspect::AccessorPropertyTable_YIterator yit )
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, setPharmacyGroupNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            if( yit.isContainedAndSet(properties::PHARMACY_GROUPID) )
            {
                ui.lePharmacyGroupId->setText(yit.getString(properties::PHARMACY_GROUPID ).c_str() );
            }
            else 
            {
                ui.lePharmacyGroupId->setText( "" );
            }
        }
        
        basar::db::aspect::AccessorPropertyTable_YIterator PharmacyGroupPickerVC::getPickedPharmacyGroup()
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, getPickedPharmacyGroup )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
        
            return m_PickedPharmacyGroup;
        }
        
        basar::gui::tie::WidgetReturnEnum PharmacyGroupPickerVC::show()
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, show )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            m_PickedPharmacyGroup.clear();

            return basar::gui::tie::getWidgetReturnType( exec() );
        }

        void PharmacyGroupPickerVC::clear()
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, clear )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            ui.twPharmacyGroup->setRowCount( 0 );
            ui.twPharmacyGroup->setDisabled( true );
            
            ui.lePharmacyGroupId->setText( "" );
            ui.lePharamacyGroupName->setText( "" );
            ui.btnTakeOver->setDisabled( true );
        }

        void PharmacyGroupPickerVC::setupPharmacyGroupTable()
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, setupPharmacyGroupTable )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
        
            ui.twPharmacyGroup->setABCVerticalHeaderPolicy  ();
            ui.twPharmacyGroup->setColumnCount              ( m_LstLabels.size()                 );
            ui.twPharmacyGroup->setHorizontalHeaderLabels   ( m_LstLabels                        );
            ui.twPharmacyGroup->setResizeRowsToContents     ( true                               );
            ui.twPharmacyGroup->setResizeColumnsToContents  ( true                               );
            ui.twPharmacyGroup->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn              );
            ui.twPharmacyGroup->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twPharmacyGroup->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twPharmacyGroup->setSelectionMode            ( QAbstractItemView::SingleSelection );
            ui.twPharmacyGroup->setSortingEnabled           ( false );
            ui.twPharmacyGroup->setDisabled                 ( true  );
        }

        void PharmacyGroupPickerVC::initPharmacyGroupHeaderLabels()
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, initPharmacyGroupHeaderLabels )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
			m_LstLabels.push_back            ( QApplication::translate("PharmacyGroupPickerVC", "Pharmacy Group ID") );
            m_LstOriginalAttributes.push_back( properties::PHARMACY_GROUPID     );

            m_LstLabels.push_back            ( QApplication::translate("PharmacyGroupPickerVC", "Customer No.") );
            m_LstOriginalAttributes.push_back( properties::CUSTOMER_NO          );

            m_LstLabels.push_back            ( QApplication::translate("PharmacyGroupPickerVC", "Pharmacy Group Name") );
            m_LstOriginalAttributes.push_back( properties::PHARMACY_GROUP_NAME  );
        }

        basar::Int32 PharmacyGroupPickerVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, initMatcher )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_PharmacyGroupMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twPharmacyGroup, propTab );
            m_PharmacyGroupMatcher.transformProperties( ui.twPharmacyGroup->horizontalHeaderLabels(), m_LstOriginalAttributes );

            basar::Int32 count = 0;
            basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
            while (!yit.isEnd())
            {
                ++count;
                ++yit;
            }
            return count;
        }

        void PharmacyGroupPickerVC::on_DialogClose_Requested()
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, on_DialogClose_Requested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            this->reject();
        }
        
        void PharmacyGroupPickerVC::on_Search_Requested()
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, on_Search_Requested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            if( ui.lePharmacyGroupId->text().isEmpty() && ui.lePharamacyGroupName->text().isEmpty() )
            {
                showWarningMessage( tr("No search criteria.").toLocal8Bit().constData() );
                ui.lePharmacyGroupId->setFocus();
                
                return;
            }
            
            if(ui.lePharamacyGroupName->text().isEmpty() && 3 < ui.lePharmacyGroupId->text().count() )
            {
                showInfoMessage( tr("Please enter only 3 characters into the pharmacy group id field.").toLocal8Bit().constData() );
                ui.lePharmacyGroupId->setFocus();
                ui.lePharmacyGroupId->selectAll();
                
                return;
            }
            
            libutil::gui::WaitCursor bc( this );
            basar::appl::SystemEventManager::getInstance().fire( "SearchPharmacyGroupRequested" );
        }
        
        void PharmacyGroupPickerVC::on_TakeOver_Requested()
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, on_TakeOver_Requested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            m_PickedPharmacyGroup = m_PharmacyGroupMatcher.getCurrentRight();
            this->reject();
        }
        
        void PharmacyGroupPickerVC::displayNoPharmacyGroupFound()
        {
            showInfoMessage( tr("No PharmacyGroup found.").toLocal8Bit().constData() );
            
            ui.btnTakeOver->setDisabled( true );
            
            if( !ui.lePharmacyGroupId->text().isEmpty() )
            {
                ui.lePharmacyGroupId->setFocus();
                ui.lePharmacyGroupId->selectAll();
                return;
            }

            if( !ui.lePharamacyGroupName->text().isEmpty() )
            {
                ui.lePharamacyGroupName->setFocus();
                ui.lePharamacyGroupName->selectAll();
            }
        }
        
        void PharmacyGroupPickerVC::setupSearchCriteria()
        {
            METHODNAME_DEF( PharmacyGroupPickerVC, setupSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            ui.lePharmacyGroupId->setAccessibleName( properties::PHARMACY_GROUPID.getName().c_str()    );
            
            QRegExp rxPharmacyGroupId( "^[0-9]{1,3}$" );
            QValidator *pharmacyGroupIdValidator = new QRegExpValidator( rxPharmacyGroupId, ui.lePharmacyGroupId );
            ui.lePharmacyGroupId->setValidator( pharmacyGroupIdValidator );

            ui.lePharamacyGroupName->setAccessibleName( properties::CUSTOMER_NO.getName().c_str()    );

            QRegExp rxPharamacyGroupName( "^([^\\W]+\\s)*" );
            QValidator *pharmaGrpNameValidator = new QRegExpValidator( rxPharamacyGroupName, ui.lePharamacyGroupName );
            ui.lePharamacyGroupName->setValidator( pharmaGrpNameValidator );
        }
        
        void PharmacyGroupPickerVC::reject()
        {
            clear();
            QDialog::reject();
        }

    } // end namespace viewConn
} // end namespace tender
