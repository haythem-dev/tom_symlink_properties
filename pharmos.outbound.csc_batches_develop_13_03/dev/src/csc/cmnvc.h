//------------------------------------------------------------------------------
/*! \file
 *  \brief  common header files include for view connectors
 *       \n contains a collection of stable(!) header files
 *       \n collection can be extended if necessary
 *  \author Michael Eichenlaub
 *  \date   23.01.2008
 */
//------------------------------------------------------------------------------

#ifndef GUARD_CMNVC_H
#define GUARD_CMNVC_H   

//-------------------------------------------------------------------------------------
// define macros section
//-------------------------------------------------------------------------------------
// displays stage of precompiling, so not fulfilled precompile conditions can be detected
#ifdef WIN32
	#ifndef PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
		#pragma message("    compiling " __FILE__ " ...")
		#define PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
	#endif
#endif

//-------------------------------------------------------------------------------------
// include section
//-------------------------------------------------------------------------------------
#include "waitcursor.h"
#include <libutil/viewconn.h>

//------------------------------------------------------------------------------

#endif	// GUARD_CMNVC_H
