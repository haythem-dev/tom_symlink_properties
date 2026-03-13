#include "cmnvc.h"
#include "manufacturerfindervc.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"
#include "definitions_gui.h"

#include <libbasarlogin_manager.h>

namespace CSC_Batches
{
namespace viewConn
{

ManufacturerFinderVC::ManufacturerFinderVC( QWidget *parent )
	: QDialog(parent)
{
	ui.setupUi(this);
    initTitle();
    wireEvents();
}

ManufacturerFinderVC::~ManufacturerFinderVC()
{
}

void ManufacturerFinderVC::init( basar::db::aspect::AccessorPropertyTableRef ref )
{
	m_Results   = ref;

	QStringList listLables;
	std::list<basar::VarString> listPropTab;

	initHeaderLables(listLables,listPropTab);
	initTable(listLables);
	initSearchCriteria();
	initMatcher(listPropTab);

	ui.btnSearch->setEnabled(false);
	ui.btnSearch->setDefault(true);
	ui.btnOK->setEnabled(false);

	clear();
}

void ManufacturerFinderVC::clear()
{
	ui.tblManufacturer->clearContents();
	ui.tblManufacturer->setRowCount(0);
	ui.txtManufacturerNo->clear();
	ui.txtMatchCode->clear();
}

basar::gui::tie::WidgetReturnEnum ManufacturerFinderVC::show()
{
	return basar::gui::tie::getWidgetReturnType( exec() );
}

void ManufacturerFinderVC::hide()
{
	QDialog::hide();
}

bool ManufacturerFinderVC::shutdown()
{
	return QDialog::close();
}

void ManufacturerFinderVC::wireEvents()
{
	connect( ui.btnCancel,         SIGNAL( clicked()	                                 ),
             this,			       SLOT  ( on_Close() )                                  );
	connect( ui.btnOK,             SIGNAL( clicked()	                                 ),
             this,			       SLOT  ( on_Ok() )                                     );
	connect( ui.btnSearch,         SIGNAL( clicked()	                                 ),
             this,			       SLOT  ( on_Search() )                                 );

	connect( ui.txtManufacturerNo, SIGNAL( textChanged (const QString &)                 ),
             this,			       SLOT  ( on_TextChangedSupplierNo(const QString &) )   );
	connect( ui.txtMatchCode,      SIGNAL( textChanged (const QString &)	             ),      
             this,			       SLOT  ( on_TextChangedMatchCode(const QString &)  )   );

	connect( ui.tblManufacturer,   SIGNAL( dataSelectionChanged()                        ),
             this,			       SLOT  ( on_SelectionChanged() )                       );
    connect( ui.tblManufacturer,   SIGNAL( doubleClicked( QModelIndex )                  ),
		     this,                 SLOT  ( onTableDoubleClicked( QModelIndex ) )         );
}																	  

void ManufacturerFinderVC::on_Close()
{
    basar::appl::SystemEventManager::getInstance().fire( "DialogCloseRequested" );
}

void ManufacturerFinderVC::on_Ok()
{
	if( !( m_SearchResultMatcher.getSelectedRight().isNull() || m_SearchResultMatcher.getSelectedRight().isEnd() ) )
    {
		basar::db::aspect::AccessorPropertyTable_YIterator yit = m_SearchResultMatcher.getSelectedRight();
		basar::appl::SystemEventManager::getInstance().fire( "NewManufacturerRequested", yit );
	}
}

void ManufacturerFinderVC::on_Search()
{
    CSC_Batches::gui::WaitCursor waitCursor( this );
	basar::appl::SystemEventManager::getInstance().fire( "FindManufacturerRequested" );
}

void ManufacturerFinderVC::on_TextChangedSupplierNo(const QString &newVal)
{
	if( ! newVal.isEmpty() )
	{
		ui.btnSearch->setEnabled(true);
		ui.btnSearch->setDefault(true);
		ui.txtMatchCode->setEnabled(false);
	}
	else
	{
		ui.txtMatchCode->setEnabled(true);
		ui.btnSearch->setEnabled(false);
	}
}

void ManufacturerFinderVC::on_TextChangedMatchCode(const QString &newVal)
{
	if( ! newVal.isEmpty() )
	{
		ui.txtManufacturerNo->setEnabled(false);

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
		ui.txtManufacturerNo->setEnabled(true);
		ui.btnSearch->setEnabled(false);
	}
}

void ManufacturerFinderVC::on_SelectionChanged()
{
	ui.btnOK->setEnabled(true);
}

void ManufacturerFinderVC::onTableDoubleClicked( QModelIndex index )
{
    ui.tblManufacturer->setCurrentCell( index.row(), index.column(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
    basar::appl::SystemEventManager::getInstance().fire( "NewManufacturerRequested", m_SearchResultMatcher.getSelectedRight() );
}

void ManufacturerFinderVC::initSearchCriteria()
{
    ui.txtManufacturerNo->setAccessibleName( libabbauw::properties::MANUFACTURERNO.getName().c_str()         );
    ui.txtMatchCode->setAccessibleName( libabbauw::properties::MATCHCODE.getName().c_str()                   );

	ui.txtManufacturerNo->setValidator( new QRegExpValidator( QRegExp("[0-9]{1,5}" ), ui.txtManufacturerNo ) );
	ui.txtMatchCode->setValidator(  new QRegExpValidator( QRegExp("[a-zA-Z0-9 ]{1,15}" ), ui.txtMatchCode )  );

	ui.txtMatchCode->setFocus();
}

void ManufacturerFinderVC::initTable(QStringList& lstLables)
{
	ui.tblManufacturer->setColumnCount              ( lstLables.size()                     );
    ui.tblManufacturer->setHorizontalHeaderLabels   ( lstLables                            );               
    ui.tblManufacturer->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn                );
    ui.tblManufacturer->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded                );
    ui.tblManufacturer->setSelectionBehavior        ( QAbstractItemView::SelectRows        );
    ui.tblManufacturer->setSelectionMode            ( QAbstractItemView::SingleSelection   );
    ui.tblManufacturer->hideVerticalHeader          ( true                                 );
    
	ui.tblManufacturer->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);	 // SupplierNo
	ui.tblManufacturer->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);	 // Matchcode
	ui.tblManufacturer->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);			 // Supplier
	ui.tblManufacturer->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);	 // Location
	ui.tblManufacturer->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);	 // Postcode
	ui.tblManufacturer->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);	         // Address

	ui.tblManufacturer->setTextAlignmentColumn( 0, Qt::AlignRight   |  Qt::AlignVCenter );		     // SupplierNo
	ui.tblManufacturer->setTextAlignmentColumn( 1, Qt::AlignLeft    |  Qt::AlignVCenter );		     // Matchcode
	ui.tblManufacturer->setTextAlignmentColumn( 2, Qt::AlignLeft    |  Qt::AlignVCenter );		     // Supplier
	ui.tblManufacturer->setTextAlignmentColumn( 3, Qt::AlignLeft    |  Qt::AlignVCenter );	         // Location
	ui.tblManufacturer->setTextAlignmentColumn( 4, Qt::AlignRight   |  Qt::AlignVCenter );		     // Postcode
	ui.tblManufacturer->setTextAlignmentColumn( 5, Qt::AlignLeft    |  Qt::AlignVCenter );		     // Address

    for( int i = 0; i < lstLables.size(); ++i )
    {
		ui.tblManufacturer->setEditableColumn( i , false );
    }
}

