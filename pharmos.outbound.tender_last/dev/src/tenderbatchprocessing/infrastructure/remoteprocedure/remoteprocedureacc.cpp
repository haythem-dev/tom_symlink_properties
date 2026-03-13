#include "remoteprocedureacc.h"
#include "remoteprocedureacc_definitions.h"
#include <tenderbatchprocessing_property_definitions.h>
#include <loggerpool/loggerpool.h>

namespace tenderbatchprocessing
{
namespace infrastructure 
{
namespace accessor 
{
namespace remoteProcedure 
{
BEGIN_ACCESSOR_DEFINITION( lit::REMOTE_PROCEDURE_ACC )

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PROCEDURE_NAME );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PROCEDURE_DATA );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PROCEDURE_RESULT );		
	END_PROPERTY_DESCRIPTION_LIST

    ACCESS_METHOD( lit::CALL_REMOTE_PROCEDURE );
		SQL_BUILDER_CREATE( CallRemoteProcedure )
		SQL_BUILDER_PUSH_BACK( CallRemoteProcedure )

END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( CallRemoteProcedure )

//ENSURE_PROPERTIES_ARE_SET( CallRemoteProcedure, 
//	properties::PROCEDURE_NAME.toPropertyString() +
//	properties::PROCEDURE_DATA.toPropertyString()
//		)
//


bool CallRemoteProcedure::isExecutable() const
{
	BLOG_TRACE( tenderbatchprocessing::LoggerPool::getLoggerDomModules(), getPropertyList().getString( properties::PROCEDURE_NAME ).c_str() );
	BLOG_TRACE( tenderbatchprocessing::LoggerPool::getLoggerDomModules(), getPropertyList().getString( properties::PROCEDURE_DATA ).c_str() );
    return true;
}

void CallRemoteProcedure::buildSQLString()
{
	static basar::VarString sql(
		"CALL STP.TCASTP ('" + 
		properties::PROCEDURE_NAME.toSQLReplacementString() + "', '" + 
		properties::PROCEDURE_DATA.toSQLReplacementString() + "'); "
			);

	resolve( sql );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

} //end namespace omgtransfer
} //end namespace accessor
} //end namespace infrastructure
}