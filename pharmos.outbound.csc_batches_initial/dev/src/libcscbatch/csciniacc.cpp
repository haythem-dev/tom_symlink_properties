//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "csciniacc.h"
#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertydescriptionlistref.h>
#include "libcscbatch_loggerpool.h"

#define KEY_ENTEREDBY "entered by"

//**************************************************************************************************
// using declarations
//**************************************************************************************************

namespace csciniacc {
	//************************************ SelEnterByBranch ************************************************
	BUILDER_DEFINITION(SelEnterByBranch)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(SelEnterByBranch, "branchno;")
		
	//************ buildSQLString
	void SelEnterByBranch::buildSQLString() {
		basar::I18nString sqlStatement = "";
		basar::I18nString sqlStatementSnippet = "";

		// Select
		basar::I18nString sqlStatementSelect = "SELECT ci.user ";

		// From
		basar::I18nString sqlStatementFrom = "\
			FROM cscini ci \
			";

		// Where
		basar::I18nString sqlStatementWhere = "";
		sqlStatementWhere = "\
				WHERE \
				ci.branchno=#branchno# \
				";
		sqlStatementSnippet.format("AND ci.key='%s'", KEY_ENTEREDBY);
		sqlStatementWhere.append(sqlStatementSnippet);

		sqlStatement.append(sqlStatementSelect);
		sqlStatement.append(sqlStatementFrom);
		sqlStatement.append(sqlStatementWhere);
		resolve(sqlStatement);
		BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
	}// SelEnterByBranch

	//************************************ SaveEnterByBranch ************************************************
	BUILDER_DEFINITION(SaveEnterByBranch)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(SaveEnterByBranch, "branchno;user;")

	//************ buildSQLString
	void SaveEnterByBranch::buildSQLString() {
		try {
			basar::I18nString sqlStatement = "";
			basar::I18nString sqlStatementSnippet = "";

			// Insert
			sqlStatement="insert into cscini (branchno, key, value, user) values(#branchno#,";
			sqlStatementSnippet.format("'%s'", KEY_ENTEREDBY);
			sqlStatement.append(sqlStatementSnippet);
			sqlStatement.append(",'','#user#')");

			resolve(sqlStatement);
			BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "csciniacc::SaveEnterByBranch()."); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// SaveEnterByBranch

	//************************************ DeleteEnterByBranch ************************************************
	BUILDER_DEFINITION(DeleteEnterByBranch)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(DeleteEnterByBranch, "branchno;user;")

	//************ buildSQLString
	void DeleteEnterByBranch::buildSQLString() {
		try {
			basar::I18nString sqlStatement = "";
			basar::I18nString sqlStatementSnippet = "";

			// Delete
			sqlStatement="DELETE FROM cscini WHERE branchno=#branchno# AND key=";
			sqlStatementSnippet.format("'%s'", KEY_ENTEREDBY);
			sqlStatement.append(sqlStatementSnippet);
			sqlStatement.append(" AND cscini.user='#user#'");  // table name MUST proceed column name because "user" is a key word!!!

			resolve(sqlStatement);
			BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "csciniacc::DeleteEnterByBranch()."); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// DeleteEnterByBranch

	//************************************ csciniacc ************************************************
	//
	// Accessor-Definitions werden vom Manager registriert und  verwaltet.
	// Instanziierung notwendig!
	//
	//*******************************************************************************************************
	BEGIN_ACCESSOR_DEFINITION("csciniacc")
		PROPERTY_DESCRIPTION_LIST("\
			int16 branchno; \
			string key; \
			string value; \
			string user; \
			")

		ACCESS_METHOD("SelEnterByBranch")
			SQL_BUILDER_CREATE(SelEnterByBranch)
			SQL_BUILDER_PUSH_BACK(SelEnterByBranch)

		ACCESS_METHOD("SaveEnterByBranch")
			SQL_BUILDER_CREATE(SaveEnterByBranch)
			SQL_BUILDER_PUSH_BACK(SaveEnterByBranch)

		ACCESS_METHOD("DeleteEnterByBranch")
			SQL_BUILDER_CREATE(DeleteEnterByBranch)
			SQL_BUILDER_PUSH_BACK(DeleteEnterByBranch)
END_ACCESSOR_DEFINITION			
}//csciniacc

