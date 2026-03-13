//------------------------------------------------------------------------------
/*! \file
 *  \brief  common header files include for exceptions
 *       \n contains a collection of stable(!) header files
 *       \n collection can be extended if necessary
 *  \author Michael Eichenlaub
 *  \date   25.01.2008
 */
//------------------------------------------------------------------------------

#ifndef GUARD_IVC_CMNEXCEPT_H
#define GUARD_IVC_CMNEXCEPT_H

//------------------------------------------------------------------------------
// displays stage of precompiling, so not fulfilled precompile conditions can be detected
#ifdef WIN32
	#ifndef PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
		#pragma message("    compiling " __FILE__ " ...")
		#define PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
	#endif
#endif

//------------------------------------------------------------------------------
#include <vector>
#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

//------------------------------------------------------------------------------
#endif	// GUARD


