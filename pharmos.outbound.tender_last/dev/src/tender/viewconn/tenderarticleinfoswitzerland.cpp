//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderarticleinfoswitzerland.h"
#include "loggerpool/tenderloggerpool.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/tender/itender.h"

#include <libtender/domainmodule/tender/tendertypecollection.h>
#include <libtender/domainmodule/tender/tenderstatecollection.h>

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
TenderArticleInfoSwitzerland::TenderArticleInfoSwitzerland(  QWidget * parent )
: m_EventsWiring  ( "TenderArticleInfoSwitzerland::EventsWiring"   ),
  m_Logger( libtender::LoggerPool::getLoggerViewConn() ),
  m_ArticleInputValidator( parent ),
  m_ArticleDiscountInputValidator( parent )
{

    ui.setupUi(this);

    wireEvents();

    setupArticleInfo();

    resetArticle();
}

TenderArticleInfoSwitzerland::~TenderArticleInfoSwitzerland()
{
}

void TenderArticleInfoSwitzerland::resetArticle()
{
	ui.leArticleName->setText( "" );
}

void TenderArticleInfoSwitzerland::wireEvents()
{
    if( m_EventsWiring.isSet() )
    {
        return;
    }

    connect( ui.pbArticleCode,         SIGNAL( clicked() ), 
    this,                              SLOT  ( onStartSearch() ) );

    connect( ui.leArticleCode,         SIGNAL( returnPressed() ), 
    this,                              SLOT  ( onCheckArticle() ) );

    connect( ui.leArticleCode,         SIGNAL( editingFinished() ), 
    this,                              SLOT  ( onCheckArticle() ) );

    connect( &m_ArticleInputValidator, SIGNAL( inputValid() ),
    this,                              SLOT  ( onInputValid() ));

    connect( &m_ArticleInputValidator, SIGNAL( inputInvalid() ),
    this,                              SIGNAL( inputCodeQtyInvalid() ) );
            
    m_EventsWiring.set();
}

void TenderArticleInfoSwitzerland::setupArticleInfo()
{
    METHODNAME_DEF( TenderArticleInfoSwitzerland, setupArticleInfo )
    BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

    ui.leArticleNo->setAccessibleName( QString::fromLocal8Bit( properties::ARTICLE_NO.getName().c_str() ) );
    ui.leArticleCode->setAccessibleName( QString::fromLocal8Bit( properties::ARTICLE_CODE.getName().c_str() ) );
    ui.leArticleName->setAccessibleName( QString::fromLocal8Bit( properties::ARTICLE_NAME.getName().c_str() ) );
    ui.leQuantity->setAccessibleName( QString::fromLocal8Bit( properties::CONTRACT_QTY.getName().c_str() ) );
    ui.leDiscount->setAccessibleName( QString::fromLocal8Bit( properties::ADD_DISCOUNT_PCT.getName().c_str() ) );
    ui.leOwnQuotaQty->setAccessibleName( QString::fromLocal8Bit( properties::OWNQUOTA_QTY.getName().c_str() ) );
    ui.cbOwnQuotaFlag->setAccessibleName( QString::fromLocal8Bit( properties::OWNQUOTA_FLAG.getName().c_str() ) );

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

    QRegExp rxOwnQuotaQty( "^[0-9]{1}([0-9]{1,8})?$" );
    QValidator *ownQuotaQtyValidator = new QRegExpValidator( rxOwnQuotaQty, ui.leOwnQuotaQty );
    ui.leOwnQuotaQty->setValidator( ownQuotaQtyValidator );

    // add the values for the comboboxes discountcalcfrom and discountapplyto
    ui.cmbDiscountCalcFrom->addItem("GEP");
    ui.cmbDiscountCalcFrom->addItem("AGP");

    ui.cmbDiscountApplyTo->addItem("GEP");
    ui.cmbDiscountApplyTo->addItem("AGP");

    //By defaul GEP should be displayed
    ui.cmbDiscountCalcFrom->setCurrentIndex(0);
    //By defaul AGP should be displayed
    ui.cmbDiscountApplyTo->setCurrentIndex(1);
}

void TenderArticleInfoSwitzerland::addArtValidator()
{
    m_ArticleDiscountInputValidator.addObject( ui.leDiscount );
    m_ArticleInputValidator.addObject( ui.leArticleCode );
    m_ArticleInputValidator.addObject( ui.leQuantity );
    m_ArticleInputValidator.addObject( &m_ArticleDiscountInputValidator );
}

