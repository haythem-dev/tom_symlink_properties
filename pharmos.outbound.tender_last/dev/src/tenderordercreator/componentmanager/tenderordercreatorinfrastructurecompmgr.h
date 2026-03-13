//----------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author Bjoern Bischof
 *  \date   08.07.2013
 */
//----------------------------------------------------------------------------//
#ifndef GUARD_TENDERORDERCREATOR_COMPONENTMANAGER_INFRASTRUCTURECOMPMGR_H
#define GUARD_TENDERORDERCREATOR_COMPONENTMANAGER_INFRASTRUCTURECOMPMGR_H

//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
#include "loggerpool/loggerpool.h"
#include "componentmanager/tenderordercreatorapplcompmgr.h"
#include "infrastructure/tenderordercreatorcmdlineevaluator.h"
#include "infrastructure/tenderordercreatorinfrastructuresetup.h"
#include "infrastructure/cscclient/icscclientconnectionptr.h"
#include "infrastructure/cscclient/icscclientparameterproxyptr.h"
#include "infrastructure/mail/imailparameterproxyptr.h"
#include "infrastructure/itenderordercreatorlogingetter.h"

#include <libutil/infrastructurecomponent.h>
#include <libutil/cmdline.h>

//----------------------------------------------------------------------------//
// namespace setcion
//----------------------------------------------------------------------------//
namespace tenderordercreator
{
    namespace componentManager
    {
        //----------------------------------------------------------------------------//
        // class declaration setcion
        //----------------------------------------------------------------------------//
        class InfrastructureCompMgr : public libutil::componentManager::InfrastructureCompMgrBase<
												tenderordercreator::componentManager::ApplCompMgr,
												tenderordercreator::infrastructure::cmdLine::TenderordercreatorCmdLineEvaluator,
												tenderordercreator::infrastructure::setup::TenderOrderCreatorInfrastructureSetup>,
						            public infrastructure::setup::IBatchLoginGetter,
									public boost::enable_shared_from_this< InfrastructureCompMgr >
        {
            public:
                InfrastructureCompMgr( int argc, char** argv );
                ~InfrastructureCompMgr();

            protected:
                ////////////////////////////////////////////////////////////////////////////////////////////////
                // <creation_section>
                ////////////////////////////////////////////////////////////////////////////////////////////////
                virtual libutil::infrastructure::IQApplicationPtr                           createQApplication()        const;
                virtual libutil::infrastructure::login::ILoginPtr                           createLogin()               const;
                virtual libutil::infrastructure::user::IUserPtr                             createUser()                const;
                virtual libutil::infrastructure::i18n::ILanguageFileLoaderPtr               createLanguageFileLoader()  const;
                virtual libutil::infrastructure::db::dbConnection::DBConnectionBasePtr      createDBConnection()        const;
                virtual libutil::infrastructure::parameter::IParameterGatewayPtr            createParameterGateway()    const;

                virtual void																doInjectApplComponentManagerComponents() const;
                virtual void                                                                doInjectInfrastructureSetupComponents() const;

                virtual libutil::infrastructure::login::ILoginPtr                           getNonInteractiveLogin( const libutil::infrastructure::login::UserLoginData& ) const;
				libtender::infrastructure::db::IPurchaseDBParameterProxyPtr					getPurchaseDBParameterProxy() const;

                ////////////////////////////////////////////////////////////////////////////////////////////////
                // </creation_section>
                ////////////////////////////////////////////////////////////////////////////////////////////////
            private:
                InfrastructureCompMgr( const InfrastructureCompMgr& );
                InfrastructureCompMgr& operator=( const InfrastructureCompMgr& );

                //libutil::infrastructure::login::UserLoginData	getLoginData()      const;

                libtender::infrastructure::cscclient::ICscClientParameterProxyPtr	getParameterProxy() const;
                infrastructure::mail::IMailParameterProxyPtr						getMailParameterProxy() const;
				libtender::infrastructure::cscclient::ICscClientConnectionPtr		getCscClientConnection() const;
				infrastructure::mail::IMailAlertPtr									getMailAlert() const;


                mutable libtender::infrastructure::cscclient::ICscClientParameterProxyPtr	m_ParameterProxy;
				mutable libtender::infrastructure::cscclient::ICscClientConnectionPtr		m_CscClientConnection;

				mutable infrastructure::mail::IMailParameterProxyPtr				m_MailParameterProxy;
				mutable libtender::infrastructure::db::IPurchaseDBParameterProxyPtr	m_PurchaseDBParameterProxy;
};

} // namespace componentManager
} // namespace tenderordercreator

#endif 
