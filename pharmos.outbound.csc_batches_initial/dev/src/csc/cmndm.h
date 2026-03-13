#ifndef GUARD_IRM_CMNDM_H
#define GUARD_IRM_CMNDM_H

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
#include "loggerpool.h"
#include <misc/log_macros.h>
#include <macros.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarproperty_propertytable_xiterator.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_manager.h>

#endif	// GUARD_IRM_CMNDM_H


