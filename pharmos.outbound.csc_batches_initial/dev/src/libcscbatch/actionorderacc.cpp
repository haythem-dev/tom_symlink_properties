//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "actionorderacc.h"
#include <libbasarproperty_propertydescriptionlistref.h>
#include <libbasardbaspect.h>

namespace actionorderacc {
	//************************************ Dummy ************************************************
	BUILDER_DEFINITION(Dummy)
		DONT_CHECK_PROPERTIES(Dummy)
		void Dummy::buildSQLString(){}

	//------------------------------- ActionBatchOrdersAcc -----------------------------------
	//
	// Accessor-Definitions werden vom Manager registriert und  verwaltet.
	// Instanziierung notwendig!
	//
	BEGIN_ACCESSOR_DEFINITION("ActionBatchOrdersAcc")
		PROPERTY_DESCRIPTION_LIST("\
			string action_select; \
			string action_function; \
			string action_orderno; \
			string ordernofrom; \
			string ordernoto; \
			string action_status; \
			string action_datasource; \
			string action_deliveryrun; \
			string edideliverydate; \
			string pharmacyrebooking; \
			string ordertype; \
			")

	ACCESS_METHOD("Dummy")
		SQL_BUILDER_CREATE(Dummy)
		SQL_BUILDER_PUSH_BACK(Dummy)

	END_ACCESSOR_DEFINITION		
}//actionorderacc