void TenderArticleInfoSwitzerland::onInputValid()
{
	if( basar::Decimal() == getDecimal( ui.leDiscount->text() ) )
	{
		emit inputCodeQtyInvalid();
		return;
	}

	emit inputCodeQtyValid();
}

void TenderArticleInfoSwitzerland::matchToTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator tenderPosition )
{
    METHODNAME_DEF( TenderArticleInfoSwitzerland, matchToTenderPosition )
    BLOG_TRACE_METHOD( m_Logger, fun );

    std::string articleCode = ui.leArticleCode->text().toStdString();
    tenderPosition.setString( properties::ARTICLE_CODE, articleCode );

    tenderPosition.setInt32( properties::ARTICLE_NO, ui.leArticleNo->text().toInt() );

    tenderPosition.setString( properties::ARTICLE_NAME, ui.leArticleName->text().toLocal8Bit().constData() );
    tenderPosition.setInt32( properties::CONTRACT_QTY, ui.leQuantity->text().toInt() );
    tenderPosition.setInt32( properties::OWNQUOTA_QTY, ui.leOwnQuotaQty->text().toInt() );
	tenderPosition.setInt16( properties::OWNQUOTA_FLAG, ui.cbOwnQuotaFlag->isChecked() ? 1 : 0 );
	tenderPosition.setInt16( properties::RECALL_FLAG, ui.cbRecallFlag->isChecked() ? 1 : 0 );

    if( !ui.leDiscount->text().isEmpty() )
    {
        tenderPosition.setDecimal( properties::ADD_DISCOUNT_PCT, getDecimal( ui.leDiscount->text() ) );
    }

    basar::Int16 dcfui = basar::Int16(ui.cmbDiscountCalcFrom->currentIndex());
    basar::Int16 datui = basar::Int16(ui.cmbDiscountApplyTo->currentIndex());

    tenderPosition.setInt16(properties::DISCOUNT_CALC_FROM, dcfui);
    tenderPosition.setPropertyState(properties::DISCOUNT_CALC_FROM.getName().c_str(), basar::SS_UPDATE);
    tenderPosition.setInt16(properties::DISCOUNT_APPLY_TO, datui);
    tenderPosition.setPropertyState(properties::DISCOUNT_APPLY_TO.getName().c_str(), basar::SS_UPDATE);
}

const basar::Decimal TenderArticleInfoSwitzerland::getDecimal( const QString & str ) const
{
	basar::Decimal d;
	basar::VarString txt = str.toStdString();

    if( txt.find( ',', 0 ) != std::string::npos )
    {
        d.fromString( txt );
    }
    else
    {
        if( txt.find('.', 0) != std::string::npos )
        {
            std::replace( txt.begin(), txt.end(), '.', ',' );
        }
        d.fromString( txt );
    }

	return d;
}

basar::Int32 TenderArticleInfoSwitzerland::getInt( const QString & str ) const
{
	return str.toInt();
}

void TenderArticleInfoSwitzerland::matchFromTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator tenderPosition )
{
    METHODNAME_DEF( TenderArticleInfoSwitzerland, matchFromTenderPosition )
    BLOG_TRACE_METHOD( m_Logger, fun );

    basar::db::aspect::AccessorPropertyTableRef tenderPositionPropTab = tenderPosition.getPropertyTable();
    basar::gui::tie::MatcherRef matcher = basar::gui::tie::Manager::getInstance().createMatcher( this , tenderPositionPropTab, basar::gui::tie::WITHOUT_COMBOBOX );
    matcher.RightToLeft( matcher.getCurrentLeft(), tenderPosition );

    if( tenderPosition.isContainedAndSet( properties::OWNQUOTA_FLAG  ) )
    {
		ui.cbOwnQuotaFlag->setChecked( 1 == tenderPosition.getInt16( properties::OWNQUOTA_FLAG ) );
    }

    if( tenderPosition.isContainedAndSet( properties::RECALL_FLAG  ) )
    {
		ui.cbRecallFlag->setChecked( 1 == tenderPosition.getInt16( properties::RECALL_FLAG ) );
    }

    //By defaul GEP should be displayed
    ui.cmbDiscountCalcFrom->setCurrentIndex(0);
    if (tenderPosition.isContainedAndSet(properties::DISCOUNT_CALC_FROM) )
    {
        int dcf = tenderPosition.getInt16(properties::DISCOUNT_CALC_FROM);
        ui.cmbDiscountCalcFrom->setCurrentIndex(dcf);
    }

    //By defaul AGP should be displayed
    ui.cmbDiscountApplyTo->setCurrentIndex(1);
    if (tenderPosition.isContainedAndSet(properties::DISCOUNT_APPLY_TO) )
    {
        int dat = tenderPosition.getInt16(properties::DISCOUNT_APPLY_TO);
        ui.cmbDiscountApplyTo->setCurrentIndex(dat);
    }

    ui.leArticleCode->setFocus();
}

