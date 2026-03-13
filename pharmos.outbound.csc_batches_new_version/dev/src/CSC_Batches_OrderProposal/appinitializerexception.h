//--------------------------------------------------------------------------------------------------//
/*! \file		
 *  \brief      
 *  \author     
 *  \date      
 *  \version    
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_APP_INITIALIZER_EXCEPTION_H
#define GUARD_ABBAUW_APP_INITIALIZER_EXCEPTION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil_exceptions.h>

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{

class AppInitializerException : public basar::Exception
{
public:
	AppInitializerException( const basar::ExceptInfo& sInfo );
	~AppInitializerException();

private:
	AppInitializerException();					
    AppInitializerException& operator=( AppInitializerException& );
};

} 

#endif 

