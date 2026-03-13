#include "articlepickervc.h"
#include "libtender/domainmodule/tender/tendertypecollection.h"
#include "libtender/domainmodule/libtender_properties_definitions.h"
#include "loggerpool/tenderloggerpool.h"

#include <util.h>


namespace tender
{
    namespace viewConn
    {
    	ArticlePickerVC::ArticlePickerVC( QWidget* parent )
    	:BaseVC( parent ),
    	 m_EventsWiring( "ArticlePickerVC : EventsWiring" )
    	{
    		ui.setupUi( this );
    		wireEvents();

            initArticleHeaderLabels();

    		setupArticleTable();
			setupSearchCriteria();

			ui.btnTakeOver->setDisabled( true );
			ui.cbBusinessType->setVisible( false );
			ui.lblBusinessType->setVisible( false );
		}

		ArticlePickerVC::~ArticlePickerVC()
		{
		}

		void ArticlePickerVC::injectTenderTypeCollection( libtender::domMod::tender::TenderTypeCollectionPtr tenderTypeCollection )
		{
			ui.cbBusinessType->setVisible( true );
			ui.lblBusinessType->setVisible( true );

			m_TenderTypeCollection = tenderTypeCollection;

			using namespace libtender::domMod::tender;
			TenderTypeCollection::const_iterator iter = m_TenderTypeCollection->begin();
			for( ; iter != m_TenderTypeCollection->end(); ++iter )
			{
			    ui.cbBusinessType->addItem( QString::fromLocal8Bit( iter->second.c_str() ), QVariant( iter->first ) );
			}
		}

		void ArticlePickerVC::wireEvents()
		{
			if( m_EventsWiring.isSet() )
			{
				return;
			}

			connect(ui.btnCancel,   SIGNAL( clicked()                      ),
			this,                   SLOT  ( on_DialogClose_Requested()     ));

			connect( ui.twArticle,  SIGNAL( activated(const QModelIndex &) ),
            this,                   SLOT  ( on_TakeOver_Requested()        ));

			connect(ui.btnSearch,   SIGNAL( clicked()                      ),
			this,                   SLOT  ( on_Search_Requested()          ));

			connect(ui.btnTakeOver, SIGNAL( clicked()                      ),
			this,                   SLOT  ( on_TakeOver_Requested()        ));

			m_EventsWiring.set();
		}

		void ArticlePickerVC::matchFromArticleCollection( basar::db::aspect::AccessorPropertyTableRef propTab )
		{
			METHODNAME_DEF( ArticlePickerVC, matchFromArticleCollection )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twArticle->setDisabled( true );

			basar::Int32 rowCount = initMatcher(propTab);

			libutil::gui::SignalBlocker signalblocker( ui.twArticle );
            ui.twArticle->setRowCount( rowCount );
			m_ArticleMatcher.RightToLeft( 0, ui.twArticle->rowCount() );
			signalblocker.unblock();

			ui.twArticle->resizeColumnsToContents();

			if(rowCount)
            {
                ui.twArticle->setEnabled( true );

                ui.twArticle->selectRow(0);
                ui.twArticle->setFocus();

				ui.btnTakeOver->setEnabled( true );
            }
		}

		void ArticlePickerVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
		{
			METHODNAME_DEF( ArticlePickerVC, matchToSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !ui.leArticleNo->text().isEmpty() )
			{
				yitSearch.setString( properties::ARTICLE_CODE, ui.leArticleNo->text().toLocal8Bit().constData() );
			}

			if( !ui.leArticleName->text().isEmpty() )
			{
				yitSearch.setString( properties::ARTICLE_NAME, ui.leArticleName->text().toLocal8Bit().constData() );
			}

			if( ui.cbBusinessType->currentIndex() < 4 )
			{
				yitSearch.setInt32( properties::TENDER_TYPE, ui.cbBusinessType->currentData().toInt() );
			}
		}

		basar::db::aspect::AccessorPropertyTable_YIterator ArticlePickerVC::getCurrentArticle()
		{
			METHODNAME_DEF( ArticlePickerVC, getCurrentArticle )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			return m_PickedArticle;
		}

		basar::gui::tie::WidgetReturnEnum ArticlePickerVC::show()
        {
            METHODNAME_DEF( ArticlePickerVC, show )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			m_PickedArticle.clear();

			if( !ui.leArticleNo->text().isEmpty() )
			{
				ui.leArticleNo->setFocus();
			}
			else
			{
				ui.leArticleName->setFocus();
			}
			
            return basar::gui::tie::getWidgetReturnType( exec() );
        }

		void ArticlePickerVC::clear()
		{
			METHODNAME_DEF( ArticlePickerVC, clear )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twArticle->setRowCount( 0 );
			ui.twArticle->setDisabled( true );

			ui.leArticleName->setText( "" );
			ui.leArticleNo->setText  ( "" );

			ui.btnTakeOver->setDisabled( true );
		}

		void ArticlePickerVC::setupArticleTable()
		{
			METHODNAME_DEF( ArticlePickerVC, setupArticleTable )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

		    ui.twArticle->setABCVerticalHeaderPolicy  ();
            ui.twArticle->setColumnCount              ( m_LstLabels.size()                 );
            ui.twArticle->setHorizontalHeaderLabels   ( m_LstLabels                        );
            ui.twArticle->setResizeRowsToContents     ( true                               );
            ui.twArticle->setResizeColumnsToContents  ( true                               );
            ui.twArticle->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn              );
            ui.twArticle->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twArticle->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twArticle->setSelectionMode            ( QAbstractItemView::SingleSelection );
			ui.twArticle->setSortingEnabled           ( false );
			ui.twArticle->setDisabled                 ( true  );
		}

