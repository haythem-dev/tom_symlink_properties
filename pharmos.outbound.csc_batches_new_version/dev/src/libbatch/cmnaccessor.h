//------------------------------------------------------------------------------
/*! \file
 *  \brief  common header files include for accessors
 *       \n contains a collection of stable(!) header files
 *       \n collection can be extended if necessary
 *  \author Michael Eichenlaub
 *  \date   23.01.2008
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBIVC_CMNACCESSOR_H
#define GUARD_LIBIVC_CMNACCESSOR_H

//------------------------------------------------------------------------------
// displays stage of precompiling, so not fulfilled precompile conditions can be detected
#ifdef WIN32
	#ifndef PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
		#pragma message("    compiling " __FILE__ " ...")
		#define PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
	#endif
#endif

//------------------------------------------------------------------------------
#include <libbasardbaspect.h>

//------------------------------------------------------------------------------
#endif	// GUARD_LIBIVC_CMNACCESSOR_H

