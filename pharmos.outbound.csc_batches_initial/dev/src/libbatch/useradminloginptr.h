//--------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      declaration of abstract connection base class
 *  \author     Bischof Bjoern
 *  \date       27.01.2009
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_USERADMIN_LOGIN_PTR_H
#define GUARD_LIBBATCH_USERADMIN_LOGIN_PTR_H

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
        class UserAdminLogin;
        typedef boost::shared_ptr<libBatch::userAdmin::UserAdminLogin> UserAdminLoginPtr;
    }
}

#endif // end GUARD_LIBBATCH_USERADMIN_LOGIN_PTR_H