		void ArticlePickerVC::initArticleHeaderLabels()
		{
			METHODNAME_DEF( ArticlePickerVC, initArticleHeaderLabels )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			m_LstLabels.push_back            ( tr("Article Code")				);
			m_LstOriginalAttributes.push_back( properties::ARTICLE_CODE			);

			m_LstLabels.push_back            ( tr("Article Name")				);
			m_LstOriginalAttributes.push_back( properties::ARTICLE_NAME			);

			m_LstLabels.push_back            ( tr("Pack")						);
			m_LstOriginalAttributes.push_back( properties::PACKGROESSE			);

			m_LstLabels.push_back            ( tr("Form")						);
			m_LstOriginalAttributes.push_back( properties::DARREICHFORM			);

			m_LstLabels.push_back            ( tr("Manufacturer")				);
			m_LstOriginalAttributes.push_back( properties::MANUFACTURER_NO		);

			m_LstLabels.push_back            ( tr("Manufacturer Name")			);
			m_LstOriginalAttributes.push_back( properties::MANUFACTURER_NAME	);
		}

		basar::Int32 ArticlePickerVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( TenderOverviewVC, initMatcher )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_ArticleMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twArticle, propTab );
            m_ArticleMatcher.transformProperties( ui.twArticle->horizontalHeaderLabels(), m_LstOriginalAttributes );

			basar::Int32 count = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();

			while (!yit.isEnd())
			{
				++count;
				++yit;
			}
			return count;
        }

		void ArticlePickerVC::on_DialogClose_Requested()
		{
			METHODNAME_DEF( ArticlePickerVC, on_DialogClose_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			this->reject();
		}

		void ArticlePickerVC::on_Search_Requested()
		{
			METHODNAME_DEF( ArticlePickerVC, on_Search_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( ui.leArticleNo->text().isEmpty() && ui.leArticleName->text().isEmpty() )
			{
				showWarningMessage( tr("No search criteria.").toLocal8Bit().constData() );
				ui.leArticleNo->setFocus();

				return;
			}

			if( ui.leArticleNo->text().isEmpty() &&	3 > ui.leArticleName->text().count() )
			{
				showInfoMessage( tr("Please enter at least 3 characters into the article name field.").toLocal8Bit().constData() );
				ui.leArticleName->setFocus();
				ui.leArticleName->selectAll();

				return;
			}

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchArticleRequested" );
		}

		void ArticlePickerVC::on_TakeOver_Requested()
		{
			METHODNAME_DEF( ArticlePickerVC, on_TakeOver_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_PickedArticle = m_ArticleMatcher.getCurrentRight();
			this->reject();
		}

		void ArticlePickerVC::focusInEvent( QFocusEvent* event)
        {
            event;
			ui.leArticleNo->setFocus();
        }

		void ArticlePickerVC::displayNoArticleFound()
		{
			showInfoMessage( tr("No article found").toLocal8Bit().constData() );
			
			ui.btnTakeOver->setDisabled( true );

			if( !ui.leArticleNo->text().isEmpty() )
			{
				ui.leArticleNo->setFocus();
				ui.leArticleNo->selectAll();
				return;
			}

			if( !ui.leArticleName->text().isEmpty() )
			{
				ui.leArticleName->setFocus();
				ui.leArticleName->selectAll();
				return;
			}
		}

		void ArticlePickerVC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
            METHODNAME_DEF( ArticlePickerVC, setSearchCriteria )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::ARTICLE_CODE ) )
			{
				ui.leArticleNo->setText( yit.getString( properties::ARTICLE_CODE ).c_str() );
			}
			else
			{
				ui.leArticleNo->setText( "" );
			}

			if( yit.isContainedAndSet( properties::TENDER_TYPE ) )
			{	
				ComboBox::setCurrentIndex( ui.cbBusinessType, yit.getInt32( properties::TENDER_TYPE ) );			
			}
			else
			{
				ui.cbBusinessType->addItem("All", ui.cbBusinessType->count() );
				ui.cbBusinessType->setCurrentIndex( ui.cbBusinessType->count() - 1 );
			}	
		}

		void ArticlePickerVC::setupSearchCriteria()
		{
			METHODNAME_DEF( ArticlePickerVC, setupSearchCriteria )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.leArticleNo->setAccessibleName( properties::ARTICLE_CODE.getName().c_str() );

			QRegExp rxArticleNo( "^[a-zA-Z0-9]{1,13}$" );
			QValidator *articleNoValidator = new QRegExpValidator( rxArticleNo, ui.leArticleNo );
			ui.leArticleNo->setValidator( articleNoValidator );

			ui.leArticleName->setAccessibleName(properties::ARTICLE_NAME.getName().c_str());

			QRegExp rxArticleName( "^([^\\W]+\\s)*" );
			QValidator *articleNameValidator = new QRegExpValidator( rxArticleName, ui.leArticleName );
			ui.leArticleName->setValidator( articleNameValidator );
		}

		void ArticlePickerVC::reject()
		{
			clear();
			QDialog::reject();
		}

    } //end namespace viewConn
} //end namespace tender
