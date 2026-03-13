#include "cmnvc.h"
#include "customerfindervc.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include "definitions_gui.h"

#include <libbasarlogin_manager.h>

namespace CSC_Batches
{
namespace viewConn
{

//-------------------------------------------------------------------------------------------------//
// class definition
//-------------------------------------------------------------------------------------------------//
CustomerFinderVC::CustomerFinderVC( QWidget *parent )
	: QDialog( parent )
{
	ui.setupUi( this );
    initTitle();
    wireEvents();
}

CustomerFinderVC::~CustomerFinderVC()
{
}

void CustomerFinderVC::init( basar::db::aspect::AccessorPropertyTableRef ref )
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

void CustomerFinderVC::clear()
{
	ui.tblIDF->clearContents();
	ui.tblIDF->setRowCount(0);
    ui.txtPharmacyNo->clear();
	ui.txtMatchCode->clear();
}

basar::gui::tie::WidgetReturnEnum CustomerFinderVC::show()
{
	return basar::gui::tie::getWidgetReturnType( exec() );
}

void CustomerFinderVC::hide()
{
	QDialog::hide();
}

bool CustomerFinderVC::shutdown()
{
	return QDialog::close();
}

void CustomerFinderVC::wireEvents()
{
	connect( ui.btnClose,      SIGNAL( clicked()	                                ),
             this,			   SLOT  ( on_Close() )                                 );
	connect( ui.btnOk,         SIGNAL( clicked()	                                ),
             this,			   SLOT  ( on_Ok() )                                    );
	connect( ui.btnSearch,     SIGNAL( clicked()	                                ),
             this,			   SLOT  ( on_Search() )                                );
																                    
	connect( ui.txtPharmacyNo, SIGNAL( textChanged (const QString &)                ),
             this,			   SLOT  ( on_TextChangedCustomerNo(const QString &) )  );
	connect( ui.txtMatchCode,  SIGNAL( textChanged (const QString &)	            ),
             this,			   SLOT  ( on_TextChangedMatchCode(const QString &)  )  );
																				    
	connect( ui.tblIDF,        SIGNAL( dataSelectionChanged()                       ),
             this,			   SLOT  ( on_SelectionChanged() )                      );
    connect( ui.tblIDF,         SIGNAL( doubleClicked( QModelIndex )                ),
		     this,              SLOT  ( onTableDoubleClicked( QModelIndex ) )       );
}

void CustomerFinderVC::on_Close()
{
    basar::appl::SystemEventManager::getInstance().fire( "DialogCloseRequested" );
}

void CustomerFinderVC::on_Ok()
{
	if(! ( m_SearchResultMatcher.getSelectedRight().isNull() || m_SearchResultMatcher.getSelectedRight().isEnd() ) )
    {
		basar::db::aspect::AccessorPropertyTable_YIterator yit = m_SearchResultMatcher.getSelectedRight();
		basar::appl::SystemEventManager::getInstance().fire( "NewCustomerRequested", yit );
	}
}

void CustomerFinderVC::on_Search()
{
	CSC_Batches::gui::WaitCursor waitCursor( this );
	basar::appl::SystemEventManager::getInstance().fire( "FindCustomerRequested" );
}

void CustomerFinderVC::on_TextChangedCustomerNo(const QString &newVal)
{
	if( ! newVal.isEmpty() )
	{
		ui.btnSearch->setEnabled(true);
		ui.btnSearch->setDefault(true);
		ui.txtMatchCode->setEnabled(false);
	}
	else
	{
		ui.btnSearch->setEnabled(false);
		ui.txtMatchCode->setEnabled(true);
	}
}

void CustomerFinderVC::on_TextChangedMatchCode(const QString &newVal)
{
	if( ! newVal.isEmpty() )
	{
		ui.txtPharmacyNo->setEnabled(false);

		if(newVal.length() >= 3)
		{
			ui.btnSearch->setEnabled(true);
			ui.btnSearch->setDefault(true);
		}
		else
		{
			ui.btnSearch->setEnabled(false);
		}
	}
	else
	{
		ui.txtPharmacyNo->setEnabled(true);
		ui.btnSearch->setEnabled(false);
		ui.btnSearch->setDefault(true);
	}
}

void CustomerFinderVC::on_SelectionChanged()
{
	ui.btnOk->setEnabled(true);
}

void CustomerFinderVC::onTableDoubleClicked( QModelIndex index )
{
    ui.tblIDF->setCurrentCell( index.row(), index.column(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
    basar::appl::SystemEventManager::getInstance().fire( "NewCustomerRequested", m_SearchResultMatcher.getSelectedRight() );
}

void CustomerFinderVC::initSearchCriteria()
{
    ui.txtPharmacyNo->setAccessibleName( libabbauw::properties::PHARMACYNO.getName().c_str()  );
    ui.txtMatchCode->setAccessibleName(  libabbauw::properties::MATCHCODE.getName().c_str()   );

	ui.txtPharmacyNo->setValidator( new QRegExpValidator( QRegExp("[0-9]{1,8}" ), ui.txtPharmacyNo ) );
	ui.txtMatchCode->setValidator(  new QRegExpValidator( QRegExp("[-*a-zA-Z0-9 ]{1,15}" ), ui.txtMatchCode ) );

	ui.txtMatchCode->setFocus();
}

void CustomerFinderVC::initTable( QStringList& lstLables )
{
	ui.tblIDF->setColumnCount              ( lstLables.size()                     );
    ui.tblIDF->setHorizontalHeaderLabels   ( lstLables                            );
    ui.tblIDF->setResizeRowsToContents	   ( true								  );
	ui.tblIDF->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn                );
    ui.tblIDF->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded                );
    ui.tblIDF->setSelectionBehavior        ( QAbstractItemView::SelectRows        );
    ui.tblIDF->setSelectionMode            ( QAbstractItemView::SingleSelection   );
    ui.tblIDF->hideVerticalHeader          ( true                                 );
    
    for( int i = 0; i < lstLables.size(); ++i )
    {
		ui.tblIDF->setEditableColumn( i , false );
    }

	ui.tblIDF->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::ResizeToContents ); // IDFNo
	ui.tblIDF->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::ResizeToContents ); // Matchcode
	ui.tblIDF->horizontalHeader()->setSectionResizeMode( 2, QHeaderView::Stretch          ); // Pharmacy
	ui.tblIDF->horizontalHeader()->setSectionResizeMode( 3, QHeaderView::Stretch          ); // CustomerName
	ui.tblIDF->horizontalHeader()->setSectionResizeMode( 4, QHeaderView::ResizeToContents ); // Location
	ui.tblIDF->horizontalHeader()->setSectionResizeMode( 5, QHeaderView::ResizeToContents ); // Postcode
	ui.tblIDF->horizontalHeader()->setSectionResizeMode( 6, QHeaderView::Stretch          ); // Address

