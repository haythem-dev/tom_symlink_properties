#ifndef GUARD_CSC_BATCHES_COMPONENTMANAGER_INFRASTRUCTURECOMPMGR_H
#define GUARD_CSC_BATCHES_COMPONENTMANAGER_INFRASTRUCTURECOMPMGR_H

#include "applcomponentmanager.h"

#include "ilanguagefileloadergetter.h"
#include "infrastructuresetup.h"

#include "icscsessionptr.h"
#include <libutil/infrastructurecomponent.h>
#include <libutil/cmdline.h>

namespace CSC_Batches
{
namespace componentManager
{
    class InfrastructureCompMgr : 
        public libutil::componentManager::InfrastructureCompMgrBase< ApplComponentManager,
																	 libutil::infrastructure::cmdLine::NullCmdLineEvaluator,
																	 CSC_Batches::infrastructure::InfrastructureSetup >, 
		public CSC_Batches::infrastructure::ILanguageFileLoaderGetter,
		public boost::enable_shared_from_this< InfrastructureCompMgr >
    {
        public:
            InfrastructureCompMgr( int argc, char** argv );
            ~InfrastructureCompMgr();

        protected:
            virtual libutil::infrastructure::IQApplicationPtr                           createQApplication()        const;
            virtual libutil::infrastructure::login::ILoginPtr                           createLogin()               const;
            virtual libutil::infrastructure::user::IUserPtr                             createUser()                const;
            virtual libutil::infrastructure::i18n::ILanguageFileLoaderPtr               createLanguageFileLoader()  const;
            virtual libutil::infrastructure::db::dbConnection::DBConnectionBasePtr      createDBConnection()        const;
            virtual libutil::infrastructure::parameter::IParameterGatewayPtr            createParameterGateway()    const;

			void																		doInjectInfrastructureSetupComponents() const;
            void                                                                        doInjectApplComponentManagerComponents() const;

            virtual libutil::infrastructure::parameter::IParameterGatewayPtr            doGetDBParameterGateway() const;

        private:
            InfrastructureCompMgr( const InfrastructureCompMgr& );
            InfrastructureCompMgr& operator=( const InfrastructureCompMgr& );

            infrastructure::session::ICSCSessionPtr                                         getCSCSession()     const;
            libutil::infrastructure::login::UserLoginData                                   getLoginData()      const;
            basar::I18nString                                                               getLanguageCode()   const;
            libutil::infrastructure::i18n::LanguageFileLoader::LanguageDirectoryCollection  getLanguageDirs()   const;
    };

} // end namespace componentManager
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_COMPONENTMANAGER_INFRASTRUCTURECOMPMGR_H
