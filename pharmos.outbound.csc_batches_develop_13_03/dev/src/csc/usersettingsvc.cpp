//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmnvc.h"
#include "usersettingsvc.h"

#include "libabbauw/iparameteredit.h"
#include "iuserrights.h"
#include "definitions_gui.h"

#include <libbasarlogin_manager.h>

#include "tablewidgetsorter.h"

#include "libabbauw/loggerpool/libabbauw_loggerpool.h"
#include "libabbauw/libabbauw_properties_definitions.h"

namespace CSC_Batches
{
namespace viewConn
{

UserSettingsVC::UserSettingsVC( QWidget *parent )
	: QDialog(parent), m_Logger(  libabbauw::LoggerPool::loggerViewConn ), m_EventsWiring ( "UserSettingsVC::EventsWiring" )
{
	ui.setupUi( this );
	wireEvents();
	labelWindowTitle();
	setupList();
	setupDateWidgets();
	setupLineEdits();
}

UserSettingsVC::~UserSettingsVC()
{
}

void UserSettingsVC::injectParameter( libabbauw::domMod::parameter::IParameterEditPtr param )
{
    m_ParameterEditPtr = param;
}

void UserSettingsVC::injectUserRights( infrastructure::rights::IUserRightsPtr userRights )
{
    m_UserRights = userRights;
}

void UserSettingsVC::setupPermissions()
{
    const bool isAllowed = m_UserRights->isUserAllowedToChangePrivilegedSettings();

    ui.chkNotSalesEffectiveFactor->setEnabled( ui.chkNotSalesEffectiveFactor->isEnabled() && isAllowed );
    ui.txtReserveQtyRelease->setEnabled( ui.txtReserveQtyRelease->isEnabled() && isAllowed );
    ui.txtReserveQtyOrder->setEnabled( ui.txtReserveQtyOrder->isEnabled() && isAllowed );
	ui.txtResQtyNotSalesEffectiveRelease->setEnabled( ui.txtResQtyNotSalesEffectiveRelease->isEnabled() && isAllowed );
	ui.txtResQtyNotSalesEffectiveOrder->setEnabled( ui.txtResQtyNotSalesEffectiveOrder->isEnabled() && isAllowed );
}

void UserSettingsVC::wireEvents()
{
    if( m_EventsWiring.isSet() ){ return; }
    
	QObject::connect( ui.btnSaveCancel,                       SIGNAL( clicked( QAbstractButton* )                ),
                      this,                                   SLOT  ( onSaveCancelBtnClicked(QAbstractButton*) ) );
	QObject::connect( ui.btnMoveRight,                        SIGNAL( clicked()                                  ),
                      this,                                   SLOT  ( onBtnMoveRightClicked() )                  );
	QObject::connect( ui.btnMoveLeft,                         SIGNAL( clicked()                                  ),
                      this,                                   SLOT  ( onBtnMoveLeftClicked() )                   );
	QObject::connect( ui.btnMoveUp,                           SIGNAL( clicked()                                  ),
                      this,                                   SLOT  ( onBtnMoveUpClicked() )                     );
	QObject::connect( ui.btnMoveDown,                         SIGNAL( clicked()                                  ),
                      this,                                   SLOT  ( onBtnMoveDownClicked() )                   );
	QObject::connect( ui.rdAscending,                         SIGNAL( toggled( bool )                            ),
		              this,                                   SLOT  ( onRadioBtnOrderClicked() )                 );
    QObject::connect( ui.rdDescending,                        SIGNAL( toggled( bool )                            ),
		              this,                                   SLOT  ( onRadioBtnOrderClicked() )                 );
	QObject::connect( ui.lstActiveSort,                       SIGNAL( itemSelectionChanged()                     ),
		              this,                                   SLOT  ( onLstItemSelectionChanged() )              );
    QObject::connect( ui.chkNotSalesEffectiveFactor,          SIGNAL( toggled(bool)                              ),
					  ui.txtResQtyNotSalesEffectiveRelease,   SLOT  ( setEnabled(bool) )                         );
	QObject::connect( ui.chkNotSalesEffectiveFactor,          SIGNAL( toggled(bool)                              ),
					  ui.txtResQtyNotSalesEffectiveOrder,	  SLOT  ( setEnabled(bool) )                         );

    m_EventsWiring.set();
}

void UserSettingsVC::labelWindowTitle()
{
	QString windowTitle;
	windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_USERSETTINGS ).append( DLG_TITLESHORT2 );
	windowTitle.append( tr( "Settings" ) );
	windowTitle.append( tr( " area: " ) );
	basar::I18nString areaName = basar::login::Manager::getInstance().getAreaName();
	windowTitle.append( QString::fromLocal8Bit( areaName.c_str() ) );
	setWindowTitle( windowTitle );
}

void UserSettingsVC::setupList()
{
	m_StringToProperty[ tr("MNo")           ] = libabbauw::properties::MANUFACTURERNO;
    m_StringToProperty[ tr("Pharmacy No")   ] = libabbauw::properties::PHARMACYNO;
	m_StringToProperty[ tr("Order No")      ] = libabbauw::properties::ORDERNO;
	m_StringToProperty[ tr("Status")        ] = libabbauw::properties::STATUS;
	m_StringToProperty[ tr("Order Date")    ] = libabbauw::properties::ORDERDATE;
	m_StringToProperty[ tr("Delivery Date") ] = libabbauw::properties::EDIDELIVERYDATE;
	m_StringToProperty[ tr("Order Value")   ] = libabbauw::properties::ORDERVALUE;

	for( std::map< QString, basar::property::PropertyDescription >::const_iterator i = m_StringToProperty.begin(); i != m_StringToProperty.end(); ++i )
	{
		ui.lstSort->addItem( new QListWidgetItem( i->first ) );
	}
}

void UserSettingsVC::setupDateWidgets()
{
	QDate minDate( 2000,  1,  1 ), maxDate( 2099, 12, 31 );
	
	ui.dtStart->setMinimumDate( minDate );
	ui.dtEnd->setMinimumDate(   minDate );

	ui.dtStart->setMaximumDate( maxDate );
	ui.dtEnd->setMaximumDate(   maxDate );
}

void UserSettingsVC::setupLineEdits()
{
    ui.txtDaysToColor->setValidator( new QIntValidator( 0, 999, ui.txtDaysToColor ) );
	ui.txtReserveQtyRelease->setValidator( new QDoubleValidator( 0.0, 99999.0, 2, ui.txtReserveQtyRelease ) );
    ui.txtReserveQtyOrder->setValidator( new QDoubleValidator( 0.0, 99999.0, 2, ui.txtReserveQtyOrder ) );
    ui.txtResQtyNotSalesEffectiveRelease->setValidator( new QDoubleValidator( 0.0, 99999.0, 2, ui.txtResQtyNotSalesEffectiveRelease ) );
	ui.txtResQtyNotSalesEffectiveOrder->setValidator( new QDoubleValidator( 0.0, 99999.0, 2, ui.txtResQtyNotSalesEffectiveOrder ) );
	ui.txtLSOLimit->setValidator( new QIntValidator( 0, 99999, ui.txtLSOLimit ) );
	ui.txtExportSeparator->setValidator( new QRegExpValidator( QRegExp( "[-/\\:|+*.,;#]{1}" ),  ui.txtExportSeparator ) );
}

QIcon UserSettingsVC::drawSortIcon( util::SortOrder::Order order )
{
    return util::TableWidgetSorter::drawSortIcon( order );
}

basar::gui::tie::WidgetReturnEnum UserSettingsVC::show()
{ 
    QWidget::setWindowModality( Qt::ApplicationModal );
    QDialog::show(); 
    return basar::gui::tie::WidgetReturnEnum(); 
}

void UserSettingsVC::matchFromOrderSort()
{
	m_OrderSort = m_ParameterEditPtr->getOrderSort();

	// processes order sort and put matching sortable columns in right list
	for( std::size_t i = 0; i < m_OrderSort.size(); ++i )
	{
		const basar::property::PropertyDescription& prop = m_OrderSort.propertyByIndex( i );
		
		// find matching list entry string
		StringToProperties::const_iterator it;
		for( it = m_StringToProperty.begin(); it != m_StringToProperty.end(); ++it )
		{
			if( it->second.getName() == prop.getName() )
				break;
		}

		// list entry string found?
		if( it != m_StringToProperty.end() )
		{
			// search for list entry matching list entry string
			for( int ict = 0; ict < ui.lstSort->count(); ++ict )
			{
				if( it->first == ui.lstSort->item( ict )->text() )
				{
					// move list entry to right list
					QListWidgetItem* item = ui.lstSort->takeItem( ict );
					item->setIcon( drawSortIcon( m_OrderSort.getOrder( prop ) ) );
					ui.lstActiveSort->insertItem( ui.lstActiveSort->count(), item );
					ui.lstActiveSort->setCurrentRow( ui.lstActiveSort->count() - 1 );
				}
			}
		}
	}
}

void UserSettingsVC::matchFrom()
{
	matchFromOrderSort();

	ui.txtDaysToColor->setText(     QString::number( m_ParameterEditPtr->getDaysToColor() ) );
	ui.txtReserveQtyRelease->setText( m_ParameterEditPtr->getReserveQuantityReleaseFactor().toString().c_str() );
    ui.txtReserveQtyOrder->setText  ( m_ParameterEditPtr->getReserveQuantityOrderFactor().toString().c_str() );

    ui.chkNotSalesEffectiveFactor->setChecked( true ); // toggle to emit signal

	if( m_ParameterEditPtr->isNotSalesEffectiveReserveCheckSet() )
    {
			basar::Decimal rQtyFactorRel = m_ParameterEditPtr->getNotSalesEffectiveReserveQtyReleaseFactor();
			ui.txtResQtyNotSalesEffectiveRelease->setText( rQtyFactorRel.toString().c_str() );

			basar::Decimal rQtyFactorOrd = m_ParameterEditPtr->getNotSalesEffectiveReserveQtyOrderFactor();
			ui.txtResQtyNotSalesEffectiveOrder->setText( rQtyFactorOrd.toString().c_str() );
	}
    else
    {
        ui.chkNotSalesEffectiveFactor->setChecked( false );
    }
	
	basar::Date startDate = m_ParameterEditPtr->getFilterStartDate();
	ui.dtStart->setDate( QDate( startDate.getYear(), startDate.getMonth(), startDate.getDay() ) );
	
	basar::Date endDate = m_ParameterEditPtr->getFilterEndDate();
	ui.dtEnd->setDate( QDate( endDate.getYear(), endDate.getMonth(), endDate.getDay() ) );

    ui.txtLSOLimit->setText( QString::number( m_ParameterEditPtr->getLargeScaleOrderLimit() ) );
    
	ui.txtExportSeparator->setText( QString::fromStdString(m_ParameterEditPtr->getCSVExportSeparator()) );

    setupPermissions();
}

void UserSettingsVC::matchTo()
{
	m_ParameterEditPtr->setOrderSort( m_OrderSort );

	m_ParameterEditPtr->setDaysToColor( ui.txtDaysToColor->text().toInt() );

	basar::Decimal relFactor( ui.txtReserveQtyRelease->text().toLocal8Bit().constData() );
	m_ParameterEditPtr->setReserveQuantityReleaseFactor( relFactor );

    basar::Decimal orderFactor( ui.txtReserveQtyOrder->text().toLocal8Bit().constData() );
	m_ParameterEditPtr->setReserveQuantityOrderFactor( orderFactor );

	m_ParameterEditPtr->setNotSalesEffectiveReserveCheck(ui.chkNotSalesEffectiveFactor->isChecked());

    if( ui.chkNotSalesEffectiveFactor->isChecked())
    {
		if(! ui.txtResQtyNotSalesEffectiveRelease->text().isEmpty())
		{
			basar::Decimal d2( ui.txtResQtyNotSalesEffectiveRelease->text().toLocal8Bit().constData() );
			m_ParameterEditPtr->setNotSalesEffectiveReserveQtyReleaseFactor( d2 );
		}
		else
		{
			// set parameter to unset
			m_ParameterEditPtr->setNotSalesEffectiveReserveQtyReleaseFactor( basar::Decimal( 0.0 ) );
		}

		if(!ui.txtResQtyNotSalesEffectiveOrder->text().isEmpty())
		{
			basar::Decimal d2( ui.txtResQtyNotSalesEffectiveOrder->text().toLocal8Bit().constData() );
			m_ParameterEditPtr->setNotSalesEffectiveReserveQtyOrderFactor( d2 );
		}
		else
		{
			// set parameter to unset
			m_ParameterEditPtr->setNotSalesEffectiveReserveQtyOrderFactor( basar::Decimal( 0.0 ) );
		}
    }

	QDate date = ui.dtStart->date();
	m_ParameterEditPtr->setFilterStartDate( basar::Date( date.year(), date.month(), date.day() ) );

	date = ui.dtEnd->date();
	m_ParameterEditPtr->setFilterEndDate( basar::Date( date.year(), date.month(), date.day() ) );

    m_ParameterEditPtr->setLargeScaleOrderLimit( ui.txtLSOLimit->text().toInt() );

	m_ParameterEditPtr->setCSVExportSeparator( basar::VarString(ui.txtExportSeparator->text().toStdString()) );
}

void UserSettingsVC::onSaveCancelBtnClicked( QAbstractButton* b )
{
	QDialogButtonBox::StandardButton button = ui.btnSaveCancel->standardButton( b );
	switch( button )
	{
		case QDialogButtonBox::Save:
			basar::appl::SystemEventManager::getInstance().fire( "SaveSettingsRequested" );
			break;
		default:
		case QDialogButtonBox::Cancel:
			close();
	}
}

void UserSettingsVC::onBtnMoveRightClicked()
{
	if( ui.lstSort->currentItem() != NULL )
	{
		libutil::gui::SignalBlocker signalblocker( ui.lstActiveSort );
		
		// move selected item to right list and set selection mark on it
		const int lstSortRow = ui.lstSort->currentRow();
		const int row = ui.lstActiveSort->count();

		QListWidgetItem* item = ui.lstSort->takeItem( lstSortRow );
		item->setIcon( drawSortIcon( util::SortOrder::DESCENDING ) ); // set default icon to descending
		m_OrderSort.setOrder( m_StringToProperty[ item->text() ], util::SortOrder::DESCENDING );
		ui.lstActiveSort->insertItem( row, item );
		
		signalblocker.unblock();	// allow onSelectionChanged signal triggering corresponding slot to update radio button
		ui.lstActiveSort->setCurrentRow( row );
		
		// set selection mark in left list to next item if any
		if( ui.lstSort->count() > lstSortRow )
			ui.lstSort->setCurrentRow( lstSortRow );
	}
}

void UserSettingsVC::onBtnMoveLeftClicked()
{
	if( ui.lstActiveSort->currentItem() != NULL )
	{
		libutil::gui::SignalBlocker signalblocker( ui.lstActiveSort );
		
		// move selected item to left list and set selection mark on it
		int lstActiveSortRow = ui.lstActiveSort->currentRow();
		int row = ui.lstSort->count();
		QListWidgetItem* item = ui.lstActiveSort->takeItem( lstActiveSortRow );
		item->setIcon( QIcon() ); // remove icon
		ui.lstSort->insertItem( row, item );
		ui.lstSort->setCurrentRow( row );

		m_OrderSort.setOrder( m_StringToProperty[ item->text() ], util::SortOrder::NONE );

		// set selection mark in right list to next item if any
		signalblocker.unblock();	// allow onSelectionChanged signal triggering corresponding slot to update radio button
		if( ui.lstActiveSort->count() > lstActiveSortRow )
			ui.lstActiveSort->setCurrentRow( lstActiveSortRow );
	}
}

void UserSettingsVC::onBtnMoveUpClicked()
{
	const int row = ui.lstActiveSort->currentRow();
	if( row > 0 ) // move only if item is not the first one in list
	{
		QListWidgetItem* item = ui.lstActiveSort->takeItem( row );
		ui.lstActiveSort->insertItem( row - 1, item );

		std::string a = m_StringToProperty[ ui.lstActiveSort->currentItem()->text() ];
		std::string b = m_StringToProperty[  item->text() ];
		m_OrderSort.swapProperties( m_StringToProperty[ ui.lstActiveSort->currentItem()->text() ], 
			                        m_StringToProperty[  item->text() ] );
		ui.lstActiveSort->setCurrentRow( row - 1 );
	}
}

void UserSettingsVC::onBtnMoveDownClicked()
{
	const int row = ui.lstActiveSort->currentRow();
	if( row < ui.lstActiveSort->count() - 1 )   // move only if item is not the last one in list
	{
		QListWidgetItem* item = ui.lstActiveSort->takeItem( row );
		ui.lstActiveSort->insertItem( row + 1, item );
		m_OrderSort.swapProperties( m_StringToProperty[ ui.lstActiveSort->item( row )->text() ], 
			                        m_StringToProperty[  item->text() ] );
		ui.lstActiveSort->setCurrentRow( row + 1 );
	}
}

void UserSettingsVC::onRadioBtnOrderClicked()
{
	QListWidgetItem* item = ui.lstActiveSort->currentItem();
	if( item != NULL )
	{
		if( ui.rdAscending->isChecked() )
		{
			item->setIcon( drawSortIcon( util::SortOrder::ASCENDING ) );
			m_OrderSort.setOrder( m_StringToProperty[ item->text() ], util::SortOrder::ASCENDING );
		}
		else
		{
			item->setIcon( drawSortIcon( util::SortOrder::DESCENDING ) );
			m_OrderSort.setOrder( m_StringToProperty[ item->text() ], util::SortOrder::DESCENDING );
		}
	}
}

void UserSettingsVC::onLstItemSelectionChanged()
{
	QListWidgetItem* item = ui.lstActiveSort->currentItem();
	if( item != NULL )
	{
		// block click events of both radio buttons
		libutil::gui::SignalBlocker blockRdBtnAsc( ui.rdAscending );
		libutil::gui::SignalBlocker blockRdBtnDesc( ui.rdDescending );

		// sync radio buttons with stored value of selected item
		if( m_OrderSort.getOrder( m_StringToProperty[ item->text() ] ) == util::SortOrder::DESCENDING )
			ui.rdDescending->setChecked( true );
		else
			ui.rdAscending->setChecked( true );
	}
}

bool UserSettingsVC::shutdown()
{
    return close();
}

void UserSettingsVC::waitForEvents()
{
    setAttribute( Qt::WA_Moved, false );
    exec();
}

void UserSettingsVC::info( const basar::I18nString& text )
{
	basar::gui::tie::infoMsgBox( this, text.c_str(), tr( "User Settings" ).toLocal8Bit().data() );
}

void UserSettingsVC::error( const basar::I18nString& text)
{
	basar::gui::tie::criticalMsgBox( this, text.c_str(), tr( "User Settings" ).toLocal8Bit().data() );
}

const basar::gui::tie::MessageBoxButtonEnum UserSettingsVC::question( const basar::I18nString& text)
{
	return basar::gui::tie::questionMsgBox( this, text.c_str(), tr( "User Settings" ).toLocal8Bit().data() );
}

} // end namespace viewConn
} // end namespace CSC_Batches
