//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderarticleinfo.h"
#include "loggerpool/tenderloggerpool.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/tender/itender.h"

#include <libtender/domainmodule/tender/tendertypecollection.h>
#include <libtender/domainmodule/tender/tenderstatecollection.h>
#include <libtender/domainmodule/constants.h>

#include <libutil/viewconn.h>
#include "util.h"
#include "doublevalidator.h"

#include "util.h"


//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {

        TenderArticleInfo::TenderArticleInfo(  QWidget * parent , const basar::Int16 area)
        : m_EventsWiring  ( "TenderArticleInfo::EventsWiring"   ),
        m_Logger( libtender::LoggerPool::getLoggerViewConn() ),
        m_ArticleInputValidator( parent ),
        m_ArticlePriceDiscountInputValidator( parent, true ),
        m_Area(area)
        {
            ui.setupUi(this);

            wireEvents();

            setupArticleInfo();

            resetArticle();
        }

        TenderArticleInfo::~TenderArticleInfo()
        { }

        void TenderArticleInfo::resetArticle()
        {
	        ui.leArticleName->setText( "" );
        }

        void TenderArticleInfo::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }

            connect( ui.pbArticleCode,         SIGNAL( clicked()	   ), 
            this,                              SLOT  ( onStartSearch() ));

            connect( ui.leArticleCode,         SIGNAL( returnPressed()	), 
			this,							   SLOT  ( onCheckArticle() ));

            connect( ui.leArticleCode,         SIGNAL( editingFinished() ), 
			this,							   SLOT  ( onCheckArticle()  ));

            connect( &m_ArticleInputValidator, SIGNAL( inputValid()		   ),
            this,                              SIGNAL( inputCodeQtyValid() ));
			
            connect( &m_ArticleInputValidator, SIGNAL( inputInvalid()		 ),
            this,                              SIGNAL( inputCodeQtyInvalid() ));
                        
            m_EventsWiring.set();
        }

        void TenderArticleInfo::setupArticleInfo()
        {
            METHODNAME_DEF( TenderArticleInfo, setupArticleInfo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            ui.leArticleNo->setAccessibleName( QString::fromLocal8Bit( properties::ARTICLE_NO.getName().c_str() ) );
            ui.leArticleCode->setAccessibleName( QString::fromLocal8Bit( properties::ARTICLE_CODE.getName().c_str() ) );
            ui.leArticleName->setAccessibleName( QString::fromLocal8Bit( properties::ARTICLE_NAME.getName().c_str() ) );
            ui.leQuantity->setAccessibleName( QString::fromLocal8Bit( properties::CONTRACT_QTY.getName().c_str() ) );
            ui.leDiscount->setAccessibleName( QString::fromLocal8Bit( properties::ADD_DISCOUNT_PCT.getName().c_str() ) );
            ui.lePrice->setAccessibleName( QString::fromLocal8Bit( properties::CONTRACT_PRICE.getName().c_str() ) );

            if (constants::REGION_RS == m_Area) // The check box for blocking/unblocking an article is available only for Serbia.
            {
                ui.checkBoxBlocked->setAccessibleName(QString::fromLocal8Bit(properties::TENDER_STATE.getName().c_str()));
            }

            if (constants::REGION_CH != m_Area && constants::REGION_RS != m_Area )
            {
                ui.checkBoxBlocked->hide();
            }

            ui.leArticleNo->hide();

            QRegExp rxArticleNo( "^[a-zA-Z0-9]{1,13}$" );
            QValidator *articleNoValidator = new QRegExpValidator( rxArticleNo, ui.leArticleCode );
            ui.leArticleCode->setValidator( articleNoValidator );

            QRegExp rxQty("^[1-9]{1}([0-9]{1,8})?$" );
            QValidator *qtyValidator = new QRegExpValidator( rxQty, ui.leQuantity );
            ui.leQuantity->setValidator( qtyValidator );

            QRegExp rxDiscount( "^[0-9]{1,2}(\\,[0-9]{1,2})?$|^[0-9]{1,2}(\\.[0-9]{1,2})?$" );
            QValidator *discountValidator = new QRegExpValidator( rxDiscount, ui.leDiscount );
            ui.leDiscount->setValidator( discountValidator  );

            QRegExp rxPrice( "^\\d{1,7}(\\.\\d{1,2})?$|^\\d{1,7}(\\,\\d{1,2})?$" );
            QValidator *priceValidator = new QRegExpValidator( rxPrice, ui.lePrice );
            ui.lePrice->setValidator( priceValidator );
        }
        
        void TenderArticleInfo::addArtValidator()
        {
            m_ArticlePriceDiscountInputValidator.addObject( ui.leDiscount );
            m_ArticlePriceDiscountInputValidator.addObject( ui.lePrice );
            m_ArticleInputValidator.addObject( ui.leArticleCode );
            m_ArticleInputValidator.addObject( ui.leQuantity );
            m_ArticleInputValidator.addObject( &m_ArticlePriceDiscountInputValidator );
        }

        void TenderArticleInfo::matchToTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator tenderPosition )
        {
            METHODNAME_DEF( TenderArticleInfo, matchToTenderPosition )
            BLOG_TRACE_METHOD( m_Logger, fun );

            std::string articleCode = ui.leArticleCode->text().toStdString();
            tenderPosition.setString( properties::ARTICLE_CODE, articleCode );

            tenderPosition.setInt32( properties::ARTICLE_NO, ui.leArticleNo->text().toInt() );

            tenderPosition.setString( properties::ARTICLE_NAME, ui.leArticleName->text().toLocal8Bit().constData() );
            tenderPosition.setInt32(properties::CONTRACT_QTY, ui.leQuantity->text().toInt());
            
            if (constants::REGION_RS == m_Area)
            {
                tenderPosition.setInt16(properties::TENDER_STATE, ui.checkBoxBlocked->isChecked() ? 8 : 1);
            }

            if (constants::REGION_RS == m_Area)
            {
                tenderPosition.setInt16(properties::TENDER_STATE, ui.checkBoxBlocked->isChecked() ? 8 : 1);
            }

            if( !ui.leDiscount->text().isEmpty() )
            {
                basar::Decimal discount;
                basar::VarString discountText = ui.leDiscount->text().toStdString();

                if( discountText.find(',', 0) != std::string::npos )
                {
                    discount.fromString( discountText );
                }
                else
                {
                    if( discountText.find('.', 0) != std::string::npos )
                    {
                        std::replace(discountText.begin(), discountText.end(), '.', ',');
                    }
                    discount.fromString( discountText );
                }
            
                tenderPosition.setDecimal( properties::ADD_DISCOUNT_PCT, discount );
            }

            if( !ui.lePrice->text().isEmpty() )
            {
                basar::Decimal price;
                basar::VarString test = ui.lePrice->text().toStdString();

                if( test.find(',', 0) != std::string::npos )
                {
                    price.fromString( test );
                }
                else
                {
                    if( test.find('.', 0) != std::string::npos )
                    {
                        std::replace(test.begin(), test.end(), '.', ',');
                    }
                    price.fromString( test );
                }
                tenderPosition.setDecimal( properties::CONTRACT_PRICE, price );
            }

            // setting these values 0. These fields are applicable only for switzerland.
            tenderPosition.setInt32( properties::OWNQUOTA_QTY, 0 );
            tenderPosition.setInt16( properties::OWNQUOTA_FLAG, 0);
            tenderPosition.setInt16( properties::RECALL_FLAG, 0 );
        }

        void TenderArticleInfo::matchFromTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator tenderPosition )
        {
            METHODNAME_DEF( TenderArticleInfo, matchFromTenderPosition )
            BLOG_TRACE_METHOD( m_Logger, fun );

            basar::db::aspect::AccessorPropertyTableRef tenderPositionPropTab = tenderPosition.getPropertyTable();
            basar::gui::tie::MatcherRef matcher = basar::gui::tie::Manager::getInstance().createMatcher( this , tenderPositionPropTab, basar::gui::tie::WITHOUT_COMBOBOX );
            matcher.RightToLeft( matcher.getCurrentLeft(), tenderPosition );

            ui.checkBoxBlocked->setChecked(false);

            if (tenderPosition.isContainedAndSet(properties::TENDER_STATE))
            {
                ui.checkBoxBlocked->setChecked(8 == tenderPosition.getInt16(properties::TENDER_STATE));
            }

            if (constants::REGION_RS != m_Area && constants::REGION_CH != m_Area)
            {
                ui.checkBoxBlocked->hide();
            }

            ui.leArticleCode->setFocus();

        }

        void TenderArticleInfo::onStartSearch()
        {
            METHODNAME_DEF( TenderArticleInfo, OnStartSearch )
            BLOG_TRACE_METHOD( m_Logger, fun );

            libutil::gui::WaitCursor bc( this);
            basar::appl::SystemEventManager::getInstance().fire( "SearchArticleRequested" );
        }

		void TenderArticleInfo::onCheckArticle()
		{
			METHODNAME_DEF( TenderArticleInfo, onCheckArticle )
			BLOG_TRACE_METHOD( m_Logger, fun );
		
			basar::appl::SystemEventManager::getInstance().fire( "CheckArticleRequested" );
		}
        

        basar::VarString TenderArticleInfo::getArticleCode()
        {
            METHODNAME_DEF( TenderArticleInfo, getArticleCode )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return ui.leArticleCode->text().toStdString();
        }
        
        void TenderArticleInfo::resetArticleName()
        {
            ui.leArticleName->setText( "" ); 
        }

        void TenderArticleInfo::resetTenderPosition()
        {
            ui.leArticleNo->setText( "" );
            ui.leArticleCode->setText( "" );
            ui.leQuantity->setText( "" );
            ui.leDiscount->setText( "" );
            ui.lePrice->setText( "" );
            ui.leArticleName->setText( "" );
        }

		//TODO DZ one function
        void TenderArticleInfo::disableArticleCode()
        {
            METHODNAME_DEF( TenderArticleInfo, disableArticleCode )
            BLOG_TRACE_METHOD( m_Logger, fun );

            ui.pbArticleCode->setDisabled(true);
            ui.leArticleCode->setDisabled(true);
        }

        void TenderArticleInfo::enableArticleCode()
        {
            METHODNAME_DEF( TenderArticleInfo, enableArticleCode )
            BLOG_TRACE_METHOD( m_Logger, fun );

            ui.pbArticleCode->setEnabled(true);
            ui.leArticleCode->setEnabled(true);
        }

        void TenderArticleInfo::enableInputValidator( bool ip)
        {
            if(ip == true)
            {
                m_ArticleInputValidator.setEnabled();
            }
            else
            {
                m_ArticleInputValidator.setDisabled();
            }
        }

        void TenderArticleInfo::setFocusArticleCode()
        {
            ui.leArticleCode->setFocus();
        }

        void TenderArticleInfo::matchFromArticle( basar::db::aspect::AccessorPropertyTable_YIterator article )
        {
            METHODNAME_DEF( TenderArticleInfo, matchFromArticle )
            BLOG_TRACE_METHOD( m_Logger, fun );

            if (article.isNull() || article.isEnd())
            {
                ui.leArticleNo->clear();
                ui.leArticleCode->clear();
                ui.leArticleName->clear();
            }
            else
            {
                ui.leArticleNo  ->setText( QString::number       ( article.getInt32 ( properties::ARTICLE_NO )           ) );
                ui.leArticleCode->setText( QString::fromLocal8Bit( article.getString( properties::ARTICLE_CODE ).c_str() ) );
                ui.leArticleName->setText( QString::fromLocal8Bit( article.getString( properties::ARTICLE_NAME ).c_str() ) );
            }
        }
        
        void TenderArticleInfo::setFocus()
        {
            ui.leArticleCode->setFocus();
        }

        void TenderArticleInfo::selectArticlecode()
        {
            ui.leArticleCode->selectAll();
        }

        void TenderArticleInfo::setFocusQuantity()
        {
            ui.leQuantity->setFocus();
        }

        void TenderArticleInfo::selectQuantity()
        {
            ui.leQuantity->selectAll();
        }

        bool TenderArticleInfo::hasFocus()
        {
            return ( ui.leArticleCode->hasFocus() || ui.pbArticleCode->hasFocus() );
        }

        void TenderArticleInfo::focusInEvent( QFocusEvent* event )
        {
            event;
            ui.leArticleCode->setFocus();
        }

        void TenderArticleInfo::focusOutEvent( QFocusEvent* event )
        {
            event;
        }

    }
}
