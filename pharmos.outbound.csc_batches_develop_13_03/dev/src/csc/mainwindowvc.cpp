//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "mainwindowvc.h"

#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libbasarqtwidget_functions.h>

#include <libutil/viewconn.h>
#include <libutil/util.h>
#include <libutil/dbconnection.h>

#include "waitcursor.h"
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QGridlayout>

#include "iuserrights.h"
#include "definitions_gui.h"
#include "libcscbatch/csc_batches_version.h"
//#include "gitversioninfo.h"

namespace CSC_Batches 
{
namespace viewConn 
{
	MainWindowVC::MainWindowVC( const libutil::infrastructure::session::ISessionPtr session, const basar::I18nString& dbServerName, 
                                const basar::I18nString& dbName, infrastructure::rights::IUserRightsPtr userRights, QWidget* parent )
        : QMainWindow( parent )
    {
        METHODNAME_DEF( MainWindowVC, MainWindowVC )
		BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

        m_UserRights = userRights;
        init( session );

        configStatusLine( dbServerName, dbName, session );
		configMainLayout( CSC_Batches::APPLICATION, CSC_BATCHES_VERSION );
	}

	MainWindowVC::~MainWindowVC()
    {
        METHODNAME_DEF( MainWindowVC, ~MainWindowVC )
		BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );
		
        m_Statusline.clear();
        MainWindowVC::close();
	}

	void MainWindowVC::init( const libutil::infrastructure::session::ISessionPtr session )
    {
        METHODNAME_DEF( MainWindowVC, init )
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );
		
        createMenus();
        //configStatusLine( session );
        //configMainLayout( session );

