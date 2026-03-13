//--------------------------------------------------------------------------------------------------//
/*! \file		appinitializerexception.cpp
 *  \brief		exception indicating error with initialization of the app
 *  \author     Johann Franz
 *  \date       09.08.2012
 *  \version 00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
//#include "cmnexcept.h"
#include <libbasar_definitions.h>
#include "appinitializerexception.h"

//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{

AppInitializerException::AppInitializerException( const basar::ExceptInfo& sInfo )
: BasarException( sInfo, "AppInitializerException" )
{
}

AppInitializerException::~AppInitializerException()
{
}

}
