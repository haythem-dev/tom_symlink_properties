#include "infrastructurecompmgr.h"

#include "definitions_gui.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include "applparameter.h"
#include "applparameterptr.h"
#include "cscuseradminlogin.h"
#include "cscsession.h"

#include <libbasarguitie_qtmessagehandler.h>
#include <boost/make_shared.hpp>

namespace CSC_Batches
{
namespace componentManager
{
    InfrastructureCompMgr::InfrastructureCompMgr( int argc, char** argv )
    : libutil::componentManager::InfrastructureCompMgrBase< 
		ApplComponentManager,
		libutil::infrastructure::cmdLine::NullCmdLineEvaluator,
		CSC_Batches::infrastructure::InfrastructureSetup>( argc, argv, libabbauw::LoggerPool::loggerMain )
    {
        METHODNAME_DEF( InfrastructureCompMgr, InfrastructureCompMgr)
        BLOG_TRACE_METHOD( getLogger(), fun );

    }

    InfrastructureCompMgr::~InfrastructureCompMgr()
    {
        METHODNAME_DEF( InfrastructureCompMgr, ~InfrastructureCompMgr )
        BLOG_TRACE_METHOD( getLogger(), fun );

    }

	void InfrastructureCompMgr::doInjectInfrastructureSetupComponents() const
	{
		METHODNAME_DEF( InfrastructureCompMgr, doInjectInfrastructureSetupComponents )
		BLOG_TRACE_METHOD( getLogger(), fun );
	
		getInfrastructureSetup()->injectLanguageFileLoaderGetter( shared_from_this() );
	}

    void InfrastructureCompMgr::doInjectApplComponentManagerComponents() const
    {
        getApplComponentManager()->setCSCSession( getCSCSession() );
        getApplComponentManager()->injectParameterGateway( getParameterGateway() );
    }

    libutil::infrastructure::IQApplicationPtr InfrastructureCompMgr::createQApplication() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, createQApplication )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return boost::make_shared< libutil::infrastructure::QApplication >( getCmdLineConfiguration().m_Argc, getCmdLineConfiguration().m_Argv );
        //qInstallMessageHandler( basar::gui::tie::basarQTMessageHandler );
        //QApplication::instance()->setWindow
        //return qApp;
    }

    libutil::infrastructure::login::ILoginPtr InfrastructureCompMgr::createLogin() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, createLogin )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return boost::make_shared< CSC_Batches::infrastructure::login::CSCUserAdminLogin >( getLoginData() );
    }

    libutil::infrastructure::user::IUserPtr InfrastructureCompMgr::createUser() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, createUser )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return boost::make_shared< libutil::infrastructure::user::UserAdminUser >();
    }

    libutil::infrastructure::i18n::ILanguageFileLoaderPtr InfrastructureCompMgr::createLanguageFileLoader() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, createLanguageFileLoader )
        BLOG_TRACE_METHOD( getLogger(), fun );

        libutil::infrastructure::i18n::LanguageFileLoaderPtr loader = boost::make_shared< libutil::infrastructure::i18n::LanguageFileLoader >( getLanguageCode(), getLanguageDirs() ) ;
        loader->injectQApplication( getQApplication() );
        return loader;
    }

    libutil::infrastructure::db::dbConnection::DBConnectionBasePtr InfrastructureCompMgr::createDBConnection() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, createDBConnection )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return boost::make_shared< libutil::infrastructure::db::dbConnection::DBIfxConnection >();
    }

    libutil::infrastructure::parameter::IParameterGatewayPtr InfrastructureCompMgr::createParameterGateway() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, createParameterGateway )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return boost::make_shared< libutil::infrastructure::parameter::UserAdminParameter >();
    }

    libutil::infrastructure::parameter::IParameterGatewayPtr InfrastructureCompMgr::doGetDBParameterGateway() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, doGetDBParameterGateway )
        BLOG_TRACE_METHOD( getLogger(), fun );
    
        infrastructure::parameter::ApplParameterPtr parameter = boost::make_shared< infrastructure::parameter::ApplParameter >();
        parameter->injectParameterGateway( createParameterGateway() );

        return parameter;
    }

    infrastructure::session::ICSCSessionPtr InfrastructureCompMgr::getCSCSession() const
    {
        typedef boost::shared_ptr< infrastructure::session::CSCSession > CSCSessionPtr;
        CSCSessionPtr session = boost::make_shared< infrastructure::session::CSCSession > ();
        session->injectSession( getSession() );
        session->injectParameterGateway( doGetDBParameterGateway() );
        
        return session;
    }

    libutil::infrastructure::login::UserLoginData InfrastructureCompMgr::getLoginData() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, getLoginData )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return libutil::infrastructure::login::UserLoginData( libutil::STRING_UNSET, libutil::STRING_UNSET, 
                                                              CSC_Batches::APPLICATION );
    }

    basar::I18nString InfrastructureCompMgr::getLanguageCode() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, getLanguageCode )
        BLOG_TRACE_METHOD( getLogger(), fun );

        basar::I18nString languageID = getLogin()->getUserLoginData().getLanguageID();
        languageID.lower();
		
		return languageID;
    }

    libutil::infrastructure::i18n::LanguageFileLoader::LanguageDirectoryCollection InfrastructureCompMgr::getLanguageDirs() const
    {
        METHODNAME_DEF( InfrastructureCompMgr, getLanguageDirs )
        BLOG_TRACE_METHOD( getLogger(), fun );

        QString multiLanguageDir = "multilang";
        libutil::infrastructure::i18n::LanguageFileLoader::LanguageDirectoryCollection dirs;
        dirs.push_back( getQApplication()->applicationDirPath() + "/" + multiLanguageDir );
        dirs.push_back( QDir::currentPath() + "/../" + multiLanguageDir );

        return dirs;
    }
} // end namespace infrastructure
} // end namespace CSC_Batches