	ui.tblIDF->setTextAlignmentColumn( 0, Qt::AlignRight   |  Qt::AlignVCenter );		  	 // IDFNo
	ui.tblIDF->setTextAlignmentColumn( 1, Qt::AlignLeft    |  Qt::AlignVCenter );		  	 // Matchcode
	ui.tblIDF->setTextAlignmentColumn( 2, Qt::AlignLeft    |  Qt::AlignVCenter );	      	 // Pharmacy
	ui.tblIDF->setTextAlignmentColumn( 3, Qt::AlignLeft    |  Qt::AlignVCenter );		  	 // CustomerName
	ui.tblIDF->setTextAlignmentColumn( 4, Qt::AlignLeft    |  Qt::AlignVCenter );		  	 // Location
	ui.tblIDF->setTextAlignmentColumn( 5, Qt::AlignRight   |  Qt::AlignVCenter );		 	 // Postcode
	ui.tblIDF->setTextAlignmentColumn( 6, Qt::AlignLeft    |  Qt::AlignVCenter );		 	 // Address
}

void CustomerFinderVC::initHeaderLables(QStringList& lstLables, std::list<basar::VarString>& lstOriginalAttributes)
{
	 METHODNAME_DEF( CustomerFinderVC, initHeaderLables )
    BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

	using namespace libabbauw::properties;

    lstLables.push_back            ( tr("IDFNo")             );
    lstOriginalAttributes.push_back( PHARMACYNO.getName()    );

	lstLables.push_back            ( tr("Matchcode")         );
    lstOriginalAttributes.push_back( MATCHCODE.getName()     );

	lstLables.push_back            ( tr("Pharmacy")          );
    lstOriginalAttributes.push_back( PHARMACYNAME.getName()  );

	lstLables.push_back            ( tr("CustomerName")      );
    lstOriginalAttributes.push_back( CUSTOMER_NAME.getName() );

    lstLables.push_back            ( tr("Location")          );
    lstOriginalAttributes.push_back( LOCATION.getName()      );

	lstLables.push_back            ( tr("Postcode")          );
    lstOriginalAttributes.push_back( POSTCODE.getName()      );

	lstLables.push_back            ( tr("Address")           );
    lstOriginalAttributes.push_back( ADDRESS.getName()       );
}

