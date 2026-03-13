#include "tenderimportvc.h"

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4251 4311 4312 4481 4512 4800)
	#include <QFileDialog>
#pragma warning (pop)

namespace tender 
{
namespace viewConn
{
TenderImportVC::TenderImportVC( QWidget * parent )
: BaseVC( parent )
{
	ui.setupUi( this );

	m_IconPath = QCoreApplication::applicationDirPath() + "/icons/";
	ui.lblProgress->setText( "" );
	ui.progressBar->setValue( 0 );
	ui.btnStart->setDisabled( true );

	connect( ui.btnSelectPositionsFile, SIGNAL( clicked() ),
	this,								SLOT  ( OnSelectPositionsFileClicked() ) );
	connect( ui.btnSelectDeliveriesFile, SIGNAL( clicked() ),
	this,								 SLOT  ( OnSelectDeliveriesFileClicked() ) );
	connect( ui.btnStart, SIGNAL( clicked() ),
	this,				  SLOT  ( OnStartClicked() ) );
	connect( ui.btnClose, SIGNAL( clicked() ),
	this,				  SLOT  ( OnCloseClicked() ) );
	connect( ui.btnPositionsHelp, SIGNAL( clicked() ),
	this,						  SLOT  ( OnPositionsHelpClicked() ) );
	connect( ui.btnScheduledDeliveryHelp, SIGNAL( clicked() ),
	this,								  SLOT  ( OnScheduledDeliveryHelpClicked() ) );

	
	
}

TenderImportVC::~TenderImportVC()
{
}

void TenderImportVC::setIconPath( const basar::I18nString & path )
{
	m_IconPath = QString::fromLocal8Bit( path.c_str() );
}

void TenderImportVC::matchToPositionsFilename( basar::VarString & filename ) const
{
	filename = ui.lePositionsFile->text().toLocal8Bit().constData();
}

void TenderImportVC::matchToDeliveriesFilename( basar::VarString & filename ) const
{
	filename = ui.leDeliveriesFile->text().toLocal8Bit().constData();
}

void TenderImportVC::resetProgress()
{
	ui.lwProgress->clear();
}

void TenderImportVC::setProgressDescription( const basar::I18nString & description )
{
	ui.lblProgress->setText( QString::fromLocal8Bit( description.c_str() ) );
	addInfoMessage( description );
}

void TenderImportVC::progress( const basar::Int32 percent )
{
	if(    0 > percent
		|| 100 < percent )
	{
		return;
	}

	ui.progressBar->setValue( percent );
	this->update();
	QCoreApplication::sendPostedEvents();
}

void TenderImportVC::addMessage( const basar::I18nString & msg )
{
	addLogMessage( LG_NONE, QString::fromLocal8Bit( msg.c_str() ) );
}

void TenderImportVC::addInfoMessage( const basar::I18nString & msg )
{
	addLogMessage( LG_INFORMATION, QString::fromLocal8Bit( msg.c_str() ) );
}

void TenderImportVC::addWarningMessage( const basar::I18nString & msg )
{
	addLogMessage( LG_WARNING, QString::fromLocal8Bit( msg.c_str() ) );
}

void TenderImportVC::addErrorMessage( const basar::I18nString & msg )
{
	addLogMessage( LG_CRITICAL, QString::fromLocal8Bit( msg.c_str() ) );
}

void TenderImportVC::addLogMessage( const LogGroupEnum group, const QString & message )
{
	if( message.isEmpty() )
	{
		return;
	}

	QListWidgetItem * item = new QListWidgetItem( message );
	item->setData( 100, group );

	switch( group )
	{
		case LG_CRITICAL:
		{
			item->setIcon( QIcon( m_IconPath + ( "critical.png" ) ) );
			break;
		}

		case LG_WARNING:
		{
			item->setIcon( QIcon( m_IconPath + ( "warning.png" ) ) );
			break;
		}

		case LG_INFORMATION:
		{
			item->setIcon( QIcon( m_IconPath + ( "information.png" ) ) );
			break;
		}

		case LG_NONE:
		default:
		{
			// nothing to do
			break;
		}
	}

	ui.lwProgress->addItem( item );
	ui.lwProgress->scrollToItem( item );
}

void TenderImportVC::OnSelectPositionsFileClicked()
{
	static QString title( QCoreApplication::translate( "tender::viewConn::TenderImportVC", "Open Tender Positions Import File" ) );

	QString fileName = getFilename( title );
	ui.lePositionsFile->setText( fileName );
	ui.btnStart->setDisabled( fileName.isEmpty() );
}

void TenderImportVC::OnSelectDeliveriesFileClicked()
{
	static QString title( QCoreApplication::translate( "tender::viewConn::TenderImportVC", "Open Tender Deliveries Import File" ) );

	QString fileName = getFilename( title );
	ui.leDeliveriesFile->setText( fileName );
}

const QString TenderImportVC::getFilename( const QString & title )
{
	QString fileName = QFileDialog::getOpenFileName( this, title, "", tr( "Tender CSV (*.csv *.txt)" ) );

	QFile file( fileName );
	if( fileName.isEmpty() )
	{
		return "";
	}
	if( false == file.open( QFile::ReadOnly | QFile::Text ) )
	{
		basar::I18nString msg( QCoreApplication::translate( "tender::viewConn::TenderImportVC", "Could not open file:" ).toLocal8Bit().constData() );
		msg.append( "\n" );
		msg.append( fileName.toLocal8Bit().constData() );
		showErrorMessage( msg, title.toLocal8Bit().constData() );
		return "";
	}

	file.close();
	return fileName;
}

void TenderImportVC::OnStartClicked()
{
	libutil::gui::WaitCursor bc( this );
	basar::appl::SystemEventManager::getInstance().fire( "StartImportRequested" );
}

void TenderImportVC::OnCloseClicked()
{
	reject();
}

void TenderImportVC::OnPositionsHelpClicked()
{
	TenderImportPositionsHelpVC vc( this ); 
	vc.exec();
}

void TenderImportVC::OnScheduledDeliveryHelpClicked()
{
	TenderImportScheduledDeliveryHelpVC vc( this );
	vc.exec();
}

} // end namespace viewConn
} // end namespace tender 
