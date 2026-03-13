#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_COMMON_ACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_COMMON_ACC_H

// displays stage of precompiling, so not fulfilled precompile conditions can be detected
#ifdef WIN32
	#ifndef PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
		#pragma message("    compiling " __FILE__ " ...")
		#define PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
	#endif
#endif

#include <libbasardbaspect.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasarproperty.h>
#include <libutil/accessor.h>
#include <libutil/util.h>
#include <loggerpool/tenderloggerpool.h>
#include <domainmodule/libtender_properties_definitions.h>

#endif // GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_COMMON_ACC_H