void CustomerFinderVC::initMatcher(std::list<basar::VarString>& listPropTable)
{
	m_SearchResultMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.tblIDF, m_Results );
    m_SearchResultMatcher.transformProperties( ui.tblIDF->horizontalHeaderLabels(), listPropTable );
}

void CustomerFinderVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
{
    basar::gui::tie::MatcherRef matcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpSearch, 
                                                                                                 yitSearch.getPropertyTable() );
	matcher.LeftToRight( );
}

void CustomerFinderVC::initTitle()
{
    QString windowTitle;
	windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_IDFFINDER ).append( DLG_TITLESHORT2 );
	windowTitle.append( tr( "Pharmacy Search" ) );
	windowTitle.append( tr( " area: " ) );
	basar::I18nString areaName = basar::login::Manager::getInstance().getAreaName();
	windowTitle.append( QString::fromLocal8Bit( areaName.c_str() ) );
	setWindowTitle( windowTitle );
}

void CustomerFinderVC::info( const basar::I18nString& text )
{
	basar::gui::tie::infoMsgBox( this, text.c_str(), tr("Info").toLocal8Bit().constData() );
}

void CustomerFinderVC::error( const basar::I18nString& text )
{
	basar::gui::tie::criticalMsgBox( this, text.c_str(), tr("Error").toLocal8Bit().constData() );
}

const basar::gui::tie::MessageBoxButtonEnum CustomerFinderVC::question( const basar::I18nString& text )
{
	return basar::gui::tie::questionMsgBox( this, text.c_str(), tr("Question").toLocal8Bit().constData() );
}

void CustomerFinderVC::matchSearchResults()
{
	libutil::gui::SignalBlocker signalblocker( ui.tblIDF );

    m_SearchResultMatcher.getRight().size();
	ui.tblIDF->setRowCount( static_cast<int>(m_Results.size()) );
    ui.tblIDF->resizeRowsToContents();

	ui.btnOk->setEnabled(false);
	ui.tblIDF->clearSelection();

    const basar::Int32 matchedHits = m_SearchResultMatcher.RightToLeft( 0, ui.tblIDF->rowCount() );
	if( matchedHits == 0 )
    { 
		 //info( tr("No searchresults. Either you didn't enter any searchcriteria or for this criteria there are no entries.").toLocal8Bit().constData() );
	}
	else
	{
		ui.btnOk->setEnabled(true);
		ui.btnOk->setDefault(true);

		ui.tblIDF->setRowCount( static_cast<int>(m_Results.size()) );
        ui.tblIDF->resizeRowsToContents();
		ui.tblIDF->selectRow(0);
		ui.tblIDF->setFocus();
	}
}
}
}
