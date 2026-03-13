//-----------------------------------------------------------------------------------------------//
/*! \file commandlineexception.cpp
 *  \brief exception from parsing the command line
 *  \author Dirk Kapusta
 *  \date 21.10.2008
 *  \version 00.00.00.01 first version
 */ 
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// include sectioon
//-----------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include "useradminfailureexception.h"

//-----------------------------------------------------------------------------------------------//
// namespace sectioon
//-----------------------------------------------------------------------------------------------//
namespace libBatch  
{
namespace userAdmin
{

UserAdminFailureException::UserAdminFailureException( const basar::ExceptInfo& sInfo )
: BasarException( sInfo, "UserAdminFailureException" )
{
}

UserAdminFailureException::~UserAdminFailureException()
{
}

} // end namespace userAdmin
} // end namespace libBatch
