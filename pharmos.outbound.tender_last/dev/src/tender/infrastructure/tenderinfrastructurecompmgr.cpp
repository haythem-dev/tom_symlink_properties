#include "tenderinfrastructurecompmgr.h"
#include <infrastructure/purchasedbconn/purchasedbparameterproxyptr.h>
#include <infrastructure/purchasedbconn/purchasedbparameterproxy.h>
#include "tender_definitions.h"
#include "tender_version.h"

#include <infrastructure/cscclient/cscclientconnection.h>
#include <infrastructure/cscclient/cscclientconnectionptr.h>
#include <infrastructure/cscclient/cscclientparameterproxyptr.h>
#include <infrastructure/cscclient/cscclientparameterproxy.h>

namespace tender
{
namespace componentManager
{
InfrastructureCompMgr::InfrastructureCompMgr( int argc, char** argv )
: libutil::componentManager::InfrastructureCompMgrBase< 
	tender::componentManager::ApplCompMgr,
	libutil::infrastructure::cmdLine::NullCmdLineEvaluator,
	tender::infrastructure::TenderInfrastructureSetup
		>( argc, argv, libtender::LoggerPool::getLoggerTender() )
{
	METHODNAME_DEF( InfrastructureCompMgr, InfrastructureCompMgr)
	BLOG_TRACE_METHOD( getLogger(), fun );
}

InfrastructureCompMgr::~InfrastructureCompMgr()
{
	METHODNAME_DEF( InfrastructureCompMgr, ~InfrastructureCompMgr )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

libutil::infrastructure::IQApplicationPtr InfrastructureCompMgr::createQApplication() const
{
	METHODNAME_DEF( InfrastructureCompMgr, createQApplication )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return libutil::infrastructure::IQApplicationPtr( new libutil::infrastructure::QApplication(getCmdLineConfiguration().m_Argc, getCmdLineConfiguration().m_Argv) );
}

libutil::infrastructure::login::ILoginPtr InfrastructureCompMgr::createLogin() const
{
	METHODNAME_DEF( InfrastructureCompMgr, createLogin )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return libutil::infrastructure::login::ILoginPtr( new libutil::infrastructure::login::OnlineUserAdminLogin(getLoginData()) );
}

libutil::infrastructure::login::UserLoginData InfrastructureCompMgr::getLoginData() const
{
	METHODNAME_DEF( InfrastructureCompMgr, getLoginData )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::I18nString appVersion = TENDER_VERSION;
	return libutil::infrastructure::login::UserLoginData( libutil::STRING_UNSET, libutil::STRING_UNSET, tender_APPLICATION_NAME, libutil::NUMBER_UNSET, libutil::STRING_UNSET, appVersion, true);
	//return libutil::infrastructure::login::UserLoginData( libutil::STRING_UNSET, libutil::STRING_UNSET, tender_APPLICATION_NAME );
}

libutil::infrastructure::user::IUserPtr InfrastructureCompMgr::createUser() const
{
	METHODNAME_DEF( InfrastructureCompMgr, createUser )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return libutil::infrastructure::user::IUserPtr( new libutil::infrastructure::user::UserAdminUser() );
}

libutil::infrastructure::i18n::ILanguageFileLoaderPtr InfrastructureCompMgr::createLanguageFileLoader() const
{
	METHODNAME_DEF( InfrastructureCompMgr, createLanguageFileLoader )
	BLOG_TRACE_METHOD( getLogger(), fun );

	libutil::infrastructure::i18n::LanguageFileLoaderPtr loader = libutil::infrastructure::i18n::LanguageFileLoaderPtr( 
		new libutil::infrastructure::i18n::LanguageFileLoader( getLanguageCode(), getLanguageDirs() ) );
	loader->injectQApplication( getQApplication() );
	return loader;
}

libutil::infrastructure::db::dbConnection::DBConnectionBasePtr InfrastructureCompMgr::createDBConnection() const
{
	METHODNAME_DEF( InfrastructureCompMgr, createDBConnection )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return libutil::infrastructure::db::dbConnection::DBConnectionBasePtr( new libutil::infrastructure::db::dbConnection::DBIfxConnection(/*dbServerInfo*/) );
}

libutil::infrastructure::parameter::IParameterGatewayPtr InfrastructureCompMgr::createParameterGateway() const
{
	METHODNAME_DEF( InfrastructureCompMgr, createParameterGateway )
	BLOG_TRACE_METHOD( getLogger(), fun );

	return libutil::infrastructure::parameter::IParameterGatewayPtr( new libutil::infrastructure::parameter::UserAdminParameter() );
}

void InfrastructureCompMgr::doInjectInfrastructureSetupComponents() const
{
	METHODNAME_DEF( InfrastructureCompMgr, doInjectInfrastructureSetupComponents )
	BLOG_TRACE_METHOD( getLogger(), fun );

	getInfrastructureSetup()->injectLanguageFileLoaderGetter( shared_from_this() );
}

void InfrastructureCompMgr::doInjectApplComponentManagerComponents() const
{
	METHODNAME_DEF( InfrastructureCompMgr, doInjectInfrastructureSetupComponents )
	BLOG_TRACE_METHOD( getLogger(), fun );

	getApplComponentManager()->injectPurchaseDBParameterProxy ( getPurchaseDBParameterProxy() );
	getApplComponentManager()->injectParameterGateway( createParameterGateway() );
	getApplComponentManager()->injectCscClientConnection( getCscClientConnection() );
	getApplComponentManager()->injectParameterProxy( getParameterProxy() );
}

libtender::infrastructure::db::IPurchaseDBParameterProxyPtr InfrastructureCompMgr::getPurchaseDBParameterProxy() const
{
	METHODNAME_DEF( InfrastructureCompMgr, getPurchaseDBParameterProxy )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if ( NULL == m_PurchaseDBParameterProxy.get() )
	{
		using libtender::infrastructure::db::PurchaseDBParameterProxyPtr;
		using libtender::infrastructure::db::PurchaseDBParameterProxy;

		PurchaseDBParameterProxyPtr paraProxy = PurchaseDBParameterProxyPtr( new PurchaseDBParameterProxy() );

		paraProxy->injectParameterGateway( getParameterGateway() );
		m_PurchaseDBParameterProxy = paraProxy;
	}

	return m_PurchaseDBParameterProxy;
}

libtender::infrastructure::cscclient::ICscClientParameterProxyPtr InfrastructureCompMgr::getParameterProxy() const
{
	METHODNAME_DEF( InfrastructureCompMgr, getParameterProxy )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if ( NULL == m_ParameterProxy.get() )
	{
		using libtender::infrastructure::cscclient::CscClientParameterProxyPtr;
		using libtender::infrastructure::cscclient::CscClientParameterProxy;
			
		CscClientParameterProxyPtr paraProxy = CscClientParameterProxyPtr( new CscClientParameterProxy() );
			
		paraProxy->injectParameterGateway( getParameterGateway() );
		m_ParameterProxy = paraProxy;
	}
	return m_ParameterProxy;
}

libtender::infrastructure::cscclient::ICscClientConnectionPtr InfrastructureCompMgr::getCscClientConnection() const
{
	METHODNAME_DEF( InfrastructureCompMgr, getCscClientConnection )
	BLOG_TRACE_METHOD( getLogger(), fun );
		
	if ( NULL == m_CscClientConnection.get() )
	{
		libtender::infrastructure::cscclient::CscClientConnectionPtr ptr = libtender::infrastructure::cscclient::CscClientConnectionPtr( new libtender::infrastructure::cscclient::CscClientConnection(getLogger()) );
		ptr->injectParameterProxy( getParameterProxy() );
		m_CscClientConnection = ptr;
	}
		
	return m_CscClientConnection;
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

} // namespace infrastructure
} // namespace tender
