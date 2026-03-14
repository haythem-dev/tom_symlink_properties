//--------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      declaration of abstract connection base class
 *  \author     Bischof Bjoern
 *  \date       27.01.2009
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_USERADMIN_DB_CONFIGURATION_PTR_H
#define GUARD_LIBBATCH_USERADMIN_DB_CONFIGURATION_PTR_H

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
        class UserAdminDBConfiguration;
        typedef boost::shared_ptr<libBatch::userAdmin::UserAdminDBConfiguration> UserAdminDBConfigurationPtr;
    }
}

#endif // end GUARD_LIBBATCH_USERADMIN_DB_CONFIGURATION_PTR_H
