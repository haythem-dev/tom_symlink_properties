#include "cmnvc.h"
#include "articlefindervc.h"

#include "definitions_gui.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include "waitcursor.h"
#include <libbasarlogin_manager.h>
#include <QtWidgets/QToolTip>

namespace CSC_Batches
{
namespace viewConn
{

//-------------------------------------------------------------------------------------------------//
// class definition
//-------------------------------------------------------------------------------------------------//
ArticleFinderVC::ArticleFinderVC( QWidget *parent )
	: QDialog( parent )
{
	ui.setupUi( this );
    initTitle();
    wireEvents();
}

ArticleFinderVC::~ArticleFinderVC()
{
}

void ArticleFinderVC::init( basar::db::aspect::AccessorPropertyTableRef ref )
{
	m_Results = ref;

	QStringList listLables;
	std::list<basar::VarString> listPropTab;

	initHeaderLables( listLables, listPropTab );
	initTable( listLables );
	initSearchCriteria();
	initMatcher( listPropTab );

	ui.btnSearch->setEnabled(false);
	ui.btnSearch->setDefault(true);
	ui.btnOk->setEnabled(false);

	clear();
}

void ArticleFinderVC::clear()
{
	ui.tblArticle->clearContents();
	ui.tblArticle->setRowCount(0);
	ui.txtArticleNo->clear();
	ui.txtMatchCode->clear();
	ui.txtEAN->clear();
}

basar::gui::tie::WidgetReturnEnum ArticleFinderVC::show()
{
	return basar::gui::tie::getWidgetReturnType( exec() );
}

void ArticleFinderVC::hide()
{
	QDialog::hide();
}

bool ArticleFinderVC::shutdown()
{
	return QDialog::close();
}

void ArticleFinderVC::wireEvents()
{
	connect( ui.btnClose,       SIGNAL( clicked()	                                  ),
             this,			    SLOT  ( on_Close()                                )   );
	connect( ui.btnOk,          SIGNAL( clicked()	                                  ),
             this,			    SLOT  ( on_Ok() )                                     );
	connect( ui.btnSearch,      SIGNAL( clicked()	                                  ),
             this,			    SLOT  ( on_Search()                               )   );	
																					  
	connect( ui.txtArticleNo,   SIGNAL( textChanged	           (const QString &)      ),
             this,			    SLOT  ( on_TextChangedArticleNo(const QString &)  )   );
	connect( ui.txtMatchCode,   SIGNAL( textChanged	           (const QString &)      ),	  
             this,			    SLOT  ( on_TextChangedMatchCode(const QString &)  )   );
	connect( ui.txtEAN,         SIGNAL( textChanged	           (const QString &)      ),
             this,			    SLOT  ( on_TextChangedEAN      (const QString &)  )   );

	connect( ui.tblArticle,     SIGNAL( dataSelectionChanged()                        ),
             this,			    SLOT  ( on_SelectionChanged()                     )   );
    connect( ui.tblArticle,     SIGNAL( doubleClicked( QModelIndex )                  ),
		     this,              SLOT  ( onTableDoubleClicked( QModelIndex ) )         );
																					  
}

void ArticleFinderVC::on_Close()
{
    basar::appl::SystemEventManager::getInstance().fire( "DialogCloseRequested" );
}

void ArticleFinderVC::on_Ok()
{
	if(! ( m_SearchResultMatcher.getSelectedRight().isNull() || m_SearchResultMatcher.getSelectedRight().isEnd() ) )
    {
        basar::db::aspect::AccessorPropertyTable_YIterator yit = m_SearchResultMatcher.getSelectedRight();
		basar::appl::SystemEventManager::getInstance().fire( "NewArticleRequested", yit );
	}
}

void ArticleFinderVC::on_Search()
{
	CSC_Batches::gui::WaitCursor waitCursor( this );
	basar::appl::SystemEventManager::getInstance().fire( "FindArticleRequested" );
}

void ArticleFinderVC::on_TextChangedArticleNo(const QString &newVal)
{
	if( ! newVal.isEmpty() )
	{
		ui.btnSearch->setEnabled(true);
		ui.btnSearch->setDefault(true);
		ui.txtMatchCode->setEnabled(false);
		ui.txtEAN->setEnabled(false);
	}
	else
	{
		ui.txtMatchCode->setEnabled(true);
		ui.txtEAN->setEnabled(true);
		ui.btnSearch->setEnabled(false);
	}
}

void ArticleFinderVC::on_TextChangedMatchCode(const QString &newVal)
{
	if( ! newVal.isEmpty() )
	{
		ui.txtArticleNo->setEnabled(false);
		ui.txtEAN->setEnabled(false);

		if(newVal.length() >= 3)
		{
			ui.btnSearch->setEnabled(true);
			ui.btnSearch->setDefault(true);
		}
		else
        {
			ui.btnSearch->setEnabled(false);
            QToolTip::showText( ui.txtMatchCode->mapToGlobal( QPoint( 5, 5 ) ), tr("Enter at least 3 characters") );
        }
	}
	else
	{
		ui.txtArticleNo->setEnabled(true);
		ui.txtEAN->setEnabled(true);
		ui.btnSearch->setEnabled(false);
	}

}

void ArticleFinderVC::on_TextChangedEAN(const QString &newVal)
{
	if( ! newVal.isEmpty() )
	{
		ui.btnSearch->setEnabled(true);
		ui.btnSearch->setDefault(true);
		ui.txtMatchCode->setEnabled(false);
		ui.txtArticleNo->setEnabled(false);
	}
	else
	{
		ui.txtArticleNo->setEnabled(true);
		ui.txtMatchCode->setEnabled(true);
		ui.btnSearch->setEnabled(false);
	}
	ui.btnSearch->setDefault(true);
}

void ArticleFinderVC::on_SelectionChanged()
{
	ui.btnOk->setEnabled(true);
}

void ArticleFinderVC::onTableDoubleClicked( QModelIndex index )
{
    ui.tblArticle->setCurrentCell( index.row(), index.column(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
    basar::appl::SystemEventManager::getInstance().fire( "NewArticleRequested", m_SearchResultMatcher.getSelectedRight() );
}

void ArticleFinderVC::initSearchCriteria()
{
    ui.txtArticleNo->setAccessibleName( libabbauw::properties::PZN.getName().c_str()  );
    ui.txtMatchCode->setAccessibleName( libabbauw::properties::MATCHCODE.getName().c_str()   );
	ui.txtEAN->setAccessibleName(       libabbauw::properties::EAN.getName().c_str()	);

	ui.txtArticleNo->setValidator(     new QRegExpValidator( QRegExp("[0-9]{1,8}" ), ui.txtArticleNo ));
	ui.txtMatchCode->setValidator(	   new QRegExpValidator( QRegExp(".{3,12}" ), ui.txtMatchCode ));
	ui.txtEAN->setValidator(           new QRegExpValidator( QRegExp("[0-9]{13}" ), ui.txtEAN ));

	ui.txtMatchCode->setFocus();
}

void ArticleFinderVC::initTable( QStringList& lstLables )
{
	ui.tblArticle->setColumnCount              ( lstLables.size()                     );
    ui.tblArticle->setHorizontalHeaderLabels   ( lstLables                            );
    ui.tblArticle->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn                );
    ui.tblArticle->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded                );
    ui.tblArticle->setSelectionBehavior        ( QAbstractItemView::SelectRows        );
    ui.tblArticle->setSelectionMode            ( QAbstractItemView::SingleSelection   );
    ui.tblArticle->hideVerticalHeader          ( true                                 );
	
	ui.tblArticle->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // PZN
	ui.tblArticle->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // EAN
	ui.tblArticle->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Pharmaceuticalform
	ui.tblArticle->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);		   // Article Name
	ui.tblArticle->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Norm Pack
	ui.tblArticle->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents); // Unit