void TenderArticleInfoSwitzerland::onStartSearch()
{
    METHODNAME_DEF( TenderArticleInfoSwitzerland, OnStartSearch )
    BLOG_TRACE_METHOD( m_Logger, fun );

    libutil::gui::WaitCursor bc( this);
    basar::appl::SystemEventManager::getInstance().fire( "SearchArticleRequested" );
}

void TenderArticleInfoSwitzerland::onCheckArticle()
{
	METHODNAME_DEF( TenderArticleInfoSwitzerland, onCheckArticle )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::appl::SystemEventManager::getInstance().fire( "CheckArticleRequested" );
}

basar::VarString TenderArticleInfoSwitzerland::getArticleCode()
{
    METHODNAME_DEF( TenderArticleInfoSwitzerland, getArticleCode )
    BLOG_TRACE_METHOD( m_Logger, fun );

    return ui.leArticleCode->text().toStdString();
}
        
void TenderArticleInfoSwitzerland::resetArticleName()
{
    ui.leArticleName->setText( "" ); 
}

void TenderArticleInfoSwitzerland::resetTenderPosition()
{
    ui.leArticleNo->setText( "" );
    ui.leArticleCode->setText( "" );
    ui.leQuantity->setText( "" );
    ui.leDiscount->setText( "" );
    //ui.lePrice->setText( "" );
    ui.leArticleName->setText( "" );
    ui.leOwnQuotaQty->setText( "" );
    ui.cbOwnQuotaFlag->setChecked(false);
    //ui.cbRecallFlag->setChecked(false);
    //By defaul GEP should be displayed
    ui.cmbDiscountCalcFrom->setCurrentIndex(0);
    //By defaul AGP should be displayed
    ui.cmbDiscountApplyTo->setCurrentIndex(1);

}

void TenderArticleInfoSwitzerland::disableArticleCode()
{
    METHODNAME_DEF( TenderArticleInfoSwitzerland, disableArticleCode )
    BLOG_TRACE_METHOD( m_Logger, fun );

    ui.pbArticleCode->setDisabled(true);
    ui.leArticleCode->setDisabled(true);
}

void TenderArticleInfoSwitzerland::enableArticleCode()
{
    METHODNAME_DEF( TenderArticleInfoSwitzerland, enableArticleCode )
    BLOG_TRACE_METHOD( m_Logger, fun );

    ui.pbArticleCode->setEnabled(true);
    ui.leArticleCode->setEnabled(true);

}

void TenderArticleInfoSwitzerland::enableInputValidator( bool ip)
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

void TenderArticleInfoSwitzerland::setFocusArticleCode()
{
    ui.leArticleCode->setFocus();
}

void TenderArticleInfoSwitzerland::matchFromArticle( basar::db::aspect::AccessorPropertyTable_YIterator article )
{
    METHODNAME_DEF( TenderArticleInfoSwitzerland, matchFromArticle )
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
        
void TenderArticleInfoSwitzerland::setFocus()
{
    ui.leArticleCode->setFocus();
}

void TenderArticleInfoSwitzerland::selectArticlecode()
{
    ui.leArticleCode->selectAll();
}

void TenderArticleInfoSwitzerland::setFocusQuantity()
{
    ui.leQuantity->setFocus();
}

void TenderArticleInfoSwitzerland::selectQuantity()
{
    ui.leQuantity->selectAll();
}

bool TenderArticleInfoSwitzerland::hasFocus()
{
    return ( ui.leArticleCode->hasFocus() || ui.pbArticleCode->hasFocus() );
}

void TenderArticleInfoSwitzerland::focusInEvent( QFocusEvent* )
{
    ui.leArticleCode->setFocus();
}

void TenderArticleInfoSwitzerland::focusOutEvent( QFocusEvent* )
{
}

}
}