//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "batchorderacc.h"
#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertydescriptionlistref.h>
#include "libcscbatch_loggerpool.h"

//**************************************************************************************************
// using declarations
//**************************************************************************************************
//using CSC_Batches::CSC_Batches::LoggerPool;

namespace batchorderacc {
	//************************************ SelBatchOrdersByPattern ************************************************
	BUILDER_DEFINITION(SelBatchOrdersByPattern)
	//************ isExecutable
		ENSURE_PROPERTIES_ARE_SET(SelBatchOrdersByPattern, "branchno;")
		
	//************ buildSQLString
	void SelBatchOrdersByPattern::buildSQLString() {
//! \todo: Kennzeichen füllen (VX13.KZ) für PWS Frankreich, PWS Schweiz, NO22 Flash-Aufträge, Ueberweiser
//! \todo: SA füllen (VX13.SA) für PWS Frankreich PWS-Partner, z.B. 79 für Ratiopharm
		basar::I18nString sqlStatement = "";

		// Select
		// orderby_FIELDs are only selected because they are needed for the ORDER BY part of the SELECT statement
		// sqlStatementSelect too long, therefore 2 parts
		basar::I18nString sqlStatementSelect1 = "";
		basar::I18nString sqlStatementSelect2 = "";
		basar::I18nString sqlStatementSelectCount = "";

		sqlStatementSelect1.append("SELECT ");
		sqlStatementSelect1.append("oh.branchno as branchno ");
		sqlStatementSelect1.append(",oh.orderno as orderno ");
		sqlStatementSelect1.append(",oh.status as orderby_status ");
		sqlStatementSelect1.append(",CASE WHEN oh.status='0' THEN 'ZU' WHEN oh.status='1' THEN 'FE' WHEN oh.status='2' THEN 'ER' WHEN oh.status='9' THEN 'ST' ELSE '?' END as status ");
		sqlStatementSelect1.append(",oh.ordertype || oh.orderpickingtype || oh.bookentrytype as ordertype ");
		sqlStatementSelect1.append(",Substring(cast(oh.orderdate as char(8)) from 7 for 2) || '.' || Substring(cast(oh.orderdate as char(8)) from 5 for 2) || '.' || Substring(cast(oh.orderdate as char(8)) from 1 for 4)as orderdate ");
		sqlStatementSelect1.append(",oh.orderdate as orderby_orderdate ");
		sqlStatementSelect1.append(",oh.ordervalue as ordervalue ");
		sqlStatementSelect1.append(",oh.pharmacyno as pharmacyno ");
		sqlStatementSelect1.append(",oh.headertext as headertext ");
		sqlStatementSelect1.append(",CASE WHEN oh.DeferredPaymentDate=0 THEN '' ELSE Substring(cast(oh.deferredpaymentdate as char(8)) from 7 for 2) || '.' || Substring(cast(oh.deferredpaymentdate as char(8)) from 5 for 2) || '.' || Substring(cast(oh.deferredpaymentdate as char(8)) from 1 for 4) END as deferredpaymentdate ");
		sqlStatementSelect1.append(",Substring(cast(oh.orderacquisitiondate as char(8)) from 7 for 2) || '.' || Substring(cast(oh.orderacquisitiondate as char(8)) from 5 for 2) || '.' || Substring(cast(oh.orderacquisitiondate as char(8)) from 1 for 4) as orderacquisitiondate ");
		sqlStatementSelect1.append(",oh.datasourcetype as datasourcetype ");
		sqlStatementSelect1.append(",oh.branchnoorig as branchnoorig ");
		sqlStatementSelect1.append(",oh.EDIDeliveryDate as orderby_edideliverydate ");
		sqlStatementSelect1.append(",CASE WHEN oh.EDIDeliveryDate=0 THEN '' ELSE Substring(cast(oh.EDIDeliveryDate as char(8)) from 7 for 2) || '.' || Substring(cast(oh.EDIDeliveryDate as char(8)) from 5 for 2) || '.' || Substring(cast(oh.EDIDeliveryDate as char(8)) from 1 for 4) END as edideliverydate ");
		sqlStatementSelect1.append(",oh.purchaseorderno as purchaseorderno ");

		sqlStatementSelect2.append(",MAX(az.hersteller_nr) as manufacturerno ");
		sqlStatementSelect2.append(",MAX(NVL(cu.matchcode,'UNKNOWN')) as pharmacyname ");
		sqlStatementSelect2.append(",MAX(NVL(cu.nameinhaber,'')) as nameowner ");
		sqlStatementSelect2.append(",MAX(NVL(cu.strasse,'')) as street ");
		sqlStatementSelect2.append(",MAX(NVL(cu.postleitzahl,'')) as zipcode ");
		sqlStatementSelect2.append(",MAX(NVL(cu.ort,'')) as city ");
		sqlStatementSelect2.append(",MAX(NVL(cu.telnr,'')) as telno ");
		sqlStatementSelect2.append(",MAX(NVL(cu.telkurzwahl,'')) as quickdialno ");
		sqlStatementSelect2.append(",MAX(NVL(cu.faxnr,'')) as faxno ");
		sqlStatementSelect2.append(",(SELECT count(op.posno) FROM orderpos op WHERE op.branchno = oh.branchno and op.orderNo=oh.OrderNo)::INT as posqty ");
		sqlStatementSelect2.append(",(count(op.articleno) - count(az.artikel_nr))::INT as countunknown ");
		sqlStatementSelect2.append(",MAX(NVL(cs.activ,'')) as uw_active ");
		sqlStatementSelect2.append(",oh.pharmacyno as book_pharmacyno ");

		sqlStatementSelectCount.append("SELECT count(oh.branchno)::INT as cntrecords ");

		// From
		basar::I18nString sqlStatementFrom = "\
			FROM orderhead oh \
			LEFT OUTER JOIN orderpos op ON oh.orderno = op.orderno AND oh.branchno = op.branchno \
			LEFT OUTER JOIN artikelzentral az ON op.articleno = az.artikel_nr \
			LEFT OUTER JOIN kunde cu ON oh.pharmacyno = cu.idfnr AND oh.branchno = cu.filialnr \
			LEFT OUTER JOIN customerspecials cs ON oh.pharmacyno = cs.customerno AND oh.branchno = cs.branchno \
			";

		basar::I18nString sqlStatementFromCount = " FROM orderhead oh ";

		// Where
		basar::I18nString sqlStatementWhere = "";
		if(isContainedAndSet("auto")) {
			sqlStatementWhere = "\
				WHERE \
				oh.branchno=#branchno#  \
				AND oh.datasourcetype in (SELECT ci.value FROM cscini ci WHERE ci.branchno=#branchno# AND ci.key='datasourcetype_auto') \
				";
		}// if(isContainedAndSet("auto")) {
		else {
			sqlStatementWhere = "\
				WHERE \
				oh.branchno=#branchno#  \
				AND oh.datasourcetype in (SELECT ci.value FROM cscini ci WHERE ci.branchno=#branchno# AND ci.key='datasourcetype_gui') \
				";
		}// else

		if(isContainedAndSet("weekday")) {
			sqlStatementWhere.append(" AND (cs.weekday = #weekday# or cs.weekday is null) ");
		}
		if(isContainedAndSet("orderno")) {
			sqlStatementWhere.append("AND oh.orderno = #orderno# ");
		}
		if(isContainedAndSet("ordernofrom")) {
			sqlStatementWhere.append("AND oh.orderno >= #ordernofrom# ");
		}
		if(isContainedAndSet("ordernoto")) {
			sqlStatementWhere.append("AND oh.orderno <= #ordernoto# ");
		}
		if(isContainedAndSet("status")) {
			// strings are always set, so they have to be checked if !=""
			basar::cmnutil::BString Str=getParamValue("status");
			Str.trim(basar::cmnutil::BString::FIRST_END);
			if(Str!="")
				sqlStatementWhere.append("AND oh.status = '#status#' ");
		}
		if(isContainedAndSet("datasourcetype")) {
			// strings are always set, so they have to be checked if !=""
			basar::cmnutil::BString Str=getParamValue("datasourcetype");
			Str.trim(basar::cmnutil::BString::FIRST_END);
			if(Str!="")
				sqlStatementWhere.append("AND oh.datasourcetype = UPPER('#datasourcetype#') ");
		}
		if(isContainedAndSet("pharmacyno")) {
			sqlStatementWhere.append("AND oh.pharmacyno = #pharmacyno# ");
		}
		if(isContainedAndSet("favoredduedate")) {
			sqlStatementWhere.append("AND oh.EDIDeliveryDate = #favoredduedate# ");
		}
		if(isContainedAndSet("manufacturerno")) {
			sqlStatementWhere.append("AND az.hersteller_nr = #manufacturerno# ");
		}

		// Group By
		basar::I18nString sqlStatementGroupBy = " GROUP BY ";
		sqlStatementGroupBy.append(" oh.branchno, oh.orderno, oh.status, oh.ordertype, oh.orderpickingtype, oh.bookentrytype, oh.ordervalue, oh.orderdate, oh.pharmacyno, oh.headertext, oh.deferredpaymentdate,  oh.orderacquisitiondate, oh.datasourcetype, oh.branchnoorig, oh.EDIDeliveryDate, oh.EstorelFlashFlag, oh.purchaseorderno");

		// Order By
		basar::I18nString sqlStatementOrderBy = " ORDER BY oh.status, oh.orderdate DESC, oh.orderno ";

		if(isContainedAndSet("count")) {
			sqlStatement.append(sqlStatementSelectCount);
			sqlStatement.append(sqlStatementFromCount);
			sqlStatement.append(sqlStatementWhere);
		}// if(isContainedAndSet("count")) {
		else {
			sqlStatement.append(sqlStatementSelect1);
			sqlStatement.append(sqlStatementSelect2);
			sqlStatement.append(sqlStatementFrom);
			sqlStatement.append(sqlStatementWhere);
			sqlStatement.append(sqlStatementGroupBy);
			sqlStatement.append(sqlStatementOrderBy);
		}// else

		resolve(sqlStatement);
		BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
	}// SelBatchOrdersByPattern

