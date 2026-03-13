//-----------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       27.10.2008
 *  \version    00.00.00.01 first version
 */ 
//-----------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_USER_ADMIN_FAILURE_EXCEPTION_H
#define GUARD_LIBBATCH_USER_ADMIN_FAILURE_EXCEPTION_H

//-----------------------------------------------------------------------------------------------//
// include section
//-----------------------------------------------------------------------------------------------//
#include <libbasarcmnutil_exceptions.h>

//-----------------------------------------------------------------------------------------------//
// class declaration section
//-----------------------------------------------------------------------------------------------//
namespace libBatch  
{
namespace userAdmin
{
    class UserAdminFailureException : public basar::Exception
    {
    public:
	    /*! \brief std constructor */
	    UserAdminFailureException( const basar::ExceptInfo& sInfo );
	    /*! \brief std-destructor */
	    ~UserAdminFailureException();

    private:
	    //!< forbidden
	    UserAdminFailureException();					
    };
} // end namespace userAdmin
} // end namespace libBatch  

#endif // GUARD_LIBBATCH_USER_ADMIN_FAILURE_EXCEPTION_H