void ManufacturerFinderVC::initHeaderLables(QStringList& lstLables, std::list<basar::VarString>& lstOriginalAttributes)
{
	 METHODNAME_DEF( ManufacturerFinderVC, initHeaderLables )
    BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

	using namespace libabbauw::properties;

    lstLables.push_back            ( tr("SupplierNo")           );
    lstOriginalAttributes.push_back( MANUFACTURERNO.getName()   );

	lstLables.push_back            ( tr("Matchcode")            );
    lstOriginalAttributes.push_back( MATCHCODE.getName()        );

	lstLables.push_back            ( tr("Supplier")             );
    lstOriginalAttributes.push_back( MANUFACTURERNAME.getName() );

    lstLables.push_back            ( tr("Location")             );
    lstOriginalAttributes.push_back( LOCATION.getName()         );

	lstLables.push_back            ( tr("Postcode")             );
    lstOriginalAttributes.push_back( POSTCODE.getName()         );

	lstLables.push_back            ( tr("Address")              );
    lstOriginalAttributes.push_back( ADDRESS.getName()          );
}

void ManufacturerFinderVC::initMatcher(std::list<basar::VarString>& listPropTable)
{
	m_SearchResultMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.tblManufacturer, m_Results);
    m_SearchResultMatcher.transformProperties( ui.tblManufacturer->horizontalHeaderLabels(), listPropTable );
}

void ManufacturerFinderVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
{
    basar::gui::tie::MatcherRef matcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpSearch, 
                                                                                                 yitSearch.getPropertyTable() );
	matcher.LeftToRight( );
}

void ManufacturerFinderVC::matchSearchResults()
{
	libutil::gui::SignalBlocker signalblocker( ui.tblManufacturer );

	m_SearchResultMatcher.getRight().size();
	ui.tblManufacturer->setRowCount( static_cast<int>(m_Results.size()) );
	ui.tblManufacturer->resizeRowsToContents();

	ui.btnOK->setEnabled(false);
	ui.tblManufacturer->clearSelection();

    const basar::Int32 matchedHits = m_SearchResultMatcher.RightToLeft( 0, ui.tblManufacturer->rowCount() );
	if( matchedHits == 0 )
    { 
        //info( tr("No searchresults. Either you didn't enter any searchcriteria or for this criteria there are no entries.").toLocal8Bit().constData() );
	}
    else
    {	
		ui.btnOK->setEnabled(true);
		ui.btnOK->setDefault(true);

		ui.tblManufacturer->setRowCount( static_cast<int>(m_Results.size()) );
        ui.tblManufacturer->resizeRowsToContents();
		ui.tblManufacturer->selectRow(0);
		ui.tblManufacturer->setFocus();
    }
}

void ManufacturerFinderVC::initTitle()
{
    QString windowTitle;
	windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_MANUFACTURERFINDER ).append( DLG_TITLESHORT2 );
	windowTitle.append( tr( "Manufacturer Search" ) );
	windowTitle.append( tr( " area: " ) );
	basar::I18nString areaName = basar::login::Manager::getInstance().getAreaName();
	windowTitle.append( QString::fromLocal8Bit( areaName.c_str() ) );
	setWindowTitle( windowTitle );
}

void ManufacturerFinderVC::info( const basar::I18nString& text )
{
	basar::gui::tie::infoMsgBox( this, text.c_str(), tr("Info").toLocal8Bit().constData() );
}

void ManufacturerFinderVC::error( const basar::I18nString& text )
{
	basar::gui::tie::criticalMsgBox( this, text.c_str(), tr("Error").toLocal8Bit().constData() );
}

const basar::gui::tie::MessageBoxButtonEnum ManufacturerFinderVC::question( const basar::I18nString& text )
{
	return basar::gui::tie::questionMsgBox( this, text.c_str(), tr("Question").toLocal8Bit().constData() );
}
}
}
