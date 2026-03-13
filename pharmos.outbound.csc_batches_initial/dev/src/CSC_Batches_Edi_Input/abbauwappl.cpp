// appl. spec.
#include "abbauwappl.h"
#include "loggerpool.h"
#include "appinitializer.h"
#include "appsession.h"
#include "dbifxconnection.h"

#include "abbauwexception.h"
#include "unprocessableexception.h"
#include "editransferordermapper.h"

// lib: libbatch
#include <cmdlineparser.h>
#include <cmdlineconfiguration.h>
#include <classinfo_macros.h>
#include <dbconnectionconfiguration.h>
//#include <dbconnectionbaseptr.h>

// lib: BASAR
#include <libbasardbsql_databaseinfo.h> 
#include "infrastructure/mail/mailalert.h"

//-------------------------------------------------------------------------------------------------//
/*! \ingroup DIFF_WIN_UNIX */
//#ifdef __sun // Solaris
//    using basar::cmnutil::Singleton;
//    //! \brief explicit template instantiation for singleton
//    Singleton<abbaUW::ABBAUWApplBase>::InstancePtr Singleton<abbaUW::ABBAUWApplBase>::m_Instance;
//#endif

namespace basar
{
	namespace cmnutil
	{
		using abbaUW::ABBAUWApplBase;

		template <>
		typename Singleton< ABBAUWApplBase >::InstancePtr &
		Singleton< ABBAUWApplBase >::inst()
		{
			// the one and only instance
			static InstancePtr s_Inst;

			return s_Inst;
		}
	}
}

namespace abbaUW
{
    ABBAUWApplBase::ABBAUWApplBase() 
    : m_Logger( LoggerPool::LoggerAbbaUW ), m_InitFlag( "ABBAUWApplBase::init" )
    {
        METHODNAME_DEF( ABBAUWApplBase, ABBAUWApplBase ) 
        BLOG_TRACE_METHOD( LoggerPool::LoggerAbbaUW, fun );
    }
    
    ABBAUWApplBase::~ABBAUWApplBase()
    {
        METHODNAME_DEF( ABBAUWApplBase, ~ABBAUWApplBase ) 
        BLOG_TRACE_METHOD( LoggerPool::LoggerAbbaUW, fun );
    }
    
    void ABBAUWApplBase::init( int argc, char *argv[] )
    {
        METHODNAME_DEF( ABBAUWApplBase, init ) 
        BLOG_TRACE_METHOD( LoggerPool::LoggerAbbaUW, fun );
                
        try 
		{
            if( m_InitFlag.isSet() )
			{
				return; 
			}
            
            AppInitializer appInitializer( libBatch::cmdLine::CmdLineConfiguration( argc, argv ) );
            appInitializer.init();
            initSession( appInitializer );

			// Check for valid countrycode: Countrycode must be CH or DE! 
			checkCountryCodeValidity();		

			// Check for valid for syntax of ForBranchos-Parameter: Must be like "06,17,57"! 
			checkForBranchnosValidity();		

	        startSession();
	        
	        m_InitFlag.set();

        } 
		catch( basar::Exception& e )
		{
            // todo BB loggen ... 
            throw e;
        }
    }

    //-------------------------------------------------------------------------------------------------//
    /*! \throw BasarException if countrycode is invalid */
    //-------------------------------------------------------------------------------------------------//
	void ABBAUWApplBase::checkCountryCodeValidity()		
	{
		METHODNAME_DEF( ABBAUWApplBase, checkCountryCodeValidity ) 
        BLOG_TRACE_METHOD( m_Logger, fun );

		basar::cmnutil::I18nString i18nstr = m_AppSession->m_AppArgs.m_CountryCode;

		i18nstr.upper();

		if ( "CH" == i18nstr )
		{
			// setLocale for Switzerland CH
			basar::cmnutil::Locale::setLocale(basar::cmnutil::Locale::GERMAN_CH);
		}
		else if ( "DE1" == i18nstr || "DE2" == i18nstr )
		{
			// setLocale for Germany
			basar::cmnutil::Locale::setLocale(basar::cmnutil::Locale::GERMAN_DE);			// TODO: GERMAN_DE is ok?
		}
		else
		{	
			std::stringstream str;
			str << "Countrycode-Parameter=" << i18nstr << ". It must be CH or DE." << std::endl;
			throw basar::cmnutil::CmdLineException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}
	}

	//-------------------------------------------------------------------------------------------------//
    /*! \throw BasarException if checkForBranchnosValidity is invalid */
    //-------------------------------------------------------------------------------------------------//
	void ABBAUWApplBase::checkForBranchnosValidity()		
	{
		METHODNAME_DEF( ABBAUWApplBase, checkForBranchnosValidity ) 
        BLOG_TRACE_METHOD( m_Logger, fun );

		basar::cmnutil::I18nString i18nstr = m_AppSession->m_AppArgs.m_ForBranchnos;

		if (true)
		{
			// TODO: Syntax-Check ForBranchnos
		}
		else
		{	
			std::stringstream str;
			str << "ForBranchnos-Parameter=" << i18nstr << ". It must be like <06,08,45,56>" << std::endl;
			throw basar::cmnutil::CmdLineException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}
	}
    
