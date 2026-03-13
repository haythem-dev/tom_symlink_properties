//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       27.12.2010
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_USER_ADMIN_CONNECTION_PARAMETER_CHECKER_H
#define GUARD_USER_ADMIN_CONNECTION_PARAMETER_CHECKER_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil.h>
#include "libbatch_definitions.h"

//--------------------------------------------------------------------------------------------------//
// namespace setcion
//--------------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{

class ConnectionParameterChecker
{   
public:
    ConnectionParameterChecker( const basar::VarString& country );
    ~ConnectionParameterChecker();

private:
	ConnectionParameterChecker( const ConnectionParameterChecker& r );
	ConnectionParameterChecker& operator = ( const ConnectionParameterChecker& r );

};

} // end namespace userAdmin
} // end namespace libBatch

#endif // end GUARD_USER_ADMIN_CONNECTION_PARAMETER_CHECKER_H