	ui.tblArticle->setTextAlignmentColumn( 0, Qt::AlignRight   |  Qt::AlignVCenter );		   // PZN
	ui.tblArticle->setTextAlignmentColumn( 1, Qt::AlignRight   |  Qt::AlignVCenter );		   // EAN
	ui.tblArticle->setTextAlignmentColumn( 2, Qt::AlignLeft    |  Qt::AlignVCenter );		   // Pharmaceuticalform
	ui.tblArticle->setTextAlignmentColumn( 3, Qt::AlignLeft    |  Qt::AlignVCenter );	       // Article Name
	ui.tblArticle->setTextAlignmentColumn( 4, Qt::AlignHCenter |  Qt::AlignVCenter );		   // Norm Pack
	ui.tblArticle->setTextAlignmentColumn( 5, Qt::AlignRight   |  Qt::AlignVCenter );		   // Unit
		
    for( int i = 0; i < lstLables.size(); ++i )
    {
		ui.tblArticle->setEditableColumn( i , false );
    }
}

void ArticleFinderVC::initHeaderLables(QStringList& lstLables, std::list<basar::VarString>& lstOriginalAttributes)
{
	 METHODNAME_DEF( ArticleFinderVC, initHeaderLables )
    BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

	using namespace libabbauw::properties;

    lstLables.push_back            ( tr("PZN")					  );
    lstOriginalAttributes.push_back( PZN.getName()				  );
																  
	lstLables.push_back            ( tr("EAN")					  );
    lstOriginalAttributes.push_back( EAN.getName()				  );			
																  
	lstLables.push_back            ( tr("PF")                     );
    lstOriginalAttributes.push_back( PHARMACEUTICALFORM.getName() );

	lstLables.push_back            ( tr("Article Name")           );
    lstOriginalAttributes.push_back( ARTICLENAME.getName()        );

	lstLables.push_back            ( tr("Norm Pack")              );
    lstOriginalAttributes.push_back( NORMPACK.getName()           );
                                                              
	lstLables.push_back            ( tr("Unit")                   );
    lstOriginalAttributes.push_back( UNIT.getName()               );
}

