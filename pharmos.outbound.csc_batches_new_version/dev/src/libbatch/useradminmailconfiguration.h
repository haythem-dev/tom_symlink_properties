//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Ivan Fildishev
 *  \date       17.11.2023
 *  \version    00.00.00.01 first version
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_USER_ADMIN_MAIL_CONFIGURATION_H
#define GUARD_LIBBATCH_USER_ADMIN_MAIL_CONFIGURATION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil.h>
#include <sstream>
#include <iomanip>
#include "usermailconfiguration.h"

//--------------------------------------------------------------------------------------------------//
// namespace setcion
//--------------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{
    class UserAdminMailConfiguration
    {
        public:
            UserAdminMailConfiguration(libBatch::userAdmin::UserMailConfiguration& userMailConfiguration);
            ~UserAdminMailConfiguration();

            void configure();
            libBatch::userAdmin::UserMailConfiguration& getUserMailConfiguration();
            
        private:
            UserAdminMailConfiguration(const UserAdminMailConfiguration& r);
            UserAdminMailConfiguration& operator = (const UserAdminMailConfiguration& r);

            void logUserAdminMailConfiguration() const;
            void readMailRecipients();

            /////////////////////////////////
            // members
            /////////////////////////////////
            const log4cplus::Logger& m_Logger;
            libBatch::userAdmin::UserMailConfiguration   m_UserMailConfiguration;        
    };

} // end namespace userAdmin
} // end namespace libBatch

#endif // end GUARD_LIBBATCH_USER_ADMIN_MAIL_CONFIGURATION_H
