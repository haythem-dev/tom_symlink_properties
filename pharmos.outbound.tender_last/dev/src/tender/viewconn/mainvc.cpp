//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "mainvc.h"
#include "tenderoverviewvc.h"
#include "tenderoverviewvcptr.h"

#include "loggerpool/tenderloggerpool.h"
#include "tender_definitions.h"
#include "tender_version.h"
/*
#ifndef WIN32
#include "gitversioninfo.h"
#endif
*/
#include <libutil/viewconn.h>
#include <libutil/util.h>

#include <libbasarlogin.h>
#include <libbasarcmnutil.h>
#include <libbasardbsql.h>
#include <libbasarproperty.h>
#include <libbasarqtwidget.h>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QtWidgets/qgridlayout.h>
#pragma warning (pop)

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
    //-------------------------------------------------------------------------------------------------//
    /*!    \throw  no-throw */
    MainVC::MainVC( const basar::I18nString& dbServerName, const basar::I18nString& dbName,
                    const basar::I18nString& userName, const basar::I18nString& dokuLink, QWidget* parent )
    : QMainWindow( parent ), m_Logger( libtender::LoggerPool::getLoggerViewConn() )
    {
        METHODNAME_DEF( MainVC, MainVC )
        BLOG_TRACE_METHOD( m_Logger, fun );

        ui.setupUi(this);

        createMenus();

        configStatusLine( dbServerName, dbName, userName );
        configMainLayout();
        configAboutMenu( dbServerName, dbName, dokuLink );
    }

    void MainVC::createMenus()
    {
        /////////////////////////////////////////////////////////////////////////////
        // 1. menu point
        /////////////////////////////////////////////////////////////////////////////
        QMenu* tenderMenu = this->menuBar()->addMenu( tr("&Tender") );

        QAction* newAct = new QAction( tr("&New..."), this );
        newAct->setShortcut(QKeySequence::Open);
        newAct->setStatusTip( tr("Create a new tender") );
        this->connect( newAct, SIGNAL(triggered()), this, SLOT(onCreateNewTender()) );
        m_ActionList.push_back( newAct );
        tenderMenu->addAction( newAct );

        QAction* searchAct = new QAction( tr("&Search..."), this );
        searchAct->setShortcut( QKeySequence::Find );
        searchAct->setStatusTip( tr("Search for tenders") );
        this->connect( searchAct, SIGNAL(triggered()), this, SLOT(onSearchTender()) );
        m_ActionList.push_back( searchAct );
        tenderMenu->addAction( searchAct );

        //QAction* importAct = new QAction( tr("&Import ..."), this );
        //importAct->setShortcut( tr("Ctrl+I") );
        //importAct->setStatusTip( tr("Import tenders") );
        //this->connect( importAct, SIGNAL(triggered()), this, SLOT(onImportTender()) );
        //m_ActionList.push_back( importAct );
        //tenderMenu->addAction( importAct );

        //QAction* tempAct = new QAction( "Create TenderOrders", this ); // TODO remove later
        //tempAct->setStatusTip("Create Tender orders according to schedule");
		//this->connect( tempAct, SIGNAL(triggered()), this, SLOT(onCreateTO()) );
        //tenderMenu->addAction( tempAct );

		//QAction* importDeliveriesAct = new QAction( tr("&Import Deliveries..."), this );
		//importDeliveriesAct->setStatusTip( tr("Import deliveries by choosing a ? file.") );
		//this->connect( importDeliveriesAct, SIGNAL(triggered()), this, SLOT(onImportDeliveries()) );
		//m_ActionList.push_back( importDeliveriesAct );
		//tenderMenu->addAction( importDeliveriesAct );

		QAction* exitAct = new QAction( tr("E&xit"), this );
        exitAct->setShortcut( QKeySequence::Close );
        exitAct->setStatusTip( tr("Exit the application") );
        this->connect( exitAct, SIGNAL(triggered()), this, SLOT(close()) );
        m_ActionList.push_back( exitAct );
        tenderMenu->addAction( exitAct );

        /////////////////////////////////////////////////////////////////////////////
        // 2. menu point
        /////////////////////////////////////////////////////////////////////////////
		QMenu* importMenu = this->menuBar()->addMenu( tr("&Import") );

		QAction* importTenderAct = new QAction( tr("&Tender Positions and Scheduled Deliveries..."), this );
        importTenderAct->setStatusTip( tr("Import tender positions by choosing a ? file.") );
        this->connect( importTenderAct, SIGNAL(triggered()), this, SLOT(onImportTender()) );
        m_ActionList.push_back( importTenderAct );
        importMenu->addAction( importTenderAct );


/*
		QMenu* loggingMenu = this->menuBar()->addMenu( tr("&Logging") );

        QAction* logOrderAct = new QAction( tr("&Order ..."), this );
        logOrderAct->setShortcut( tr("Ctrl+O") );
        logOrderAct->setStatusTip( tr("Search for order in logs.") );
        this->connect( logOrderAct, SIGNAL(triggered()), this, SLOT(onSearchOrderInLogs()) );
        m_ActionList.push_back( logOrderAct );
        loggingMenu->addAction( logOrderAct );

        QAction* logDeliveryAct = new QAction( tr("&Delivery ..."), this );
        logDeliveryAct->setShortcut( tr("Ctrl+O") );
        logDeliveryAct->setStatusTip( tr("Search for delivery in logs.") );
        this->connect( logDeliveryAct, SIGNAL(triggered()), this, SLOT(onSearchDeliveryInLogs()) );
        m_ActionList.push_back( logDeliveryAct );
        loggingMenu->addAction( logDeliveryAct );

        QAction* logModificationAct = new QAction( tr("&Modification ..."), this );
        logModificationAct->setShortcut( tr("Ctrl+O") );
        logModificationAct->setStatusTip( tr("Search for modification in logs.") );
        this->connect( logModificationAct, SIGNAL(triggered()), this, SLOT(onSearchModificationInLogs()) );
        m_ActionList.push_back( logModificationAct );
        loggingMenu->addAction( logModificationAct );
*/
        /////////////////////////////////////////////////////////////////////////////
        // 3. menu point
        /////////////////////////////////////////////////////////////////////////////
        QMenu* helpMenu = this->menuBar()->addMenu( tr("&Help") );

        QAction* aboutAct = new QAction( tr("?"), this );
        aboutAct->setShortcut( QKeySequence::HelpContents );
        aboutAct->setStatusTip( tr("Show the application's about dialog") );
        this->connect( aboutAct, SIGNAL(triggered()), this, SLOT(about()) );
        m_ActionList.push_back( aboutAct );
        helpMenu->addAction( aboutAct );

		// TODO FN Userrights kapseln in infrastucture modul.
		if (basar::login::Manager::getInstance().isLegitimated(1, "DEVELOPER_INFO"))
		{
			helpMenu->addSeparator();
			QAction* ActHelpDevInfo = new QAction(tr("&Developer Info"), this);
			ActHelpDevInfo->setStatusTip( tr("Show the application's Developer Info") );
			connect(ActHelpDevInfo, SIGNAL( triggered() ), this, SLOT( onDeveloperInfo() ) );
			m_ActionList.push_back( ActHelpDevInfo );
			helpMenu->addAction( ActHelpDevInfo );
		}

    }

    //-------------------------------------------------------------------------------------------------//
    /*!    \throw  no-throw */
    MainVC::~MainVC()
    {
        METHODNAME_DEF( MainVC, ~MainVC )
        BLOG_TRACE_METHOD( m_Logger, fun );

        m_Statusline.clear();
        MainVC::close();
    }

    void MainVC::configStatusLine( const basar::I18nString& dbServerName, const basar::I18nString& dbName, const basar::I18nString& userName )
    {
        METHODNAME_DEF( MainVC, configStatusLine )
        BLOG_TRACE_METHOD( m_Logger, fun );

        m_Statusline.init( this );
        basar::VarString value( dbServerName );
        value.append(" - ");
        value.append( dbName );
        m_Statusline.setConnection( value );
        m_Statusline.setUser( userName );
    }

    void MainVC::configAboutMenu( const basar::I18nString& dbServerName, const basar::I18nString& dbName, const basar::I18nString& dokuLink )
    {
        METHODNAME_DEF( MainVC, configAboutMenu )
        BLOG_TRACE_METHOD( m_Logger, fun );

        basar::InitStruct initStruct;
        initStruct.database = dbName;
        initStruct.host = dbServerName;
        initStruct.dokulink = dokuLink;
        basar::gui::qt::Manager::getInstance().init( initStruct );
    }

    void MainVC::configMainLayout()
    {
        METHODNAME_DEF( MainVC, configMainLayout )
        BLOG_TRACE_METHOD( m_Logger, fun );

        m_Background.init( tender_APPLICATION_NAME,
                           TENDER_VERSION,
                           basar::gui::qt::MainBackground::OUTBOUND,
                           basar::gui::qt::MainBackground::PHOENIXGROUP );
        QGridLayout* mainLayout = new QGridLayout;
        mainLayout->addWidget( &m_Background );
        ui.centralWidget->setLayout(mainLayout);
    }

    //-------------------------------------------------------------------------------------------------//
    /*!    \throw  no-throw */
    void MainVC::show()
    {
        METHODNAME_DEF( MainVC, show )
        BLOG_TRACE_METHOD( m_Logger, fun );

        QWidget::showMaximized();
    }

    //-------------------------------------------------------------------------------------------------//
    /*!    \throw  no-throw */
    void MainVC::hide()
    {
        METHODNAME_DEF( MainVC, hide )
        BLOG_TRACE_METHOD( m_Logger, fun );

        QWidget::hide();
    }

    //-------------------------------------------------------------------------------------------------//
    /*!    \throw  no-throw */
    void MainVC::about()
    {
        METHODNAME_DEF( MainVC, about )
        BLOG_TRACE_METHOD( m_Logger, fun );

        basar::appl::SystemEventManager::getInstance().fire( "StartAboutDlg" );
    }
    //-------------------------------------------------------------------------------------------------//
    /*!    \throw  no-throw */
    //-------------------------------------------------------------------------------------------------//
    void MainVC::onCreateNewTender()
    {
        METHODNAME_DEF( MainVC, onArticleSearch )
        BLOG_TRACE_METHOD( m_Logger, fun );

        basar::appl::SystemEventManager::getInstance().fire( "StartCreateNewTender" );
    }

    void MainVC::onSearchTender()
    {
        METHODNAME_DEF( MainVC, onSearchTender )
        BLOG_TRACE_METHOD( m_Logger, fun );

        basar::appl::SystemEventManager::getInstance().fire( "StartSearchTender" );
    }

    void MainVC::onImportTender()
    {
        METHODNAME_DEF( MainVC, onImportTender )
        BLOG_TRACE_METHOD( m_Logger, fun );

        basar::appl::SystemEventManager::getInstance().fire( "StartImportTender" );
    }

	void MainVC::onImportDeliveries()
	{
		METHODNAME_DEF( MainVC, onImportDeliveries )
        BLOG_TRACE_METHOD( m_Logger, fun );

		basar::appl::SystemEventManager::getInstance().fire( "StartImportDeliveries" );
	}

	void MainVC::onCreateTO() // TODO remove later
	{
        METHODNAME_DEF( MainVC, onImportTender )
        BLOG_TRACE_METHOD( m_Logger, fun );

		basar::appl::EventReturnStruct ret =
	        basar::appl::SystemEventManager::getInstance().fire( "StartCreateTO" );
		basar::gui::tie::infoMsgBox(this, ret.message.c_str(), "tenderordercreator");
	}

