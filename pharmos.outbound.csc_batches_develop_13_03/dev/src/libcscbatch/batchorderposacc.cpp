//----------------------------------------------------------------------------
/*! \file	batchorderposacc.cpp
 *  \brief  string builder definition for order-accessor + registration of order-accessor
 *  \author	Beatrix Trömel
 *  \date   18.07.2008
 */
//----------------------------------------------------------------------------
#include "batchorderposacc.h"
#include <libbasarproperty_propertydescriptionlistref.h>

//----------------------------------------------------------------------------
namespace batchorderposacc {
	//-------------------------- SelBatchOrderPosByBranchNoAndOrderNo ------------------------------
	BUILDER_DEFINITION(SelBatchOrderPosByBranchNoAndOrderNo)
	//---------- isExecutable
	bool SelBatchOrderPosByBranchNoAndOrderNo::isExecutable() const  {
		try {		
			basar::ConstString fun = "SelBatchOrderPosByBranchNoAndOrderNo::isExecutable()";
			bool executable = false;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
			if(yit.arePropertiesSet("branchno;orderno;")) {
				executable = true;
			}
			if(!executable) {
				BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, "Nothing to be updated: "); 
				std::stringstream os;
				if(libcscbatch::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
					os.str(""); os << fun << ": "; yit.toStream(os);
					libcscbatch::LoggerPool::loggerDomModules.forcedLog(log4cplus::INFO_LOG_LEVEL, os.str().c_str(), __FILE__, __LINE__);
				}
			}// if(!executable)
			return executable;
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "batchorderposacc::SelBatchOrderPosByBranchNoAndOrderNo()."); 
			throw;
		}// catch
	}// SelBatchOrderPosByBranchNoAndOrderNo::isExecutable    
	//---------- buildSQLString
	void SelBatchOrderPosByBranchNoAndOrderNo::buildSQLString() {
		try {		
//! \todo: Kennzeichen füllen (VX13.KZ) für PWS Frankreich, PWS Schweiz, NO22 Flash-Aufträge, Ueberweiser
//! \todo: SA füllen (VX13.SA) für PWS Frankreich PWS-Partner, z.B. 79 für Ratiopharm

			// kes 05.09.13: new items for ABBA-UW: nonchargedqty, discountpct

			basar::I18nString sqlStatement = "";
			// Select
			basar::I18nString sqlStatementSelect = 
				"SELECT "
					"op.branchno as branchno, "
					"op.orderno as orderno, "
					"op.posno as position, "
					"op.orderedqty as orderedqty, "
					"op.customerorderno as customerorderno, "
					"op.customerposno as customerposno, "
					"ac.article_code as article_code, "
					"op.nonchargedqty as nonchargedqty, "
					"op.discountpct as discountpct, "
					"op.textfield as textfield, "
					"op.discountcalcfrom as discountcalcfrom, "
					"op.discountapplyto as discountapplyto, "
					"az.einheit as unit, "
					"az.artikel_name as articlename, "
					"az.darreichform as pharmaceuticalform, "
					"az.artikel_nr as pzn, "
					"ao.datumverfall as expirydate, "
					"ap.preisekapo as purchasepricepharmacy, "
					"op.batch as batch ";

// ao.lagerbereichnr für Herne
// artikelnatra.mengenatra as rebateinkind ?

			// From
			basar::I18nString sqlStatementFrom =
				"FROM "
					"orderpos op "
                    "LEFT JOIN articlecodes ac ON op.articleno = ac.articleno AND ac.preferred_flag = 1 "
					"LEFT OUTER JOIN artikelzentral az ON op.articleno=az.artikel_nr "
					"LEFT OUTER JOIN artikelpreis ap ON az.artikel_nr = ap.artikel_nr "
					"LEFT OUTER JOIN artikellagerort ao ON az.artikel_nr = ao.artikel_nr AND op.branchno = ao.filialnr ";
			
			// Where
			basar::I18nString sqlStatementWhere =
				"WHERE "
					"op.branchno=#branchno# AND "
					"op.orderno = '#orderno#' AND "
					"ap.preis_typ = 0 ";

			if(isContainedAndSet("fromposition")) {
				sqlStatementWhere.append("AND op.posno >= #fromposition# ");
			}

			// Group By
			basar::I18nString sqlStatementGroupBy = "";
	/*
				GROUP BY \
				";
	*/
			// Order By
			basar::I18nString sqlStatementOrderBy = "";
	/*
				ORDER BY \
				";
	*/
			sqlStatement.append(sqlStatementSelect);
			sqlStatement.append(sqlStatementFrom);
			sqlStatement.append(sqlStatementWhere);
			sqlStatement.append(sqlStatementGroupBy);
			sqlStatement.append(sqlStatementOrderBy);
			resolve(sqlStatement);
			BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "batchorderposacc::SelBatchOrderPosByBranchNoAndOrderNo()."); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// SelBatchOrderPosByBranchNoAndOrderNo

	//-------------------------- SaveBatchOrderPos ------------------------------
	BUILDER_DEFINITION(SaveBatchOrderPos)
	//---------- isExecutable
	bool SaveBatchOrderPos::isExecutable() const  {
		try {		
			basar::ConstString fun = "SaveBatchOrderPos::isExecutable()";
			bool executable = false;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
			if(yit.arePropertiesSet("branchno;orderno;position;")) {
				if(yit.arePropertiesSet("orderedqty;") || yit.arePropertiesSet("new_article_code;")) {
					executable = true;
				}			
			}
			if(!executable) {
				BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, "Nothing to be updated: "); 
				std::stringstream os;
				if(libcscbatch::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
					os.str(""); os << fun << ": "; yit.toStream(os);
					libcscbatch::LoggerPool::loggerDomModules.forcedLog(log4cplus::INFO_LOG_LEVEL, os.str().c_str(), __FILE__, __LINE__);
				}
			}// if(!executable)
			return executable;
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "batchorderposacc::SaveBatchOrderPos()."); 
			throw;
		}// catch
	}// SaveBatchOrderPos::isExecutable    
	//---------- buildSQLString
	void SaveBatchOrderPos::buildSQLString() {
		try {		
			basar::I18nString sqlStatement = "";
			// Update
			basar::I18nString sqlStatementUpdate = "UPDATE orderpos \
				";
			
			// Set
			bool empty=true;
			basar::I18nString sqlStatementSet = "";
			if(isContainedAndSet("orderedqty")) {
				if(empty)
					sqlStatementSet.append("SET ");
				else
					sqlStatementSet.append(", ");
				sqlStatementSet.append("orderedqty = #orderedqty# ");
				empty=false;
			}
			if(isContainedAndSet("pzn")) {
				if(empty)
					sqlStatementSet.append("SET ");
				else
					sqlStatementSet.append(", ");
				sqlStatementSet.append("articleno = #newpzn# ");
				empty=false;
			}

			// Where
			basar::I18nString sqlStatementWhere = "WHERE \
				branchno=#branchno# \
				AND orderno=#orderno# \
				AND posno=#position# \
				";

			sqlStatement.append(sqlStatementUpdate);
			sqlStatement.append(sqlStatementSet);
			sqlStatement.append(sqlStatementWhere);

			resolve(sqlStatement);
			BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "batchorderposacc::SaveBatchOrderPos()."); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// SaveBatchOrderPos

	//-------------------------- SelArticleByBranchNoAndArticleNo ------------------------------
	BUILDER_DEFINITION(SelArticleByBranchNoAndArticleNo)
	//---------- isExecutable
	bool SelArticleByBranchNoAndArticleNo::isExecutable() const  {
		try {		
			basar::ConstString fun = "SelArticleByBranchNoAndArticleNo::isExecutable()";
			bool executable = false;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
			if(yit.arePropertiesSet("branchno;newpzn;")) {
				executable = true;
			}

			if(!executable) {
				BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, "Not executable "); 
				std::stringstream os;
				if(libcscbatch::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
					os.str(""); os << fun << ": "; yit.toStream(os);
					libcscbatch::LoggerPool::loggerDomModules.forcedLog(log4cplus::INFO_LOG_LEVEL, os.str().c_str(), __FILE__, __LINE__);
				}
			}// if(!executable)
			return executable;
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "batchorderposacc::SelBatchOrderPosByBranchNoAndOrderNo()."); 
			throw;
		}// catch
	}// SelArticleByBranchNoAndArticleNo::isExecutable    

	//---------- buildSQLString
	void SelArticleByBranchNoAndArticleNo::buildSQLString() {
		try {		
			basar::I18nString sqlStatement = "";
			// Select
			basar::I18nString sqlStatementSelect = "SELECT \
				artikel_nr as pzn \
				";	

			// From
			basar::I18nString sqlStatementFrom = " \
				FROM artikellokal  \
				";
			
			// Where
			basar::I18nString sqlStatementWhere = "WHERE filialnr=#branchno# \
				AND artikel_nr = #newpzn# \
				";

			// Group By
			basar::I18nString sqlStatementGroupBy = "";

			// Order By
			basar::I18nString sqlStatementOrderBy = "";

			sqlStatement.append(sqlStatementSelect);
			sqlStatement.append(sqlStatementFrom);
			sqlStatement.append(sqlStatementWhere);
			sqlStatement.append(sqlStatementGroupBy);
			sqlStatement.append(sqlStatementOrderBy);
			resolve(sqlStatement);
			BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "batchorderposacc::SelArticleByBranchNoAndArticleNo()."); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// SelArticleByBranchNoAndArticleNo

	//-------------------------- SelPZNByArticleCode ------------------------------
	BUILDER_DEFINITION(SelPZNByArticleCode)
	//---------- isExecutable
	bool SelPZNByArticleCode::isExecutable() const  {
		try {		
			basar::ConstString fun = "SelPZNByArticleCode::isExecutable()";
			bool executable = false;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
			if(yit.arePropertiesSet("new_article_code;")) {
				executable = true;
			}

			if(!executable) {
				BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, "Not executable "); 
				std::stringstream os;
				if(libcscbatch::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
					os.str(""); os << fun << ": "; yit.toStream(os);
					libcscbatch::LoggerPool::loggerDomModules.forcedLog(log4cplus::INFO_LOG_LEVEL, os.str().c_str(), __FILE__, __LINE__);
				}
			}// if(!executable)
			return executable;
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "batchorderposacc::SelPZNByArticleCode()."); 
			throw;
		}// catch
	}// SelPZNByArticleCode::isExecutable    

	//---------- buildSQLString
	void SelPZNByArticleCode::buildSQLString() {
		try {		
			basar::I18nString sqlStatement = "";
			// Select
			basar::I18nString sqlStatementSelect = "SELECT \
				articleno as newpzn \
				";	

			// From
			basar::I18nString sqlStatementFrom = " \
				FROM articlecodes  \
				";
			
			// Where
			basar::I18nString sqlStatementWhere = "WHERE article_code='#new_article_code#' \
				";

			// Group By
			basar::I18nString sqlStatementGroupBy = "";

			// Order By
			basar::I18nString sqlStatementOrderBy = "";

			sqlStatement.append(sqlStatementSelect);
			sqlStatement.append(sqlStatementFrom);
			sqlStatement.append(sqlStatementWhere);
			sqlStatement.append(sqlStatementGroupBy);
			sqlStatement.append(sqlStatementOrderBy);
			resolve(sqlStatement);
			BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "batchorderposacc::SelPZNByArticleCode()."); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// SelPZNByArticleCode

	//------------------------------- SelBatchOrderPosAcc -----------------------------------
	//
	// Accessor-Definitions werden vom Manager registriert und  verwaltet.
	// Instanziierung notwendig!
	//
	// kes 05.09.13: new items for ABBA-UW: nonchargedqty, discountpct
	BEGIN_ACCESSOR_DEFINITION("SelBatchOrderPosAcc")
		PROPERTY_DESCRIPTION_LIST("\
			int16 branchno; \
			int32 orderno; \
			int32 position; \
			int32 orderedqty; \
			int32 customerorderno; \
			int32 customerposno; \
			string textfield; \
			int16 discountcalcfrom; \
			int16 discountapplyto; \
			string unit; \
			string articlename; \
			string pharmaceuticalform; \
			int32 pzn; \
			int32 newpzn; \
			decimal purchasepricepharmacy; \
			string article_code; \
			string new_article_code; \
			int32   nonchargedqty; \
			decimal discountpct; \
			string batch; \
			int32 expirydate; \
			")

		ACCESS_METHOD("SelBatchOrderPosByBranchNoAndOrderNo")
			SQL_BUILDER_CREATE(SelBatchOrderPosByBranchNoAndOrderNo)
			SQL_BUILDER_PUSH_BACK(SelBatchOrderPosByBranchNoAndOrderNo)
			
		ACCESS_METHOD("SaveBatchOrderPos")
			SQL_BUILDER_CREATE(SaveBatchOrderPos)
			SQL_BUILDER_PUSH_BACK(SaveBatchOrderPos)

		ACCESS_METHOD("SelArticleByBranchNoAndArticleNo")
			SQL_BUILDER_CREATE(SelArticleByBranchNoAndArticleNo)
			SQL_BUILDER_PUSH_BACK(SelArticleByBranchNoAndArticleNo)

		ACCESS_METHOD("SelPZNByArticleCode")
			SQL_BUILDER_CREATE(SelPZNByArticleCode)
			SQL_BUILDER_PUSH_BACK(SelPZNByArticleCode)
	END_ACCESSOR_DEFINITION			
}//batchorderposacc
