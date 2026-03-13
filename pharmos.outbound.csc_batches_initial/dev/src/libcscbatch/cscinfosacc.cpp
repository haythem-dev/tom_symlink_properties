#include "cscinfosacc.h"
#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertydescriptionlistref.h>

//----------------------------------------------------------------------------
namespace cscinfosacc {
	//************************************ SelOrderTypesByOrderNo ************************************************
	BUILDER_DEFINITION(SelOrderTypesByOrderNo)
	//************ isExecutable
		ENSURE_PROPERTIES_ARE_SET(SelOrderTypesByOrderNo, "branchno;orderno;ordertype;")
		
	//************ buildSQLString
	void SelOrderTypesByOrderNo::buildSQLString() {
		basar::I18nString sqlStatement = "";
		// Select
		basar::I18nString sqlStatementSelect = "SELECT \
			idfnr as pharmacyno \
			";
		// From
		basar::I18nString sqlStatementFrom = "\
			FROM auftragartkd ak \
			INNER JOIN orderhead oh ON ak.vertriebszentrumnr = oh.branchno AND ak.idfnr = oh.pharmacyno \
			";
		// Where
		basar::I18nString sqlStatementWhere = "WHERE \
			oh.branchno = #branchno# \
			AND oh.orderno = #orderno# \
			AND ak.kdauftragart = UPPER('#ordertype#') \
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
	}// SelOrderTypesByOrderNo

	//************************************ CscInfosAcc ************************************************
	//
	// Accessor-Definitions werden vom Manager registriert und  verwaltet.
	// Instanziierung notwendig!
	//
	//*******************************************************************************************************
	BEGIN_ACCESSOR_DEFINITION("CscInfosAcc")
		PROPERTY_DESCRIPTION_LIST("\
			int16 branchno; \
			int32 pharmacyno; \
			int32 orderno; \
			string ordertype; \
			int32 kscorderno; \
			")

		ACCESS_METHOD("SelOrderTypesByOrderNo")
			SQL_BUILDER_CREATE(SelOrderTypesByOrderNo)
			SQL_BUILDER_PUSH_BACK(SelOrderTypesByOrderNo)
END_ACCESSOR_DEFINITION			
}//namespace

