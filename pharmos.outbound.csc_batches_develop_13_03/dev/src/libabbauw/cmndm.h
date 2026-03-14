#ifndef GUARD_LIBABBAUW_CMNDM_H
#define GUARD_LIBABBAUW_CMNDM_H

//------------------------------------------------------------------------------
// displays stage of precompiling, so not fulfilled precompile conditions can be detected
#ifdef WIN32
	#ifndef PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
		#pragma message("    compiling " __FILE__ " ...")
		#define PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
	#endif
#endif


#include <libbasardbaspect.h>
#include <libbasarproperty.h>
#include <libbasardbsql_connectionref.h>
#include <libutil/infrastructure/db/dbtransaction/transactionguard.h>
#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libutil/misc/log_macro_definitions.h>
#include <libutil/classinfo.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarproperty_propertytable_xiterator.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_manager.h>
#include "libabbauw_definitions.h"
#include "libabbauw_properties_definitions.h"

#endif	// GUARD_LIBABBAUW_CMNDM_H
