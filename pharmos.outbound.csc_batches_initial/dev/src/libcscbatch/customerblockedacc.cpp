#include "customerblockedacc.h"
#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertydescriptionlistref.h>

//----------------------------------------------------------------------------
namespace customerblockedacc {
	//************************************ SelCustomerBlocked ************************************************
	BUILDER_DEFINITION(SelCustomerBlocked)
	//************ isExecutable
		ENSURE_PROPERTIES_ARE_SET(SelCustomerBlocked, "branchno;pharmacyno;")
		
	//************ buildSQLString
	void SelCustomerBlocked::buildSQLString() {
		basar::I18nString sqlStatement = "";
		// Select
		basar::I18nString sqlStatementSelect = "SELECT \
			skdkundeaegesperrt AS blocked \
			";
		// From
		basar::I18nString sqlStatementFrom = " FROM \
			kundeschalter \
			";
		// Where
		basar::I18nString sqlStatementWhere = "WHERE \
			vertriebszentrumnr = #branchno# \
			AND idfnr = #pharmacyno# \
			";
		// Group By
		basar::I18nString sqlStatementGroupBy = "\
			";

		// Order By
		basar::I18nString sqlStatementOrderBy = "\
			";
		sqlStatement.append(sqlStatementSelect);
		sqlStatement.append(sqlStatementFrom);
		sqlStatement.append(sqlStatementWhere);
		sqlStatement.append(sqlStatementGroupBy);
		sqlStatement.append(sqlStatementOrderBy);
		resolve(sqlStatement);
		BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
	}// SelCustomerBlocked

	//************************************ CustomerBlockedAcc ************************************************
	//
	// Accessor-Definitions werden vom Manager registriert und  verwaltet.
	// Instanziierung notwendig!
	//
	//*******************************************************************************************************
	BEGIN_ACCESSOR_DEFINITION("CustomerBlockedAcc")
		PROPERTY_DESCRIPTION_LIST("\
			int16 branchno; \
			int32 pharmacyno; \
			string blocked; \
			")

		ACCESS_METHOD("SelCustomerBlocked")
			SQL_BUILDER_CREATE(SelCustomerBlocked)
			SQL_BUILDER_PUSH_BACK(SelCustomerBlocked)
END_ACCESSOR_DEFINITION			
}//namespace