    void ABBAUWApplBase::startSession()
    {
        METHODNAME_DEF( ABBAUWApplBase, startSession ) 
        BLOG_TRACE_METHOD( m_Logger, fun );

        m_AppSession->start();
    }

    void ABBAUWApplBase::endSession()
    {
        METHODNAME_DEF( ABBAUWApplBase, endSession ) 
        BLOG_TRACE_METHOD( m_Logger, fun );

        m_AppSession->end();
    }

    void ABBAUWApplBase::initSession( const AppInitializer& appInitializer )
    {
        METHODNAME_DEF( ABBAUWApplBase, initSession ) 
        BLOG_TRACE_METHOD( m_Logger, fun );

        const libBatch::dbConnection::DBConnectionConfiguration& dbConfig = appInitializer.getDBConnectionConfiguration();
        basar::db::sql::DatabaseInfo databaseInfo;
        databaseInfo.database = dbConfig.m_DB.c_str();
        databaseInfo.dbServer = dbConfig.m_DBSrv.c_str();

        m_AppSession = AppSessionPtr( new AppSession( libBatch::dbConnection::DBConnectionBasePtr(new dbConnection::DBIfxConnection(databaseInfo)),
                                                      appInitializer.getAppArgs(), 
                                                      appInitializer.getUserLoginConfiguration()
                                                    )
                                    );

		m_MailAlert = abbaUW::infrastructure::mail::MailAlertPtr(new abbaUW::infrastructure::mail::MailAlert(m_Logger));
		m_MailAlert->injectUserAdminMailConfiguration(appInitializer.getUserAdminMailConfiguration());
    }

    //-------------------------------------------------------------------------------------------------//
    /*! \throw no-throw */
    //-------------------------------------------------------------------------------------------------//
    void ABBAUWApplBase::shutdown()
    {
        METHODNAME_DEF( ABBAUWApplBase, shutdown ) 
        BLOG_TRACE_METHOD( LoggerPool::LoggerAbbaUW, fun );
        
        try 
		{
			m_InitFlag.checkSetting();
            endSession();
            
            m_InitFlag.reset();
        } 
		catch( basar::Exception& )
		{
            // todo BB loggen ... 
            throw;
        }    
    }

    //-------------------------------------------------------------------------------------------------//
    /*! \throw  no-throw
        \retval true if initialization was ok */
    //-------------------------------------------------------------------------------------------------//
    bool ABBAUWApplBase::run()
	{  
		METHODNAME_DEF( ABBAUWApplBase, run ) 
		BLOG_TRACE_METHOD( m_Logger, fun );

		bool retvalCopy		= true;
		bool retvalMap		= true;
		bool retvalDelete	= true;
		bool retval			= true;

		try 
        {
			m_InitFlag.checkSetting();

			// The command line argument "-transferdir" contains the name of the directory with uewauf-Flatfiles
			// in the readonly system-transfer-directory (e.g. /transfer/ksc/data/ch/edi/uw)
			basar::I18nString strTransferDir(m_AppSession->m_AppArgs.m_TransferDir);

			// The command line argument "-inputdir" contains the name of the directory with uewauf-Flatfiles
			// in our own writable directory (e.g. $WSS/<country>/ksc/batch/data/edi_input)
			basar::I18nString strInputDir(m_AppSession->m_AppArgs.m_InputDir);

			basar::I18nString countrycode = m_AppSession->m_AppArgs.m_CountryCode;
			countrycode.upper();

			basar::I18nString strForBranchnos(m_AppSession->m_AppArgs.m_ForBranchnos);


			domMod::orderMapper::EdiTransferOrderMapper etom(	m_AppSession->m_DBConnection->getCurrentConnection(), 
																countrycode,
																strTransferDir, 
																strInputDir,
																strForBranchnos);	

			etom.injectMailAlert(m_MailAlert);

			// Move uewauf-Flatfiles from transferdir to inputdir
			retvalCopy = etom.doMoveFromTransferDirToInputDir();

			// Process all transfer orders
			retvalMap = etom.doMapInputDir();

			// Clean the input directory
			retvalDelete = etom.doCleanInputDir();

		} 
		catch ( basar::Exception& e )
		{
			std::stringstream str;
			str << "ERROR: Could not map or clean transfer order-inputdirectory "  << e.what() << std::endl;
			BLOG_ERROR( m_Logger, str.str().c_str());
			retval = false;
		}
		catch ( std::exception& e )
		{
			std::stringstream str;
			str << "ERROR: Could not map or clean transfer order-inputdirectory "  << e.what() << std::endl;
			BLOG_ERROR( m_Logger, str.str().c_str());
			retval = false;
		}

		if ( retval && retvalCopy && retvalMap && retvalDelete )
		{
			return true;
		}
		else
		{
			return false;
		}
    }

} // namespace abbaUW
