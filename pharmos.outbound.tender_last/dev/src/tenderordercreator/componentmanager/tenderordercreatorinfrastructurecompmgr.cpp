//----------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author Bjoern Bischof
 *  \date   08.07.2013
 */
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
#include "tenderordercreatorinfrastructurecompmgr.h"

#include "infrastructure/login_definitions.h"
#include "loggerpool/loggerpool.h"

#include "infrastructure/cscclient/cscclientparameterproxyptr.h"
#include "infrastructure/cscclient/cscclientparameterproxy.h"
#include "infrastructure/cscclient/cscclientconnection.h"
#include "infrastructure/cscclient/cscclientconnectionptr.h"

#include "infrastructure/mail/mailalertptr.h"
#include "infrastructure/mail/mailalert.h"
#include "infrastructure/mail/mailparameterproxyptr.h"
#include "infrastructure/mail/mailparameterproxy.h"

#include "infrastructure/purchasedbconn/purchasedbparameterproxyptr.h"
#include "infrastructure/purchasedbconn/purchasedbparameterproxy.h"

#include <infrastructure/countrycodemapper.h>


//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace tenderordercreator {
namespace componentManager {
        //----------------------------------------------------------------------------//
        // class definition section
        //----------------------------------------------------------------------------//
        InfrastructureCompMgr::InfrastructureCompMgr( int argc, char** argv )
        : libutil::componentManager::InfrastructureCompMgrBase< tenderordercreator::componentManager::ApplCompMgr,
                                                            tenderordercreator::infrastructure::cmdLine::TenderordercreatorCmdLineEvaluator,
                                                            tenderordercreator::infrastructure::setup::TenderOrderCreatorInfrastructureSetup >( argc, argv, tenderordercreator::LoggerPool::getLoggerTenderordercreator() )
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

            return libutil::infrastructure::login::ILoginPtr( new libutil::infrastructure::login::NullLogin() );
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

            libutil::infrastructure::i18n::NullLanguageFileLoaderPtr loader = libutil::infrastructure::i18n::NullLanguageFileLoaderPtr( new libutil::infrastructure::i18n::NullLanguageFileLoader() );
            return loader;
        }

        libutil::infrastructure::db::dbConnection::DBConnectionBasePtr InfrastructureCompMgr::createDBConnection() const
        {
            METHODNAME_DEF( InfrastructureCompMgr, createDBConnection )
            BLOG_TRACE_METHOD( getLogger(), fun );
            return libutil::infrastructure::db::dbConnection::DBConnectionBasePtr( new libutil::infrastructure::db::dbConnection::DBIfxConnection() );
        }

        libutil::infrastructure::parameter::IParameterGatewayPtr InfrastructureCompMgr::createParameterGateway() const
        {
            METHODNAME_DEF( InfrastructureCompMgr, createParameterGateway )
            BLOG_TRACE_METHOD( getLogger(), fun );

            return libutil::infrastructure::parameter::IParameterGatewayPtr( new libutil::infrastructure::parameter::UserAdminParameter() );
        }

        void InfrastructureCompMgr::doInjectApplComponentManagerComponents() const
        {
            METHODNAME_DEF( InfrastructureCompMgr, doInjectInfrastructureSetupComponents )
            BLOG_TRACE_METHOD( getLogger(), fun );

            boost::shared_ptr< const libutil::infrastructure::cmdLine::CmdLineEvaluatorBase<tenderordercreator::infrastructure::cmdLine::AppArgs,libutil::infrastructure::cmdLine::NullCmdLineOptionCollection> > p = getCmdLineEvaluator();
            getApplComponentManager()->setCmdLineArguments( p->getAppArgs() );
			getApplComponentManager()->injectCscClientConnection( getCscClientConnection() );
			getApplComponentManager()->injectMailAlert( getMailAlert() );
			getApplComponentManager()->injectPurchaseDBParameterProxy ( getPurchaseDBParameterProxy() );
        }

        void InfrastructureCompMgr::doInjectInfrastructureSetupComponents() const
        {
            METHODNAME_DEF( InfrastructureCompMgr, doInjectInfrastructureSetupComponents )
            BLOG_TRACE_METHOD( getLogger(), fun );
    
            getInfrastructureSetup()->injectLoginGetter( shared_from_this() );
        }

		libutil::infrastructure::login::ILoginPtr InfrastructureCompMgr::getNonInteractiveLogin( const libutil::infrastructure::login::UserLoginData& userLoginData ) const
        {
            METHODNAME_DEF( InfrastructureCompMgr, getNonInteractiveLogin )
            BLOG_TRACE_METHOD( getLogger(), fun );

            return libutil::infrastructure::login::ILoginPtr( new libutil::infrastructure::login::BatchUserAdminLogin(userLoginData) );
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

infrastructure::mail::IMailAlertPtr	InfrastructureCompMgr::getMailAlert() const
{
	infrastructure::mail::MailAlertPtr p = infrastructure::mail::MailAlertPtr(new infrastructure::mail::MailAlert(LoggerPool::getLoggerTenderordercreator()));
	p->injectParameterProxy(getMailParameterProxy());
	return p;
}

infrastructure::mail::IMailParameterProxyPtr InfrastructureCompMgr::getMailParameterProxy() const
{
	METHODNAME_DEF( InfrastructureCompMgr, getMailParameterProxy )
	BLOG_TRACE_METHOD( getLogger(), fun );

	if ( NULL == m_MailParameterProxy.get() )
	{
		using infrastructure::mail::MailParameterProxyPtr;
		using infrastructure::mail::MailParameterProxy;

		MailParameterProxyPtr paraProxy = MailParameterProxyPtr( new MailParameterProxy() );

		paraProxy->injectParameterGateway( getParameterGateway() );
		m_MailParameterProxy = paraProxy;
	}

	return m_MailParameterProxy;
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


} // namespace infrastructure
} // namespace tenderordercreator