extern "C"
{
	extern const char versioninfo[];
	extern const char BASAR_versioninfo[];
}
    void MainVC::onDeveloperInfo()
    {
        METHODNAME_DEF( MainWindowVC, onDeveloperInfo );
		BLOG_TRACE_METHOD( m_Logger, fun );
        std::stringstream s;
        s << versioninfo << std::endl;
        s << std::endl;
		s << BASAR_versioninfo << std::endl;
        s << std::endl;
        /*
        #ifndef WIN32
        s << "Git Information:" << std::endl;
        s << GitVersionInfo::instance();
        #endif
        */
		basar::gui::tie::infoMsgBox( this, s.str().c_str(), "Info");
    }


	void MainVC::shutdown()
	{
		METHODNAME_DEF( MainVC, shutdown )
        BLOG_TRACE_METHOD( m_Logger, fun );

		m_Statusline.clear();
	}

    //----------------------------------------------------------------------------
    /*!    \throw  no-throw
        \brief static function to initalize listbox of components in about-dialog */
    bool about_define()
    {
        basar::DefinitionStruct def;
        def.copyright = "copyright by PHOENIX GmbH & Co. KG";
        def.programDescription = "tender";
        def.version = ""; //VERSION_NUMBER;
        basar::gui::qt::Manager::getInstance().init(def);

        basar::DllStruct dll;

        dll.name = "libbasarcmnutil.dll";
        dll.pkgVersion = basar::cmnutil::getVersion();
        basar::gui::qt::Manager::getInstance().initDll(dll);

        dll.name = "libbasardbsql.dll";
        dll.pkgVersion = basar::db::sql::getVersion();
        basar::gui::qt::Manager::getInstance().initDll(dll);

        dll.name = "libbasarproperty.dll";
        dll.pkgVersion = basar::property::getVersion();
        basar::gui::qt::Manager::getInstance().initDll(dll);

        dll.name = "libbasardbaspect.dll";
        dll.pkgVersion = basar::db::aspect::getVersion();
        basar::gui::qt::Manager::getInstance().initDll(dll);

        dll.name = "libbasarappl.dll";
        dll.pkgVersion = basar::appl::getVersion();
        basar::gui::qt::Manager::getInstance().initDll(dll);

        dll.name = "libbasarlogin.dll";
        dll.pkgVersion = basar::login::getVersion();
        basar::gui::qt::Manager::getInstance().initDll(dll);

        dll.name = "libbasarguitie.dll";
        dll.pkgVersion = basar::gui::tie::getVersion();
        basar::gui::qt::Manager::getInstance().initDll(dll);

        dll.name = "libbasarqtwidget.dll";
        dll.pkgVersion = basar::gui::widget::getVersion();
        basar::gui::qt::Manager::getInstance().initDll(dll);

        dll.name = "libbasarguiqt.dll";
        dll.pkgVersion = basar::gui::qt::getVersion();
        basar::gui::qt::Manager::getInstance().initDll(dll);

        return true;
    }

    bool dummy = about_define();  //!< explicit call for about-dlg´s definition

    //----------------------------------------------------------------------------
    } // end namespace viewConn
} // end namespace tender
