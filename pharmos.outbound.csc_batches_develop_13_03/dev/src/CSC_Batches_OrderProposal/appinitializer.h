//--------------------------------------------------------------------------------------------------//
/*! \brief		AppInitChecker This Class is for checking the commandline and user-administration
 *  \author     
 *  \date       
 *  \version	
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_APP_INITIALIZER_H
#define GUARD_ABBAUW_APP_INITIALIZER_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "abbauwcmdlineevaluatorptr.h"

#include <useradmindbconfigurationptr.h>
#include <useradminloginptr.h>
#include <cmdlineconfiguration.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace libBatch
{
    namespace cmdLine
    {
        class CommandLineParser;
        struct CmdLineConfiguration;
    }
    
    namespace userAdmin
    {
        struct UserLoginConfiguration;
    }

    namespace dbConnection
    {
        struct DBConnectionConfiguration;
    }
}

namespace abbaUW
{
    namespace cmdLine
    {
        struct AppArgs;
    }
}
       

namespace log4cplus 
{
	class Logger;
}

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{
class AppInitializer
{  
    public:
		AppInitializer( const libBatch::cmdLine::CmdLineConfiguration& );	
        ~AppInitializer();
    
        void init();

        const cmdLine::AppArgs&                                     getAppArgs                  () const;
        const libBatch::userAdmin::UserLoginConfiguration&          getUserLoginConfiguration   () const;
        const libBatch::dbConnection::DBConnectionConfiguration&    getDBConnectionConfiguration() const;
        
	private:
        AppInitializer( const AppInitializer& );
        AppInitializer& operator=( const AppInitializer& );
        
        void handleCmdLine();
        void handleUserAdmin();
        void handleUserAdminLogIn();
        void handleUserAdminDBParameter();
        
        //const basar::VarString& getCountryCode() const;

        ////////////////////////////
        // memebers
        ////////////////////////////
        const log4cplus::Logger&                                m_Logger;
        const libBatch::cmdLine::CmdLineConfiguration           m_CmdLineConfiguration;
        cmdLine::AbbaUWCmdLineEvaluatorPtr				        m_AbbaUWCmdLineEvaluator;
        libBatch::userAdmin::UserAdminDBConfigurationPtr        m_UserAdminDBConfiguration;
        libBatch::userAdmin::UserAdminLoginPtr                  m_UserAdminLogin;
};

}

#endif
