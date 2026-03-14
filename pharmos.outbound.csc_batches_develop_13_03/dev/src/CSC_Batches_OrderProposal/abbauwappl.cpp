#include "abbauwappl.h"
#include "loggerpool.h"
#include "appinitializer.h"
#include "appsession.h"
#include "dbifxconnection.h"
#include "addtopubatchorderposuc.h"
#include "updateorderproposaluc.h"
#include "orderproposalinitparams.h"
#include "pubatchorderposinitparams.h"

// lib: libbatch
#include <cmdlineparser.h>
#include <cmdlineconfiguration.h>
#include <classinfo_macros.h>
#include <dbconnectionconfiguration.h>
#include <dbconnectionbaseptr.h>

// lib: BASAR
#include <libbasardbsql_databaseinfo.h> 

namespace basar
{
	namespace cmnutil
	{
		template<> typename Singleton<abbaUW::ABBAUWApplBase>::InstancePtr & Singleton<abbaUW::ABBAUWApplBase>::inst()
		{
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
        BLOG_TRACE_METHOD( LoggerPool::LoggerAbbaUW, fun);
        
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
			checkCountryCodeValidity();
			// Check for valid countrycode: Countrycode must be CH or DE! 
			// Other countries will need code changes ("ToDo NotCH")
			
	        startSession();
	        
	        m_InitFlag.set();

        } 
		catch( basar::Exception& e )
		{
            // todo BB loggen ... 
            throw e;
        }
    }