void ArticleFinderVC::initMatcher(std::list<basar::VarString>& listPropTable)
{
	m_SearchResultMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.tblArticle, m_Results );
    m_SearchResultMatcher.transformProperties( ui.tblArticle->horizontalHeaderLabels(), listPropTable );
}

void ArticleFinderVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
{
    basar::gui::tie::MatcherRef matcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpSearch, 
                                                                                                 yitSearch.getPropertyTable() );
	matcher.LeftToRight( );

	//TODO: workaround basar bug bitte ausbauen!!
	if(ui.txtArticleNo->text().isEmpty()) 
	{
		yitSearch.setPropertyState( libabbauw::properties::PZN, basar::SS_UNSET);
	}
	if(ui.txtEAN->text().isEmpty()) 
	{
		yitSearch.setPropertyState( libabbauw::properties::EAN, basar::SS_UNSET);
	}
	if(ui.txtMatchCode->text().isEmpty())
	{
		yitSearch.setPropertyState( libabbauw::properties::MATCHCODE, basar::SS_UNSET);
	}
}

void ArticleFinderVC::initTitle()
{
    QString windowTitle;
	windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_ARTICLEFINDER ).append( DLG_TITLESHORT2 );
	windowTitle.append( tr( "Article Search" ) );
	windowTitle.append( tr( " area: " ) );
	basar::I18nString areaName = basar::login::Manager::getInstance().getAreaName();
	windowTitle.append( QString::fromLocal8Bit( areaName.c_str() ) );
	setWindowTitle( windowTitle );
}

void ArticleFinderVC::info( const basar::I18nString& text )
{
	basar::gui::tie::infoMsgBox( this, text.c_str(), tr("Info").toLocal8Bit().constData() );
}

void ArticleFinderVC::error( const basar::I18nString& text )
{
	basar::gui::tie::criticalMsgBox( this, text.c_str(), tr("Error").toLocal8Bit().constData() );
}

const basar::gui::tie::MessageBoxButtonEnum ArticleFinderVC::question( const basar::I18nString& text )
{
	return basar::gui::tie::questionMsgBox( this, text.c_str(), tr("Question").toLocal8Bit().constData() );
}

void ArticleFinderVC::matchSearchResults()
{
	libutil::gui::SignalBlocker signalblocker( ui.tblArticle );

    m_SearchResultMatcher.getRight().size();
	ui.tblArticle->setRowCount( static_cast<int>(m_Results.size()) );
	ui.tblArticle->resizeRowsToContents();

	ui.btnOk->setEnabled(false);
	ui.tblArticle->clearSelection();

    const basar::Int32 matchedHits = m_SearchResultMatcher.RightToLeft( 0, ui.tblArticle->rowCount() );
	if( matchedHits == 0 )
    { 
		 //info( tr("No searchresults. Either you didn't enter any searchcriteria or for this criteria there are no entries.").toLocal8Bit().constData() );
	}
	else
	{
		ui.btnOk->setEnabled(true);
		ui.btnOk->setDefault(true);

		ui.tblArticle->setRowCount( static_cast<int>(m_Results.size()) );
        ui.tblArticle->resizeRowsToContents();
		ui.tblArticle->selectRow(0);
		ui.tblArticle->setFocus();
	}
}

void ArticleFinderVC::setMatchCode( const basar::VarString& matchCode )
{
    ui.txtMatchCode->setText( QString( matchCode.c_str() ) );
}

}
}