	//************************************ SaveBatchOrderByPattern ************************************************
	BUILDER_DEFINITION(SaveBatchOrderByPattern)
	//************ isExecutable
	bool SaveBatchOrderByPattern::isExecutable() const  {
		try {		
			basar::ConstString fun = "SaveBatchOrderByPattern::isExecutable()";
			bool executable = false;	
			bool goon1 = false;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
			
			//--- branchno
			if(yit.arePropertiesSet("branchno;")) {
				//---  orderno OR ordernofrom;ordernoto
				if(yit.arePropertiesSet("orderno;")) {
					goon1 = true;
				}
				if(yit.arePropertiesSet("ordernofrom;ordernoto;")) {
					goon1 = true;
				}
			}// if(yit.arePropertiesSet("branchno;"))

			//--- one of these set: status, edideliverydate, pharmacyno, ordertype
			if(goon1) {
				if(yit.arePropertiesSet("status;")) {
					executable = true;
				}
				if(yit.arePropertiesSet("edideliverydate;")) {
					executable = true;
				}
				if(yit.arePropertiesSet("pharmacyno;")) {
					executable = true;
				}
				if(yit.arePropertiesSet("ordertype;")) {
					executable = true;
				}
			}// if(goon1)

			if(!executable) {
				BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, "Conditions for saving not fulfilled: "); 
				std::stringstream os;
				if(libcscbatch::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
					os.str(""); os << fun << ": "; yit.toStream(os);
					libcscbatch::LoggerPool::loggerDomModules.forcedLog(log4cplus::INFO_LOG_LEVEL, os.str().c_str(), __FILE__, __LINE__);
				}
			}// if(!executable)
			return executable;
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "saveorderacc::SaveBatchOrderByPattern()."); 
			throw;
		}// catch
	}// SaveBatchOrderByPattern::isExecutable    

	//************ buildSQLString
	void SaveBatchOrderByPattern::buildSQLString() {
		try {
			basar::I18nString sqlStatement = "";

			// Update
			basar::I18nString sqlStatementUpdate = "UPDATE orderhead \
				";

			// Set
			bool empty=true;
			basar::I18nString sqlStatementSet = "";
			if(isContainedAndSet("status")) {
				if(empty)
					sqlStatementSet.append("SET ");
				else
					sqlStatementSet.append(", ");
				sqlStatementSet.append("status = '#status#' ");
				empty=false;
			}

			if(isContainedAndSet("edideliverydate")) {
				if(empty)
					sqlStatementSet.append("SET ");
				else
					sqlStatementSet.append(", ");
				sqlStatementSet.append("EDIDeliveryDate = #edideliverydate# ");
				empty=false;
			}
			if(isContainedAndSet("pharmacyno")) {
				if(empty)
					sqlStatementSet.append("SET ");
				else
					sqlStatementSet.append(", ");
				sqlStatementSet.append("PharmacyNo = #pharmacyno# ");
				empty=false;
			}
			else {
				if(isContainedAndSet("book_pharmacyno")) {
					if(empty)
						sqlStatementSet.append("SET ");
					else
						sqlStatementSet.append(", ");
					sqlStatementSet.append("PharmacyNo = #book_pharmacyno# ");
					empty=false;
				}// if(isContainedAndSet("book_pharmacyno"))
			}// else

			if(isContainedAndSet("ordertype")) {
				if(empty)
					sqlStatementSet.append("SET ");
				else
					sqlStatementSet.append(", ");
				empty=false;
				sqlStatementSet.append("OrderType = Substring('#ordertype#' from 1 for 2) ");
				sqlStatementSet.append(", ");
				sqlStatementSet.append("OrderPickingType = Substring('#ordertype#' from 3 for 1) ");
				sqlStatementSet.append(", ");
				sqlStatementSet.append("BookentryType = Substring('#ordertype#' from 4 for 1) ");			
			}

			// kes 14.01.14: Set also field orderacquisitiondate if it is given
			if(isContainedAndSet("orderacquisitiondate")) {
				if(empty)
					sqlStatementSet.append("SET ");
				else
					sqlStatementSet.append(", ");
				sqlStatementSet.append("orderacquisitiondate = #orderacquisitiondate# ");
				empty=false;
			}


			if(isContainedAndSet("ordernoksc")) {
				if(empty)
					sqlStatementSet.append("SET ");
				else
					sqlStatementSet.append(", ");
				sqlStatementSet.append("reference='kdauftrag.kdauftragnr:'||'#ordernoksc#'||', '||reference ");

				// kes 05.09.13: Set also new field ordernocsc
				sqlStatementSet.append(", ");
				sqlStatementSet.append("ordernocsc = #ordernoksc# ");

				empty=false;
			}

			// Where
			basar::I18nString sqlStatementWhere = "WHERE \
				branchno=#branchno# \
				";
			sqlStatementWhere.append(" AND (status='0' OR status='1')");

			if(isContainedAndSet("orderno"))
				sqlStatementWhere.append(" AND orderno=#orderno# ");
			else {
				if(isContainedAndSet("ordernofrom"))
					sqlStatementWhere.append(" AND orderno >= #ordernofrom# ");
				if(isContainedAndSet("ordernoto"))
					sqlStatementWhere.append(" AND orderno <= #ordernoto# ");
			}// else


			if(isContainedAndSet("datasourcetype"))
				sqlStatementWhere.append(" AND datasourcetype=UPPER('#datasourcetype#') ");

			sqlStatement.append(sqlStatementUpdate);
			sqlStatement.append(sqlStatementSet);
			sqlStatement.append(sqlStatementWhere);

			if(empty) {
				BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, "Nothing to be updated: "); 
				return;
			}

			resolve(sqlStatement);
			BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "SaveBatchOrderByPattern::SaveBatchOrderByPattern()."); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// SaveBatchOrderByPattern

	//************************************ SaveBatchOrderStatusChange ************************************************
	BUILDER_DEFINITION(SaveBatchOrderStatusChange)
	//************ isExecutable
	bool SaveBatchOrderStatusChange::isExecutable() const  {
		try {		
			basar::ConstString fun = "SaveBatchOrderStatusChange::isExecutable()";
			bool executable = false;	
			bool goon1 = false;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
			
			//--- branchno
			if(yit.arePropertiesSet("branchno;")) {
				//---  orderno OR ordernofrom;ordernoto
				if(yit.arePropertiesSet("orderno;")) {
					goon1 = true;
				}
				if(yit.arePropertiesSet("ordernofrom;ordernoto;")) {
					goon1 = true;
				}
			}// if(yit.arePropertiesSet("branchno;"))

			//--- one of these set: status, edideliverydate, pharmacyno, ordertype
			if(goon1) {
				if(yit.arePropertiesSet("newstatus;")) {
					executable = true;
				}
			}// if(goon1)

			if(!executable) {
				BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, "Conditions for saving not fulfilled: "); 
				std::stringstream os;
				if(libcscbatch::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
					os.str(""); os << fun << ": "; yit.toStream(os);
					libcscbatch::LoggerPool::loggerDomModules.forcedLog(log4cplus::INFO_LOG_LEVEL, os.str().c_str(), __FILE__, __LINE__);
				}
			}// if(!executable)
			return executable;
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "saveorderacc::SaveBatchOrderStatusChange()."); 
			throw;
		}// catch
	}// SaveBatchOrder::isExecutable    

	//************ buildSQLString
	void SaveBatchOrderStatusChange::buildSQLString() {
		try {
			basar::I18nString sqlStatement = "";

			// Update
			basar::I18nString sqlStatementUpdate = "UPDATE orderhead \
				";

			// Set
			basar::I18nString sqlStatementSet = "";
			if(isContainedAndSet("newstatus")) 
				sqlStatementSet.append("SET status = '#newstatus#' ");

			// Where
			basar::I18nString sqlStatementWhere = "WHERE \
				branchno=#branchno# \
				";

			if(isContainedAndSet("status"))
				sqlStatementWhere.append(" AND status=#status# ");

			if(isContainedAndSet("orderno"))
				sqlStatementWhere.append(" AND orderno=#orderno# ");
			else {
				if(isContainedAndSet("ordernofrom"))
					sqlStatementWhere.append(" AND orderno >= #ordernofrom# ");
				if(isContainedAndSet("ordernoto"))
					sqlStatementWhere.append(" AND orderno <= #ordernoto# ");
			}// else

			if(isContainedAndSet("datasourcetype"))
				sqlStatementWhere.append(" AND datasourcetype=UPPER('#datasourcetype#') ");

			sqlStatement.append(sqlStatementUpdate);
			sqlStatement.append(sqlStatementSet);
			sqlStatement.append(sqlStatementWhere);

			resolve(sqlStatement);
			BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "SaveBatchOrder::SaveBatchOrderStatusChange()."); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// SaveBatchOrderStatusChange

	//************************************ SaveBatchOrderForRelease ************************************************
	BUILDER_DEFINITION(SaveBatchOrderForRelease)
	//************ isExecutable
	bool SaveBatchOrderForRelease::isExecutable() const  {
		try {		
			basar::ConstString fun = "SaveBatchOrderForRelease::isExecutable()";
			bool executable = true;	
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
			
			//--- branchno
			if(!yit.arePropertiesSet("branchno;")) 
				executable = false;
			if(!yit.arePropertiesSet("status;")) 
				executable = false;

			if(!executable) {
				BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, "Conditions for saving not fulfilled: "); 
				std::stringstream os;
				if(libcscbatch::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
					os.str(""); os << fun << ": "; yit.toStream(os);
					libcscbatch::LoggerPool::loggerDomModules.forcedLog(log4cplus::INFO_LOG_LEVEL, os.str().c_str(), __FILE__, __LINE__);
				}
			}// if(!executable)
			return executable;
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "saveorderacc::SaveBatchOrderForRelease()."); 
			throw;
		}// catch
	}// SaveBatchOrderForRelease::isExecutable    

	//************ buildSQLString
	void SaveBatchOrderForRelease::buildSQLString() {
		try {
			basar::I18nString sqlStatement = "";

			// Update
			basar::I18nString sqlStatementUpdate = "UPDATE orderhead \
				";

			// Set
			basar::I18nString sqlStatementSet = "SET status = '#status#' ";

			if(isContainedAndSet("orderacquisitiondate")) {

				// kes 10.01.14: Set also  field orderacquisitiondate
				sqlStatementSet.append(", ");
				sqlStatementSet.append("orderacquisitiondate = #orderacquisitiondate# ");

			}

			if(isContainedAndSet("ordernoksc")) {
				sqlStatementSet.append(", ");
				sqlStatementSet.append("reference='kdauftrag.kdauftragnr:'||'#ordernoksc#'||', '||reference ");

				// kes 05.09.13: Set also new field ordernocsc
				sqlStatementSet.append(", ");
				sqlStatementSet.append("ordernocsc = #ordernoksc# ");
			}


			// Where
			basar::I18nString sqlStatementWhere = " WHERE \
				branchno=#branchno# \
				AND status='0' \
				";
//				AND (status='0' OR status='1') 

			if(isContainedAndSet("today"))
				sqlStatementWhere.append(" AND edideliverydate<=#today# ");

			if(isContainedAndSet("orderno"))
				sqlStatementWhere.append(" AND orderno=#orderno# ");

			if(isContainedAndSet("ordernofrom"))
				sqlStatementWhere.append(" AND orderno>=#ordernofrom# ");

			if(isContainedAndSet("ordernoto"))
				sqlStatementWhere.append(" AND orderno<=#ordernoto# ");

/*			if(isContainedAndSet("action_status"))
				sqlStatementWhere.append(" AND status=#status# ");
*/
			if(isContainedAndSet("datasourcetype"))
				sqlStatementWhere.append(" AND datasourcetype=UPPER('#datasourcetype#') ");

			if(isContainedAndSet("edideliverydate"))
				sqlStatementWhere.append(" AND edideliverydate=#edideliverydate# ");

			if(isContainedAndSet("pharmacyno"))
				sqlStatementWhere.append("AND pharmacyno=#pharmacyno# ");

			sqlStatement.append(sqlStatementUpdate);
			sqlStatement.append(sqlStatementSet);
			sqlStatement.append(sqlStatementWhere);

			resolve(sqlStatement);
			BLOG_TRACE(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, "SaveBatchOrder::SaveBatchOrderForRelease()."); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// SaveBatchOrderForRelease

	//************************************ SelBatchOrdersAcc ************************************************
	//
	// Accessor-Definitions werden vom Manager registriert und  verwaltet.
	// Instanziierung notwendig!
	//
	//*******************************************************************************************************
	BEGIN_ACCESSOR_DEFINITION("SelBatchOrdersAcc")
		PROPERTY_DESCRIPTION_LIST("\
			int16 branchno; \
			int32 orderno; \
			string status; \
			string newstatus; \
			string ordertype; \
			int32 ordervalue; \
			string orderdate; \
			int32 pharmacyno; \
			string headertext; \
			string deferredpaymentdate; \
			string orderacquisitiondate; \
			string datasourcetype; \
			int16 branchnoorig; \
			string edideliverydate; \
			string pharmacyname; \
			string nameowner; \
			string street; \
			string zipcode; \
			string city; \
			string telno; \
			string quickdialno; \
			string faxno; \
			int32 posqty; \
			int32 manufacturerno; \
			int32 countunknown; \
			int32 uw_idf; \
			int16 weekday; \
			string uw_active; \
			int32 book_pharmacyno; \
			string batch; \
			int32 cntrecords; \
			int32 purchaseorderno; \
			")

		ACCESS_METHOD("SelBatchOrdersByPattern")
			SQL_BUILDER_CREATE(SelBatchOrdersByPattern)
			SQL_BUILDER_PUSH_BACK(SelBatchOrdersByPattern)

		ACCESS_METHOD("SaveBatchOrderByPattern")
			SQL_BUILDER_CREATE(SaveBatchOrderByPattern)
			SQL_BUILDER_PUSH_BACK(SaveBatchOrderByPattern)

		ACCESS_METHOD("SaveBatchOrderStatusChange")
			SQL_BUILDER_CREATE(SaveBatchOrderStatusChange)
			SQL_BUILDER_PUSH_BACK(SaveBatchOrderStatusChange)

		ACCESS_METHOD("SaveBatchOrderForRelease")
			SQL_BUILDER_CREATE(SaveBatchOrderForRelease)
			SQL_BUILDER_PUSH_BACK(SaveBatchOrderForRelease)

END_ACCESSOR_DEFINITION			
}//batchorderacc

