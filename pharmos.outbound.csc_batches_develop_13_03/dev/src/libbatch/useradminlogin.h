//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       27.12.2010
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_USER_ADMIN_LOGIN_H
#define GUARD_LIBBATCH_USER_ADMIN_LOGIN_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil.h>
#include "userloginconfiguration.h"

//--------------------------------------------------------------------------------------------------//
// namespace setcion
//--------------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{

    class UserAdminLogin
    {
        public:
            UserAdminLogin( const UserLoginConfiguration& userLoginConfiguration );
	        ~UserAdminLogin();
	        void login();
            const UserLoginConfiguration& getUserLoginConfiguration() const;
            bool isloggedIn() const;

        private:
            ////////////////////////////////
            // methods
            ////////////////////////////////
	        UserAdminLogin( const UserAdminLogin& r );
	        UserAdminLogin& operator = ( const UserAdminLogin& r );

            void logLogin() const;
            
            ////////////////////////////////
            // variables
            ////////////////////////////////
            const log4cplus::Logger&    m_Logger;
            UserLoginConfiguration      m_UserLoginConfiguration;
            bool                        m_LoggedIn;
    };

} // end namespace userAdmin
} // end namespace libBatch

#endif // end GUARD_LIBBATCH_USER_ADMIN_LOGIN_H