        labelWindowTitle( session );
	}

    void MainWindowVC::labelWindowTitle( const libutil::infrastructure::session::ISessionPtr session )
    {
        QString windowTitle;
	    windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_MAIN ).append( DLG_TITLESHORT2 );
	    windowTitle.append( tr( "CSC-Batches" ) );
	    windowTitle.append( tr( " area: " ) );
        windowTitle.append( QString::fromLocal8Bit( session->getUser()->getAreaName().c_str() ) );
	    setWindowTitle( windowTitle );
    }

	void MainWindowVC::createMenus()
    {
        METHODNAME_DEF( MainWindowVC, createMenus )
		BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

		QMenu* menuFile = menuBar()->addMenu( tr("&Batchlist") );
        {
            QAction* ActBatchlistOpen = new QAction(tr("Manage &UW batch orders ..."), this);
		    ActBatchlistOpen->setShortcut(tr("Ctrl+U"));
		    //ActBatchlistOpen->setStatusTip(tr(""));
		    connect( ActBatchlistOpen, SIGNAL( triggered() ), this, SLOT( onBatchOrderEditionStart() ) );
            m_ActionList.push_back( ActBatchlistOpen );
            menuFile->addAction( ActBatchlistOpen );

            QAction* ActCreateBatch = new QAction(tr("&Enter new UW orders ..."), this);
            ActCreateBatch->setShortcut(tr("Ctrl+E"));
            connect( ActCreateBatch, SIGNAL( triggered() ), this, SLOT( onCreateTransferOrdersStart() ) );
            m_ActionList.push_back( ActCreateBatch );
            menuFile->addAction( ActCreateBatch );
            
            menuFile->addSeparator();

            QAction* ActMaintainBatchOrder = new QAction( tr("Manage &SH/CL batch orders ..."), this );
		    ActMaintainBatchOrder->setShortcut(tr("Ctrl+S"));
		    //ActBatchOrder->setStatusTip(tr(""));
		    connect( ActMaintainBatchOrder, SIGNAL( triggered() ), this, SLOT( onBatchOrderMaintenanceStart() ) );
            m_ActionList.push_back( ActMaintainBatchOrder );
            menuFile->addAction( ActMaintainBatchOrder );

            menuFile->addSeparator();

            QAction* ActFileExit = new QAction(tr("E&xit"), this);
		    ActFileExit->setShortcut(tr("F12"));
		    ActFileExit->setStatusTip(tr("Exit the application"));
		    connect( ActFileExit, SIGNAL( triggered()), this, SLOT( close() ) );
            m_ActionList.push_back( ActFileExit );
            menuFile->addAction( ActFileExit );
        }

        QMenu* menuOptions = menuBar()->addMenu( tr("&Options") );
        {
            QAction* ActSettings = new QAction( tr("&Settings"), this );
		    ActSettings->setStatusTip( tr("Change settings") );
		    connect( ActSettings, SIGNAL( triggered() ), this, SLOT( onSettingsStart() ) );
            m_ActionList.push_back( ActSettings );
            menuOptions->addAction( ActSettings );
        }

		QMenu* menuHelp = menuBar()->addMenu( tr("&?") );
        {
            QAction* ActHelpAbout = new QAction(tr("&About"), this);
		    ActHelpAbout->setStatusTip( tr("Show the application's About box") );
		    connect(ActHelpAbout, SIGNAL( triggered() ), this, SLOT( onAboutDlg() ) );
            m_ActionList.push_back( ActHelpAbout );
            menuHelp->addAction( ActHelpAbout );

            if (m_UserRights->isUserAllowedToAccessDeveloperInfo())
            {
                menuFile->addSeparator();

                QAction* ActHelpDevInfo = new QAction(tr("&Developer Info"), this);
		        ActHelpDevInfo->setStatusTip( tr("Show the application's Developer Info") );
		        connect(ActHelpDevInfo, SIGNAL( triggered() ), this, SLOT( onDeveloperInfo() ) );
                m_ActionList.push_back( ActHelpDevInfo );
                menuHelp->addAction( ActHelpDevInfo );
            }
        }
	}

    void MainWindowVC::configStatusLine( const basar::I18nString& dbServerName, const basar::I18nString& dbName, 
                                         const libutil::infrastructure::session::ISessionPtr session )
    {
        METHODNAME_DEF( MainWindowVC, configStatusLine );
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

        m_Statusline.init( this );
        basar::I18nString dbServerAndDBName ( dbServerName );
        dbServerAndDBName.append(" - ");
        dbServerAndDBName.append( dbName );
        m_Statusline.setConnection( dbServerAndDBName.c_str() );
        m_Statusline.setUser( session->getUser()->getUserName() );
    }

    void MainWindowVC::configMainLayout( const basar::I18nString& programName, const basar::I18nString& version )
    {
        METHODNAME_DEF( MainWindowVC, configMainLayout );
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

        m_Background.init( programName, version, basar::gui::qt::MainBackground::OUTBOUND, basar::gui::qt::MainBackground::PHOENIXGROUP );
        QGridLayout* mainLayout = new QGridLayout;
        mainLayout->addWidget( &m_Background );
        
        QWidget* centWidget = new QWidget();
        centWidget->setLayout( mainLayout );
        setCentralWidget( centWidget );
    }

    /*
    void MainWindowVC::configStatusLine( const libutil::infrastructure::session::ISessionPtr session )
    {
        METHODNAME_DEF( MainWindowVC, configStatusLine );
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

        m_Statusline.init( this );
        basar::I18nString dbServerAndDBName ( session->getDBInfoGetter()->getDBServerInfo().getDBServerName() );
        dbServerAndDBName.append(" - ");
        dbServerAndDBName.append( session->getDBInfoGetter()->getDBServerInfo().getDBName() );
        m_Statusline.setConnection( dbServerAndDBName.c_str() );
        m_Statusline.setUser( session->getUser()->getUserName() );
    }

    void MainWindowVC::configMainLayout( const libutil::infrastructure::session::ISessionPtr session )
    {
        METHODNAME_DEF( MainWindowVC, configMainLayout );
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

        m_Background.init( session->getClassInfo()->getClassName(), session->getClassInfo()->getVersion().getFullVersionNumber(),
                           basar::gui::qt::MainBackground::PHOENIXGROUP );
        QGridLayout* mainLayout = new QGridLayout;
        mainLayout->addWidget( &m_Background );
        
        QWidget* centWidget = new QWidget();
        centWidget->setLayout( mainLayout );
        setCentralWidget( centWidget );
    }
    */

    void MainWindowVC::show()
    {
        METHODNAME_DEF( MainWindowVC, show )
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

        QWidget::showMaximized();
    }

    void MainWindowVC::hide()
    {
        METHODNAME_DEF( MainWindowVC, hide )
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

        QWidget::hide();
    }

    void MainWindowVC::onAboutDlg()
    {
        METHODNAME_DEF( MainWindowVC, onAboutDlg )
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

        basar::appl::SystemEventManager::getInstance().fire( "startAboutDlg" );
    }

    void MainWindowVC::showSettingSetToDefaultNotification()
    {
        QString msg = tr( "All wrong or non-existing settings have been set to default!" );
        basar::gui::tie::infoMsgBox( this, msg.toLocal8Bit().constData(), tr( "Info" ).toLocal8Bit().data() );
    }

    void MainWindowVC::showMsg( const basar::VarString& msg )
    {
        basar::gui::tie::infoMsgBox( this, msg.c_str(), tr( "Info" ).toLocal8Bit().data() );
    }

        void MainWindowVC::onBatchOrderMaintenanceStart()
    {
        METHODNAME_DEF( MainWindowVC, onBatchOrderMaintenanceStart );
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );
    
        CSC_Batches::gui::WaitCursor waitcursor( this );
        basar::appl::SystemEventManager::getInstance().fire( "startBatchOrderMaintenanceRequested" );
    }

    void MainWindowVC::onCreateTransferOrdersStart()
    {
        METHODNAME_DEF( MainWindowVC, onCreateTransferOrdersStart );
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );
    
        CSC_Batches::gui::WaitCursor waitcursor( this );
        basar::appl::SystemEventManager::getInstance().fire( "createTransferOrderRequested" );
    }

	void MainWindowVC::onBatchOrderEditionStart() 
    {
        METHODNAME_DEF( MainWindowVC, onBatchOrderEditionStart );
		BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );
    
        CSC_Batches::gui::WaitCursor waitcursor( this );
        basar::appl::SystemEventManager::getInstance().fire( "editBatchOrderRequested" );
    }

	void MainWindowVC::onSettingsStart()
	{
        METHODNAME_DEF( MainWindowVC, onSettingsStart );
		BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

        CSC_Batches::gui::WaitCursor waitcursor( this );
		basar::appl::SystemEventManager::getInstance().fire( "startSettingsRequested" );
	}
extern "C"
{
	extern const char versioninfo[];
	extern const char BASAR_versioninfo[];
}
    void MainWindowVC::onDeveloperInfo()
    {
        METHODNAME_DEF( MainWindowVC, onDeveloperInfo );
		BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );
        std::stringstream s;
        s << versioninfo << std::endl;
        s << std::endl;
		s << BASAR_versioninfo << std::endl;
        s << std::endl;
        /*
        s << "Git Information:" << std::endl;
        s << GitVersionInfo::instance();
        */
        showMsg(s.str());
    }


  } // viewConn
} // CSC_Batches
