//------------------------------------------------------------------------------
/*! \file
 *  \brief  common header files include for accessors
 *       \n contains a collection of stable(!) header files
 *       \n collection can be extended if necessary
 *  \author Michael Eichenlaub
 *  \date   23.01.2008
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBABBAUW_CMNACCESSOR_H
#define GUARD_LIBABBAUW_CMNACCESSOR_H

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
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasarproperty_propertydescriptionlistref.h>
#include <libbasarproperty.h>
#include <libbasar_definitions.h>
#include <libbasarcmnutil_parameterlist.h>
#include <libutil/misc/log_macro_definitions.h>
#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_definitions.h"
#include "libabbauw_properties_definitions.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include "accessor_datetypes_common.h"

//------------------------------------------------------------------------------
#endif	// GUARD_LIBABBAUW_CMNACCESSOR_H
