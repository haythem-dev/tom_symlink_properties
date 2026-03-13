//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderoverviewvc.h"

#include <libtender/domainmodule/constants.h>

//#include "itendercontractinfo.h" //srk
#include "itendersearchcriteria.h"
#include "businesstypetransformer.h"
#include "businessstatetransformer.h"
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
        TenderOverviewVC::TenderOverviewVC( QWidget* parent )
        : BaseVC( parent ),
          m_EventsWiring  ( "TenderOverviewVC::EventsWiring"   )
        {
            //METHODNAME_DEF( TenderSearcherVC, TenderSearcherVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            ui.setupUi(this);
            wireEvents();
			
            //initTenderHeaderLabels(region);
            //setupTenderTable();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        TenderOverviewVC::~TenderOverviewVC()
        {
            //METHODNAME_DEF( TenderOverviewVC, ~TenderOverviewVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            //TenderOverviewVC::close();
        }

        void TenderOverviewVC::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }

            connect( ui.btnClose, SIGNAL( clicked()					   ),
            this,                 SLOT  ( onDialogClose_Requested()	  ));
																	  
            connect( ui.btnEdit,  SIGNAL( clicked()					   ),
            this,                 SLOT  ( onEditTender_Requested()	  ));
																	  
            connect( ui.btnNew,   SIGNAL( clicked()					   ),
            this,                 SLOT  ( onNewTender_Requested()	  ));
																	  
            connect( ui.btnReset, SIGNAL( clicked()					   ),
            this,                 SLOT  ( onReset_Requested()		  ));
																	  
            connect( ui.btnDelete,SIGNAL( clicked()					   ),
            this,                 SLOT  ( onDeleteTender_Requested()  ));
																	  
			connect( ui.pbClone,  SIGNAL( clicked()					   ),
			this,				  SLOT  ( onCloneTender_Requested()	  ));

            connect( ui.twTender, SIGNAL( clicked(const QModelIndex &) ),
            this,                 SLOT  ( onTenderSelected()		  ));

            connect( ui.twTender, SIGNAL( cellDoubleClicked(int, int)  ),
            this,                 SLOT  ( onEditTender_Requested()    ));

            m_EventsWiring.set();
        }

        //srk
        //void TenderOverviewVC::matchToPharmacyGroupSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroupSearchYit ) const
        //{
        //    METHODNAME_DEF( TenderOverviewVC, matchToPharmacyGroupSearch )
        //    BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

        //    m_uiContractInfo->matchToPharmacyGroupSearch( pharmacyGroupSearchYit );
        //}
        //srk
        //void TenderOverviewVC::matchFromPharmacyGroup( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroup )
        //{
        //    METHODNAME_DEF( TenderOverviewVC, matchFromPharmacyGroup )
        //    BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

        //    m_uiContractInfo->matchFromPharmacyGroup( pharmacyGroup );
        //}
        //srk
        //void TenderOverviewVC::checkContractInfo()
        //{
        //    if( m_uiContractInfo->isFilled() )
        //    {
        //        //ui.pbNewItem->setEnabled( true );
        //    }
        //    else
        //    {
        //        //ui.pbNewItem->setDisabled( true );
        //    }
        //}

        void TenderOverviewVC::matchFromTenderCollection( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( TenderOverviewVC, matchFromTenderCollection )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            ui.twTender->setDisabled(true);
            ui.btnEdit->setEnabled(false);
            ui.btnDelete->setEnabled(false);
			ui.pbClone->setEnabled(false);

			int indexOfSelectedTender = 0;
			if( !m_TenderCollectionMatcher.isNull() )
			{
				indexOfSelectedTender = m_TenderCollectionMatcher.getCurrentLeft().getAbsolutePosition();
			}

			int rowCount = initMatcher( propTab );

            libutil::gui::SignalBlocker signalblocker( ui.twTender );
            ui.twTender->setRowCount( rowCount );
            const basar::Int32 matchedHits = m_TenderCollectionMatcher.RightToLeft( 0, ui.twTender->rowCount() );

			ui.twTender->setRowCount( matchedHits );
			ui.twTender->resizeColumnsToContents();
            signalblocker.unblock();

			if( 0 != matchedHits )
			{
				ui.twTender->setEnabled(true);
				//selectRowByAbsoluteIndex( ui.twTender, m_TenderCollectionMatcher, indexOfSelectedTender );

				ui.twTender->selectRow(indexOfSelectedTender);
				ui.twTender->setFocus();

				onTenderSelected();
			}
        }

        void TenderOverviewVC::matchToSearchCriteria ( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
        {
            METHODNAME_DEF( TenderOverviewVC, matchToSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_uiSearchCriteria->matchToSearchCriteria( yitSearch );
        }

        void TenderOverviewVC::injectSearchCriteria (tender::viewConn::ITenderSearchCriteriaPtr searchCriteria)
        {
            METHODNAME_DEF( TenderOverviewVC, injectSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_uiSearchCriteria = searchCriteria.get();
			ui.verticalLayout->insertWidget( 0, m_uiSearchCriteria);
        }

		void TenderOverviewVC::setArticleCode( basar::db::aspect::AccessorPropertyTable_YIterator  yit )
		{
            METHODNAME_DEF( TenderOverviewVC, setArticleCode )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !yit.isNull() )
			{
			    m_uiSearchCriteria->setArticleCode( yit );
			}
		}

        void TenderOverviewVC::setPharmacyGroupId( basar::db::aspect::AccessorPropertyTable_YIterator yit )
        {
            METHODNAME_DEF( TenderOverviewVC, setPharmacyGroupId )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            if( !yit.isNull() )
            {
                m_uiSearchCriteria->setPharmacyGroupId( yit );
            }
        }

		void TenderOverviewVC::setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( TenderOverviewVC, setCustomerNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !yit.isNull() )
			{
				m_uiSearchCriteria->setCustomerNo( yit );
			}
		}

		void TenderOverviewVC::setPartnerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( TenderOverviewVC, setPartnerNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !yit.isNull() )
			{
			    m_uiSearchCriteria->setPartnerNo( yit );
			}
		}

        void TenderOverviewVC::setSupplierNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
        {
            METHODNAME_DEF( TenderOverviewVC, setSupplierNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
            
            if( !yit.isNull() )
            {
                m_uiSearchCriteria->setSupplierNo( yit );
            }
        }

		void TenderOverviewVC::setPharmacyChainNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( TenderOverviewVC, setPharmacyChainNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !yit.isNull() )
			{
				m_uiSearchCriteria->setPharmacyChainNo( yit );
			}
		}

		void TenderOverviewVC::setArea( basar::Int16 area )
		{
			m_area = area;
		}

        basar::gui::tie::WidgetReturnEnum TenderOverviewVC::show()
        {
            METHODNAME_DEF( TenderOverviewVC, show )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

            return basar::gui::tie::getWidgetReturnType( exec() );
        }

		void TenderOverviewVC::initialize()
		{
			switch( m_area )
			{
				case constants::REGION_BG: initTenderHeaderLabelsBG();
					 break;
				case constants::REGION_RS: initTenderHeaderLabelsRS();
					 break;
				case constants::REGION_FR: initTenderHeaderLabelsFR();
					 break;
                case constants::REGION_CH: initTenderHeaderLabelsCH();
                     break;
			}

			setupTenderTable();
		}

        void TenderOverviewVC::setupTenderTable()
        {
           METHODNAME_DEF( TenderOverviewVC, setupTenderTable )
           BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

           ui.twTender->setABCVerticalHeaderPolicy();
           ui.twTender->setColumnCount              ( m_LstLabels.size()                 );
           ui.twTender->setHorizontalHeaderLabels   ( m_LstLabels                        );
           //ui.twTender->setResizeRowsToContents     ( true                               );
           ui.twTender->setResizeColumnsToContents  ( true                               );
		   ui.twTender->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn             );
           ui.twTender->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
           ui.twTender->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
           ui.twTender->setSelectionMode            ( QAbstractItemView::SingleSelection );

           ui.twTender->setSortingEnabled(false);
           ui.twTender->setDisabled(true);
        }

		void TenderOverviewVC::initTenderHeaderLabelsFR( )
        {
            METHODNAME_DEF( TenderOverviewVC, initTenderHeaderLabelsFR )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_LstLabels.push_back            ( tr("Tender ID")					 );
            m_LstOriginalAttributes.push_back( properties::TENDER_NO			 );
																	   
            m_LstLabels.push_back            ( tr("Customer No.")				 );
            m_LstOriginalAttributes.push_back( properties::CUSTOMER_NO			 );

			m_LstLabels.push_back			 ( tr("Customer name")		     	 );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_NAME		 );
																				 
            m_LstLabels.push_back            ( tr("Contract No.")				 );
            m_LstOriginalAttributes.push_back( properties::CONTRACT_NO			 );
																				 
            m_LstLabels.push_back            ( tr("Business type")				 );
            m_LstOriginalAttributes.push_back( properties::TENDER_TYPE			 );

            m_LstLabels.push_back            ( tr("Begin date")					 );
            m_LstOriginalAttributes.push_back( properties::DATE_FROM			 );
																	  
            m_LstLabels.push_back            ( tr("End date")					 );
            m_LstOriginalAttributes.push_back( properties::DATE_TO				 );
		}

		void TenderOverviewVC::initTenderHeaderLabelsRS()
		{
			METHODNAME_DEF( TenderOverviewVC, initTenderHeaderLabelsRS )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_LstLabels.push_back            ( tr("Tender ID")					 );
            m_LstOriginalAttributes.push_back( properties::TENDER_NO			 );
																	   
            m_LstLabels.push_back            ( tr("Customer No.")				 );
            m_LstOriginalAttributes.push_back( properties::CUSTOMER_NO			 );

			m_LstLabels.push_back			 ( tr("Customer name")		     	 );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_NAME		 );

			m_LstLabels.push_back            ( tr("Pharmacy chain No.")			 );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_CHAIN_NO	 );
																				 
            m_LstLabels.push_back            ( tr("Contract No.")				 );
            m_LstOriginalAttributes.push_back( properties::CONTRACT_NO			 );
																				 
            m_LstLabels.push_back            ( tr("Type")						 );
            m_LstOriginalAttributes.push_back( properties::TENDER_TYPE			 );

            m_LstLabels.push_back            ( tr("Begin date")					 );
            m_LstOriginalAttributes.push_back( properties::DATE_FROM			 );
																	  
            m_LstLabels.push_back            ( tr("End date")					 );
            m_LstOriginalAttributes.push_back( properties::DATE_TO				 );	
		}

		void TenderOverviewVC::initTenderHeaderLabelsBG()
		{
			METHODNAME_DEF( TenderOverviewVC, initTenderHeaderLabelsBG )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_LstLabels.push_back            ( tr("Tender ID")					 );
            m_LstOriginalAttributes.push_back( properties::TENDER_NO			 );
																	   
            m_LstLabels.push_back            ( tr("Customer No.")				 );
            m_LstOriginalAttributes.push_back( properties::CUSTOMER_NO			 );

			m_LstLabels.push_back			 ( tr("Customer name")		     	 );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_NAME		 );

            m_LstLabels.push_back            ( tr("Contract No.")				 );
            m_LstOriginalAttributes.push_back( properties::CONTRACT_NO			 );
																				 
            m_LstLabels.push_back            ( tr("Type")						 );
            m_LstOriginalAttributes.push_back( properties::TENDER_TYPE			 );

            m_LstLabels.push_back            ( tr("Begin date")					 );
            m_LstOriginalAttributes.push_back( properties::DATE_FROM			 );
																	  
            m_LstLabels.push_back            ( tr("End date")					 );
            m_LstOriginalAttributes.push_back( properties::DATE_TO				 );
		}
        void TenderOverviewVC::initTenderHeaderLabelsCH()
        {
            METHODNAME_DEF( TenderOverviewVC, initTenderHeaderLabelsCH )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_LstLabels.push_back            ( tr("Tender ID")                   );
            m_LstOriginalAttributes.push_back( properties::TENDER_NO             );

            m_LstLabels.push_back            ( tr("Order Quantity")                    );
            m_LstOriginalAttributes.push_back( properties::CONTRACT_QTY               );

            m_LstLabels.push_back            ( tr("Customer No.")                );
            m_LstOriginalAttributes.push_back( properties::CUSTOMER_NO           );

            m_LstLabels.push_back            ( tr("Customer name")               );
            m_LstOriginalAttributes.push_back( properties::PHARMACY_NAME         );

            m_LstLabels.push_back            ( tr("City")               );
            m_LstOriginalAttributes.push_back( properties::PHARMACY_LOCATION         );

            m_LstLabels.push_back            ( tr("Contract No.")                );
            m_LstOriginalAttributes.push_back( properties::CONTRACT_NO           );

            m_LstLabels.push_back            ( tr("Type")                        );
            m_LstOriginalAttributes.push_back( properties::TENDER_TYPE           );

            m_LstLabels.push_back            ( tr("Begin date")                  );
            m_LstOriginalAttributes.push_back( properties::DATE_FROM             );

            m_LstLabels.push_back            ( tr("End date")                    );
            m_LstOriginalAttributes.push_back( properties::DATE_TO               );

            m_LstLabels.push_back            ( tr("Article Code")                    );
            m_LstOriginalAttributes.push_back( properties::ARTICLE_CODE               );

            m_LstLabels.push_back            ( tr("Article Name")                    );
            m_LstOriginalAttributes.push_back( properties::ARTICLE_NAME               );

            m_LstLabels.push_back            ( tr("Discount")                    );
            m_LstOriginalAttributes.push_back( properties::ADD_DISCOUNT_PCT               );


            //m_LstLabels.push_back            ( tr("Current Qty")                    );
            //m_LstOriginalAttributes.push_back( properties::CURRENT_QTY               );

            //m_LstLabels.push_back            ( tr("Supplier No")                    );
            //m_LstOriginalAttributes.push_back( properties::SUPPLIER_NO               );

            //m_LstLabels.push_back            ( tr("Supplier Name")                    );
            //m_LstOriginalAttributes.push_back( properties::SUPPLIER_NAME               );

            //m_LstLabels.push_back            ( tr("Creation Date")                    );
            //m_LstOriginalAttributes.push_back( properties::CREATION_DATE               );

            m_LstLabels.push_back            ( tr("Last Change Date")                    );
            m_LstOriginalAttributes.push_back( properties::CHANGE_DATE               );

            m_LstLabels.push_back            ( tr("State")                    );
            m_LstOriginalAttributes.push_back( properties::TENDER_STATE               );

            m_LstLabels.push_back            ( tr("Pharmacy Group ID")                    );
            m_LstOriginalAttributes.push_back( properties::PHARMACY_GROUPID               );

            m_LstLabels.push_back            ( tr("Own Quota Qty")                    );
            m_LstOriginalAttributes.push_back( properties::OWNQUOTA_QTY               );

            m_LstLabels.push_back            ( tr("Own Qty Flag")                    );
            m_LstOriginalAttributes.push_back( properties::OWNQUOTA_FLAG               );

            m_LstLabels.push_back            ( tr("Recall Flag")                    );
            m_LstOriginalAttributes.push_back( properties::RECALL_FLAG               );

        }

        basar::Int32 TenderOverviewVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( TenderOverviewVC, initMatcher )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_TenderCollectionMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twTender, propTab );
            m_TenderCollectionMatcher.transformProperties( ui.twTender->horizontalHeaderLabels(), m_LstOriginalAttributes );

            switch( m_area )
            {
            case constants::REGION_BG:
                {
                    BusinessTypeTransformer businessTypeTransformer( tr("Type").toLocal8Bit().data(), properties::TENDER_TYPE.getName(), m_uiSearchCriteria->getTypeMapping() );
                    m_TenderCollectionMatcher.push_back( businessTypeTransformer );
                }
                break;
            case constants::REGION_RS:
                {
                    BusinessTypeTransformer businessTypeTransformer( tr("Type").toLocal8Bit().data(), properties::TENDER_TYPE.getName(), m_uiSearchCriteria->getTypeMapping() );
                    m_TenderCollectionMatcher.push_back( businessTypeTransformer );
                }
                break;
            case constants::REGION_FR:
                {   
                    BusinessTypeTransformer businessTypeTransformer( tr("Business type").toLocal8Bit().data(), properties::TENDER_TYPE.getName(), m_uiSearchCriteria->getTypeMapping() );
                    m_TenderCollectionMatcher.push_back( businessTypeTransformer );
                }
                break;
            case constants::REGION_CH:
                {
                    BusinessTypeTransformer businessTypeTransformer( tr("Type").toLocal8Bit().data(), properties::TENDER_TYPE.getName(), m_uiSearchCriteria->getTypeMapping() );
                    BusinessStateTransformer businessstateTransformer( tr("State").toLocal8Bit().data(), properties::TENDER_STATE.getName(), m_uiSearchCriteria->getStatusMapping() );
                    m_TenderCollectionMatcher.push_back( businessTypeTransformer );
                    m_TenderCollectionMatcher.push_back( businessstateTransformer );
                }
                break;
            
            }
            
            basar::Int32 count = 0;
            basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
            while (!yit.isEnd())
            {
                ++count;
                ++yit;
            }
            return count;
        }
        //srk
        //void TenderOverviewVC::resetPharmacyGroup()
        //{
        //    m_uiContractInfo->resetPharmacyGroup();
        //}

		void TenderOverviewVC::reset()
		{
			ui.twTender->clearContents();
            ui.twTender->setRowCount(0);
            ui.twTender->setDisabled(true);
            ui.btnEdit->setEnabled(false);
            ui.btnDelete->setEnabled(false);
			ui.pbClone->setEnabled(false);

			if( !m_TenderCollectionMatcher.isNull() )
			{
				m_TenderCollectionMatcher.reset();
			}

            m_uiSearchCriteria->prepareForNewSearch();
            m_uiSearchCriteria->setFocus();
		}

        void TenderOverviewVC::onDialogClose_Requested()
        {
            METHODNAME_DEF( TenderOverviewVC, onDialogClose_Requested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( basar::gui::tie::ButtonYes ==
				basar::gui::tie::questionMsgBox(this, tr("Do you want to close the dialog?\n").toLocal8Bit(), QString("").toStdString().c_str() ) )
			{
				reset();
				QDialog::reject();
			}	
        }

        void TenderOverviewVC::onEditTender_Requested()
        {
            METHODNAME_DEF( TenderOverviewVC, onEditTender_Requested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			libutil::gui::WaitCursor bc( this );
            basar::appl::SystemEventManager::getInstance().fire( "EditTenderRequested" );
        }

        void TenderOverviewVC::onNewTender_Requested()
        {
            METHODNAME_DEF( TenderOverviewVC, onEditTender_Requested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			libutil::gui::WaitCursor bc( this );
            basar::appl::SystemEventManager::getInstance().fire( "NewTenderRequested" );
        }

        void TenderOverviewVC::onReset_Requested()
        {
            reset();
        }

        void TenderOverviewVC::onDeleteTender_Requested()
        {
			if( constants::REGION_CH == m_area )
			{
				if( basar::gui::tie::ButtonNo == showQuestionMessage( tr("Do you want to close this tender?").toLocal8Bit().constData() ) )
				{
					return;
				}
			}
			else
			{
				if( basar::gui::tie::ButtonNo == showQuestionMessage( tr("Do you want to delete this tender?").toLocal8Bit().constData() ) )
				{
					return;
				}
			}

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "DeleteTenderRequested" );
        }

		void TenderOverviewVC::onCloneTender_Requested()
		{
			if( getCurrentTender().isEnd() )
			{
				return;
			}

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "CloneTenderRequested" );
		}

        void TenderOverviewVC::onTenderSelected()
        {
            METHODNAME_DEF( TenderOverviewVC, onTenderSelected )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            ui.btnEdit->setEnabled(true);
            ui.btnDelete->setEnabled(true);
			ui.pbClone->setEnabled(true);
        }

        basar::db::aspect::AccessorPropertyTable_YIterator TenderOverviewVC::getCurrentTender()
        {
            METHODNAME_DEF( TenderOverviewVC, getCurrentTender )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            return m_TenderCollectionMatcher.getCurrentRight();
        }

		void TenderOverviewVC::selectTender( basar::db::aspect::AccessorPropertyTable_YIterator tenderYit )
		{
			METHODNAME_DEF( TenderOverviewVC, selectTender )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !tenderYit.isContainedAndSet( properties::TENDER_NO ) )
			{
				return;
			}

			basar::Int32 tenderNo = tenderYit.getInt32( properties::TENDER_NO );

			QList< QTableWidgetItem* > items = ui.twTender->findItems( QString::number( tenderNo ), Qt::MatchExactly );
			
			foreach( QTableWidgetItem* item, items )
			{
				int row = item->row();
				ui.twTender->selectRow( row );
			}
		}

		void TenderOverviewVC::selectRowByAbsoluteIndex(BasarTableWidget* tabWidget, basar::gui::tie::MatcherRef matcher, int absoluteIndex)
		{
			if (absoluteIndex < tabWidget->rowCount()) // alles easy
			{
				tabWidget->selectRow(absoluteIndex);
				tabWidget->setFocus();
			}
			else // nix mehr easy
			{
				basar::Int32 pageCount = (absoluteIndex+1) / tabWidget->rowCount(); // zero based!
				basar::Int32 relativeIndex = (absoluteIndex+1) % tabWidget->rowCount() - 1; // zero based!
				  
				if (relativeIndex < 0)
				{
				  relativeIndex = tabWidget->rowCount() - 1;
				  pageCount--;
				}
				  
				tabWidget->clearContext();
				matcher.RightToLeft(0, (pageCount+1) * tabWidget->rowCount()); // scroll to the correct page
				tabWidget->resizeColumnsToContents();
		
				if (matcher.getRight().getNumberOfFetchedRows() < static_cast<basar::UInt32>((pageCount+1) * tabWidget->rowCount()))
				  relativeIndex += static_cast<basar::Int32>((pageCount+1) * tabWidget->rowCount() - matcher.getRight().getNumberOfFetchedRows());

				tabWidget->selectRow(relativeIndex);
				tabWidget->setFocus();
			}
		}

        void TenderOverviewVC::displayTenderStateWarning(basar::VarString warning)
        {
            showInfoMessage( tr( warning.c_str() ).toLocal8Bit().constData() );
        }

        void TenderOverviewVC::displayNoTenderFound()
        {
			showInfoMessage( tr("No tender found.").toLocal8Bit().constData() );

            m_uiSearchCriteria->setFocus();
        }

		void TenderOverviewVC::displayDeleteNotPossible(basar::VarString message)
		{
			showWarningMessage( tr("Tender can not be deleted.\nAt least one delivery has a delivery date which is in the past.").toLocal8Bit().constData() );
		}

		void TenderOverviewVC::displayEndDateSetToLastDateOfTheYear()
		{
			showWarningMessage( tr("Because the EndDate is invalid, it is set to the last day of this year!").toLocal8Bit().constData() );
		}

		basar::gui::tie::MessageBoxButtonEnum TenderOverviewVC::displayCloneDeliveriesQuestion()
		{
			return showQuestionMessage( tr("Do you want to clone the deliveries?").toLocal8Bit().constData(), basar::gui::tie::ButtonYes|basar::gui::tie::ButtonNo|basar::gui::tie::ButtonCancel  );
		}

		basar::gui::tie::MessageBoxButtonEnum TenderOverviewVC::displayOrderProposalAlreadyCreated()
		{
			return showQuestionMessage( tr("Attention!\nFor at least one delivery a purchase order proposal has already been created.\n\nDelete anyway?").toLocal8Bit().constData(),
										basar::gui::tie::ButtonYes|basar::gui::tie::ButtonNo );
		}

        void TenderOverviewVC::reject()
        {
			reset();
            QDialog::reject();
        }

    } // end namespace viewConn
} // end namespace tender
