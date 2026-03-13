#ifndef GUARD_TENDER_COMPONENTMANAGER_TENDERINFRASTRUCTURECOMPMGR_H
#define GUARD_TENDER_COMPONENTMANAGER_TENDERINFRASTRUCTURECOMPMGR_H

#include <loggerpool/tenderloggerpool.h>
#include <tenderapplcompmgr.h>
#include <infrastructure/ilanguagefileloadergetter.h>
#include <infrastructure/tenderinfrastructuresetup.h>
#include "infrastructure/purchasedbconn/ipurchasedbparameterproxyptr.h"

#include "infrastructure/cscclient/icscclientconnectionptr.h"
#include "infrastructure/cscclient/icscclientparameterproxyptr.h"

#include <libutil/infrastructurecomponent.h>
#include <libutil/cmdline.h>

#include <boost/enable_shared_from_this.hpp>

namespace tender
{
    namespace componentManager
    {
        class InfrastructureCompMgr : public libutil::componentManager::InfrastructureCompMgrBase< 
										tender::componentManager::ApplCompMgr,
										libutil::infrastructure::cmdLine::NullCmdLineEvaluator,
										tender::infrastructure::TenderInfrastructureSetup >, 
									  public tender::infrastructure::ILanguageFileLoaderGetter,
									  public boost::enable_shared_from_this< InfrastructureCompMgr >
        {
            public:
                InfrastructureCompMgr( int argc, char** argv );
	virtual ~InfrastructureCompMgr();

            protected:
                virtual libutil::infrastructure::IQApplicationPtr                           createQApplication()        const;
                virtual libutil::infrastructure::login::ILoginPtr                           createLogin()               const;
                virtual libutil::infrastructure::user::IUserPtr                             createUser()                const;
                virtual libutil::infrastructure::i18n::ILanguageFileLoaderPtr               createLanguageFileLoader()  const;
                virtual libutil::infrastructure::db::dbConnection::DBConnectionBasePtr      createDBConnection()        const;
                virtual libutil::infrastructure::parameter::IParameterGatewayPtr            createParameterGateway()    const;

                virtual void                                                                doInjectInfrastructureSetupComponents() const;
				virtual void																doInjectApplComponentManagerComponents() const;

				libtender::infrastructure::db::IPurchaseDBParameterProxyPtr					getPurchaseDBParameterProxy() const;

            private:
                InfrastructureCompMgr( const InfrastructureCompMgr& );
                InfrastructureCompMgr& operator=( const InfrastructureCompMgr& );

				libtender::infrastructure::cscclient::ICscClientParameterProxyPtr				getParameterProxy() const;
				libtender::infrastructure::cscclient::ICscClientConnectionPtr					getCscClientConnection() const;

                void                                                                            loadStyleSheet()    const;
                libutil::infrastructure::login::UserLoginData                                   getLoginData()      const;
                basar::I18nString                                                               getLanguageCode()   const;
                libutil::infrastructure::i18n::LanguageFileLoader::LanguageDirectoryCollection  getLanguageDirs()   const;

				mutable libtender::infrastructure::db::IPurchaseDBParameterProxyPtr				m_PurchaseDBParameterProxy;

				mutable libtender::infrastructure::cscclient::ICscClientParameterProxyPtr		m_ParameterProxy;
				mutable libtender::infrastructure::cscclient::ICscClientConnectionPtr			m_CscClientConnection;
        };
    } // namespace componentManager
} // namespace tender

#endif
