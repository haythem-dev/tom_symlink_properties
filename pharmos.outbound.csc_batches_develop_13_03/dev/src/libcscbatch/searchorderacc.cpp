//-------------------------------------------------------------------------------------------------//
/*! \file	searchorderacc.cpp
 *  \brief  accessor for searching batch orders
 *  \author Beatrix Trömel
 *  \date   11.06.2008
 */
//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "searchorderacc.h"

#include <libbasardbaspect.h>
#include <libbasarproperty_propertydescriptionlistref.h>

namespace searchorderacc {
	//************************************ Dummy ************************************************
	BUILDER_DEFINITION(Dummy)
		DONT_CHECK_PROPERTIES(Dummy)
		void Dummy::buildSQLString(){}

	//------------------------------- SearchBatchOrdersAcc -----------------------------------
	//
	// Accessor-Definitions werden vom Manager registriert und  verwaltet.
	// Instanziierung notwendig!
	//
	BEGIN_ACCESSOR_DEFINITION("SearchBatchOrdersAcc")
		PROPERTY_DESCRIPTION_LIST("\
			int16 branchno; \
			int32 ordernofrom; \
			int32 ordernoto; \
			string status; \
			string datasourcetype; \
			int32 pharmacyno; \
			int32 favoredduedate; \
			int32 manufacturerno; \
			string salesman; \
			string department; \
			string datasourcetype_branch; \
			int16 weekday; \
			")

	ACCESS_METHOD("Dummy")
		SQL_BUILDER_CREATE(Dummy)
		SQL_BUILDER_PUSH_BACK(Dummy)

	END_ACCESSOR_DEFINITION			
}//searchorderacc