	void ABBAUWApplBase::checkCountryCodeValidity()		
	{
		METHODNAME_DEF( ABBAUWApplBase, checkCountryCodeValidity ) 
        BLOG_TRACE_METHOD( m_Logger, fun );

		basar::cmnutil::I18nString i18nstr = m_AppSession->m_AppArgs.m_CountryCode;

		i18nstr.upper();

		if ("CH" != i18nstr && "DE1" != i18nstr && "DE2" != i18nstr && "AT" != i18nstr)
		{
			// ToDo NotCH: Countrycode must be CH or DE1 or DE2 or AT yet.  
			// ToDo NotCH: Different code changes in domain modules for other countries
			std::stringstream str;
			str << "Countrycode-Parameter=" << i18nstr << ". It must be CH, DE1, DE2 or AT." << std::endl;
			throw basar::cmnutil::CmdLineException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}

		if ("CH" == i18nstr)
		{
			// setLocale for Switzerland CH
			basar::cmnutil::Locale::setLocale(basar::cmnutil::Locale::GERMAN_CH);
		}
		else if ("DE1" == i18nstr || "DE2" == i18nstr)
		{
			// setLocale for Germany DE1 (North) or DE2 (South)
			basar::cmnutil::Locale::setLocale(basar::cmnutil::Locale::GERMAN_DE);
		}
		else if ("AT" == i18nstr )
		{
			// setLocale for Austria AT
			basar::cmnutil::Locale::setLocale(basar::cmnutil::Locale::GERMAN_DE);
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

		basar::db::sql::DatabaseInfo databaseInfoOrderprop;
		basar::db::sql::DatabaseInfo databaseInfoPuBatch;

		//connection from usradm for orderproposal
		const libBatch::dbConnection::DBConnectionConfiguration& dbConfig = appInitializer.getDBConnectionConfiguration();

		databaseInfoOrderprop.setInfxConcurrent(dbConfig.m_DBSrv.c_str(), dbConfig.m_DB.c_str());
		
		//connection from parameters for pubatchorderpos
		const basar::VarString db = appInitializer.getAppArgs().m_DB;
		const basar::VarString server = appInitializer.getAppArgs().m_Server;

		databaseInfoPuBatch.setInfxConcurrent(server, db);

        m_AppSession = AppSessionPtr( new AppSession( libBatch::dbConnection::DBConnectionBasePtr(new dbConnection::DBIfxConnection(databaseInfoOrderprop)),
													  libBatch::dbConnection::DBConnectionBasePtr(new dbConnection::DBIfxConnection(databaseInfoPuBatch)),
                                                      appInitializer.getAppArgs(), 
                                                      appInitializer.getUserLoginConfiguration()
                                                    )
                                    );
    }

    void ABBAUWApplBase::shutdown()
    {
        METHODNAME_DEF( ABBAUWApplBase, shutdown ) 
        BLOG_TRACE_METHOD( LoggerPool::LoggerAbbaUW,fun );
        
        try 
		{
			m_InitFlag.checkSetting();
            
			shutdownOrderProposalDM();
			shutdownPuBatchOrderPosDM();
            endSession();
			
            m_InitFlag.reset();
		} 
		catch( basar::Exception& e )
		{
			BLOG_ERROR(LoggerPool::LoggerAbbaUW, e.what());
            throw e;
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
      
		try 
		{
			m_InitFlag.checkSetting();
			CSC_Batches::useCase::UpdateOrderProposalUC updateOPUC;			
			CSC_Batches::useCase::AddToPuBatchOrderPosUC addToPBOPUC;
			updateOPUC.injectPuBatchOrderPositionFinder(getPuBatchOrderPosDM());
			updateOPUC.injectOrderProposalWriter(getOrderProposalDM());
			addToPBOPUC.injectOrderProposalFinder(getOrderProposalDM());
			addToPBOPUC.injectOrderProposalWriter(getOrderProposalDM());
			addToPBOPUC.injectPuBatchOrderPositionWriter(getPuBatchOrderPosDM());
			updateOPUC.run();
			addToPBOPUC.run();
			return true;
		}
		catch( basar::Exception& e )
		{
			std::stringstream str;
			str << "ERROR: Could not map or clean UEWAUF-directory "  << e.what() << std::endl;
			BLOG_ERROR( m_Logger, str.str().c_str());
		}
		catch (std::exception& e)
		{
			std::stringstream str;
			str << "ERROR: Could not map or clean UEWAUF-directory "  << e.what() << std::endl;
			BLOG_ERROR( m_Logger, str.str().c_str());
		}
           
		return false;
    }

	boost::shared_ptr< domMod::OrderProposalTransfer::OrderProposalDM > ABBAUWApplBase::getOrderProposalDM()
    {
        if( NULL == m_OrderProposalDM.get() )
		{
            m_OrderProposalDM = boost::shared_ptr< domMod::OrderProposalTransfer::OrderProposalDM >( new domMod::OrderProposalTransfer::OrderProposalDM(m_AppSession->m_DBConnectionOutbound->getCurrentConnection()) );
			domMod::OrderProposalTransfer::OrderProposalInitParams param;
			m_OrderProposalDM->init(param);
        }

        return m_OrderProposalDM;
    }

	boost::shared_ptr< domMod::OrderProposalTransfer::PuBatchOrderPosDM > ABBAUWApplBase::getPuBatchOrderPosDM()
    {
        if( NULL == m_PuBatchOrderPosDM.get() )
		{
            m_PuBatchOrderPosDM = boost::shared_ptr< domMod::OrderProposalTransfer::PuBatchOrderPosDM >( new domMod::OrderProposalTransfer::PuBatchOrderPosDM(m_AppSession->m_DBConnectionInbound->getCurrentConnection()) );
			domMod::OrderProposalTransfer::PuBatchOrderPosInitParams param;
			m_PuBatchOrderPosDM->init(param);
        }

        return m_PuBatchOrderPosDM;
    }

	void ABBAUWApplBase::shutdownOrderProposalDM()
	{
		if( NULL != m_OrderProposalDM.get() )
		{
            m_OrderProposalDM->shutdown();
        }
	}

	void ABBAUWApplBase::shutdownPuBatchOrderPosDM()
	{
		if( NULL != m_PuBatchOrderPosDM.get() )
		{
            m_PuBatchOrderPosDM->shutdown();
        }
	}

} // namespace abbaUW
