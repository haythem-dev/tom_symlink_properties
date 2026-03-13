#include "cmnaccessor.h"

#include "parameteracc.h"
#include "parameter_definitions.h"
#include <libbasarcmnutil_logging.h>
#include <libbasarcmnutil_datetime.h>

namespace libabbauw
{
namespace acc_parameter
{
	using namespace libabbauw::properties;
	using basar::VarString;

BEGIN_ACCESSOR_DEFINITION( domMod::parameter::ACC_PARAMETER )
	
	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO                                  );
		PROPERTY_DESCRIPTION_LIST_ADD( PROGNAME                                  );
		PROPERTY_DESCRIPTION_LIST_ADD( PURPOSE                                   );
		PROPERTY_DESCRIPTION_LIST_ADD( PARAMETERNAME                             );
		PROPERTY_DESCRIPTION_LIST_ADD( MANIFESTATION                             );
		PROPERTY_DESCRIPTION_LIST_ADD( PARAMETERVALUE                            );
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( domMod::parameter::SELECT_PARAMETER )
		SQL_BUILDER_CREATE( SelectParameter )
		SQL_BUILDER_PUSH_BACK( SelectParameter )

	ACCESS_METHOD( domMod::parameter::SAVE_PARAMETER )
		SQL_BUILDER_CREATE(    UpdateParameter              )
		SQL_BUILDER_PUSH_BACK( UpdateParameter              )
		SQL_BUILDER_CREATE(    InsertParameterValue         )
		SQL_BUILDER_PUSH_BACK( InsertParameterValue         )
		SQL_BUILDER_CREATE(    InsertParameterManifestation )
		SQL_BUILDER_PUSH_BACK( InsertParameterManifestation )

END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectParameter )

	ENSURE_PROPERTIES_ARE_SET( SelectParameter, BRANCHNO.toPropertyString() + PROGNAME.toPropertyString() )

	void SelectParameter::buildSQLString()
	{
		static const VarString sqlStatement(
			"SELECT zweck AS "                                                               + PURPOSE.getName()                        +
			", parametername AS "                                                            + PARAMETERNAME.getName()                  +
			", auspraegung AS "                                                              + MANIFESTATION.getName()                  +
			", wert AS "                                                                     + PARAMETERVALUE.getName()                 +
			" FROM parameter WHERE filialnr = "                                              + BRANCHNO.toSQLReplacementString()        +
			" AND programmname = '"                                                          + PROGNAME.toSQLReplacementString()        +
			"'"
		);

		resolve( sqlStatement );
		BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerParameter );
	}

BUILDER_DEFINITION( InsertParameterValue )

	ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( InsertParameterValue,  
		PROGNAME.toPropertyString()      + BRANCHNO.toPropertyString()      + PURPOSE.toPropertyString()       +
		PARAMETERNAME.toPropertyString() + PARAMETERVALUE.toPropertyString()                                   ,
		PROGNAME.toPropertyString()      + BRANCHNO.toPropertyString()      + PURPOSE.toPropertyString()       +
		PARAMETERNAME.toPropertyString() + PARAMETERVALUE.toPropertyString()                                   ,
		basar::SS_INSERT
	)

	void InsertParameterValue::buildSQLString()
	{
		static const VarString sqlStatement(
			"INSERT INTO parameter"
			"(filialnr, programmname, zweck, parametername, wert) "
			"VALUES("                                                                          +
			BRANCHNO.toSQLReplacementString()                                        + ", '"   +
			PROGNAME.toSQLReplacementString()                                        + "', '"  +
			PURPOSE.toSQLReplacementString()                                         + "', '"  +
			PARAMETERNAME.toSQLReplacementString()                                   + "', "   +
			PARAMETERVALUE.toSQLReplacementString()                                            +
			")"
		);

	    resolve( sqlStatement );
		BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerParameter );
	}

BUILDER_DEFINITION( InsertParameterManifestation )

	ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( InsertParameterManifestation,  
		PROGNAME.toPropertyString()      + BRANCHNO.toPropertyString()      + PURPOSE.toPropertyString()       +
		PARAMETERNAME.toPropertyString() + MANIFESTATION.toPropertyString()                                    ,
		PROGNAME.toPropertyString()      + BRANCHNO.toPropertyString()      + PURPOSE.toPropertyString()       +
		PARAMETERNAME.toPropertyString() + MANIFESTATION.toPropertyString()                                    ,
		basar::SS_INSERT
	)

	void InsertParameterManifestation::buildSQLString()
	{
		static const VarString sqlStatement(
			"INSERT INTO parameter"
			"(filialnr, programmname, zweck, parametername, auspraegung) "
			"VALUES("                                                                          +
			BRANCHNO.toSQLReplacementString()                                        + ", '"   +
			PROGNAME.toSQLReplacementString()                                        + "', '"  +
			PURPOSE.toSQLReplacementString()                                         + "', '"  +
			PARAMETERNAME.toSQLReplacementString()                                   + "', '"  +
			MANIFESTATION.toSQLReplacementString()                                   + "')" 
		);

	    resolve( sqlStatement );
		BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerParameter );
	}

BUILDER_DEFINITION( UpdateParameter )

	ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( UpdateParameter,  
		PROGNAME.toPropertyString()       + BRANCHNO.toPropertyString()      + PARAMETERNAME.toPropertyString() ,
		PARAMETERVALUE.toPropertyString() + MANIFESTATION.toPropertyString()                                    ,
		basar::SS_UPDATE
	)

	void UpdateParameter::buildSQLString()
	{
		static const VarString sqlStatement(
			"UPDATE parameter SET wert = "                                                   + PARAMETERVALUE.toSQLReplacementString()  +
			", auspraegung = '"                                                              + MANIFESTATION.toSQLReplacementString()   +
			"' WHERE filialnr = "                                                            + BRANCHNO.toSQLReplacementString()        +
			" AND programmname = '"                                                          + PROGNAME.toSQLReplacementString()        +
			"' AND parametername = '"                                                        + PARAMETERNAME.toSQLReplacementString()   +
			"'"
		);

	    resolve( sqlStatement );
		BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerParameter );
	}

} // end namespace acc_parameter
} // end namespace libabbauw
