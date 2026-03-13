//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Ivan Fildishev
 *  \date       17.11.2023
 *  \version    00.00.00.01 first version
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_USERADMIN_MAIL_CONFIGURATION_PTR_H
#define GUARD_LIBBATCH_USERADMIN_MAIL_CONFIGURATION_PTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace libBatch
{
    namespace userAdmin
    {
        class UserAdminMailConfiguration;
        typedef boost::shared_ptr<libBatch::userAdmin::UserAdminMailConfiguration> UserAdminMailConfigurationPtr;
    }
}

#endif // end GUARD_LIBBATCH_USERADMIN_MAIL_CONFIGURATION_PTR_H
