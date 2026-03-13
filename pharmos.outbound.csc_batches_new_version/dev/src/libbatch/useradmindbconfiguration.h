//-----------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      declaration of database login class
 *  \author     Bischof Bjoern
 *  \date       31.12.2010
 *  \version    00.00.00.01 first version
 */ 
//-----------------------------------------------------------------------------------------------//
#ifndef GUARD_LIBBATCH_USER_ADMIN_DB_CONFIGURATION_H
#define GUARD_LIBBATCH_USER_ADMIN_DB_CONFIGURATION_H

//-----------------------------------------------------------------------------------------------//
// include section
//-----------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include <libbasarcmnutil.h>
#include "dbconnectionconfiguration.h"

//-----------------------------------------------------------------------------------------------//
// class declaration section
//-----------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{
    class UserAdminDBConfiguration
    {
        public:
            UserAdminDBConfiguration( libBatch::dbConnection::DBConnectionConfiguration& dbConnectionConfiguration );
	        ~UserAdminDBConfiguration();

            void configure();
            const libBatch::dbConnection::DBConnectionConfiguration& getDBConnectionConfiguration() const;

        private:
	        UserAdminDBConfiguration( const UserAdminDBConfiguration& r );
	        UserAdminDBConfiguration& operator = ( const UserAdminDBConfiguration& r );    

            void logDBConfiguration() const;

            /////////////////////////////////
            // members
            /////////////////////////////////
            const log4cplus::Logger&                            m_Logger;
            libBatch::dbConnection::DBConnectionConfiguration   m_DBConnectionConfiguration;
    };

} // end namespace userAdmin
} // end namespace libBatch

#endif // end GUARD_LIBBATCH_USER_ADMIN_DB_CONFIGURATION_H
