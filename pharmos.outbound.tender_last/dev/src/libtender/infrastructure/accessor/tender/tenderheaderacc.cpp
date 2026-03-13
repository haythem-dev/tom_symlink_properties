// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/tender/tenderheaderacc.h>
#include <infrastructure/accessor/tender/tenderheaderacc_definitions.h>

namespace libtender
{
namespace infrastructure
{
namespace accessor
{
namespace tender
{
BEGIN_ACCESSOR_DEFINITION( lit::TENDER_HEADER_ACC )

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_NO					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_TYPE					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CONTRACT_NO					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ACC_BRANCH_NO				)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ACC_CUSTOMER_NO				)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PERSONAL_NR_VERTR			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_NAME				)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PARTNER_NO					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::DATE_FROM					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::DATE_TO						)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::EXCEEDANCE_PCT				)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_STATE					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_CHAIN_NO			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::DELETED_FLAG					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASE_ORDER_PROPOSAL_FLAG )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::NAME_OF_USER					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMERGROUPID				)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_GROUPID             )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::RECALL_FLAG                  )
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( accessor::tender::lit::SELECT_TENDER_HEADER_BY_KEY );
		SQL_BUILDER_CREATE( SelectTenderHeaderByKey )
		SQL_BUILDER_PUSH_BACK( SelectTenderHeaderByKey )

	ACCESS_METHOD( accessor::tender::lit::SAVE_TENDER_HEADER )
		SQL_BUILDER_CREATE	 ( DeleteTenderHeader )
		SQL_BUILDER_PUSH_BACK( DeleteTenderHeader )
		SQL_BUILDER_CREATE	 ( UpdateTenderHeader )
		SQL_BUILDER_PUSH_BACK( UpdateTenderHeader )
		SQL_BUILDER_CREATE	 ( InsertTenderHeader )
		SQL_BUILDER_PUSH_BACK( InsertTenderHeader )

END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectTenderHeaderByKey )

bool SelectTenderHeaderByKey::isExecutable() const
{
	METHODNAME_DEF( SelectTenderHeaderByKey, isExecutable );
	static const log4cplus::Logger & logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = arePropertiesSet( properties::TENDER_NO.toPropertyString() );

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

void SelectTenderHeaderByKey::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	// to catch hold of a little bit readability
	static const VarString & tenderno =						TENDER_NO.getName();
	static const VarString & tendertype =					TENDER_TYPE.getName();
	static const VarString & contractno =					CONTRACT_NO.getName();
	static const VarString & branchno =						BRANCH_NO.getName();
	static const VarString & customerno =					CUSTOMER_NO.getName();
	static const VarString & personalnrvertr =				PERSONAL_NR_VERTR.getName();
	static const VarString & pharmacyname =					PHARMACY_NAME.getName();
	static const VarString & partnerno =					PARTNER_NO.getName();
	static const VarString & accbranchno =					ACC_BRANCH_NO.getName();
	static const VarString & acccustomerno =				ACC_CUSTOMER_NO.getName();
	static const VarString & begindate =					DATE_FROM.getName();
	static const VarString & enddate =						DATE_TO.getName();
	static const VarString & limitexceedancepercentage =	EXCEEDANCE_PCT.getName();
	static const VarString & tenderstate =					TENDER_STATE.getName();
	static const VarString & pharmacychainno =				PHARMACY_CHAIN_NO.getName();
	static const VarString & deletedflag =					DELETED_FLAG.getName();
	static const VarString & purchaseorderproposalflag =	PURCHASE_ORDER_PROPOSAL_FLAG.getName();
	static const VarString & nameofuser =					NAME_OF_USER.getName();
	static const VarString & customergroupid =				CUSTOMERGROUPID.getName();
    static const VarString & pharmacygroupid =				PHARMACY_GROUPID.getName();
	static const VarString sql_tenderid( TENDER_NO.toSQLReplacementString() );
//  static const VarString& sql_recallflag =                RECALL_FLAG.getName();  //undo after adding recall flag column in tenderheader table 


	resolve(
		"SELECT DISTINCT "
			"tenderno AS " + tenderno + ", "
			"tendertype AS " + tendertype + ", "
			"contractno AS " + contractno + ", "
			"branchno AS " + branchno + ", "
			"customerno AS " + customerno + ", "
			"personalnrvertr AS " + personalnrvertr + ", "
			"k.nameapo AS " + pharmacyname + ", "
			"partnerno AS " + partnerno + ", "
			"accbranchno AS " + accbranchno + ", "
			"acccustomerno AS " + acccustomerno + ", "
			"TO_DATE(datefrom::CHAR(8), '%Y%m%d')::DATE AS " + begindate + ", "
			"TO_DATE(dateto::CHAR(8), '%Y%m%d')::DATE AS " + enddate + ", "
			"exceedancepct AS " + limitexceedancepercentage + ", "
			"tenderstate::SMALLINT AS " + tenderstate + ", "
			"pharmacychainno AS " + pharmacychainno + ", "
			"deletedflag AS " + deletedflag + ", "
			"purchaseorderproposalflag AS " + purchaseorderproposalflag + ", "
 //         "recallflag As " + sql_recallflag + ", " //undo after adding recall flag column in tenderheader table 
			"nameofuser AS " + nameofuser + ", "
			"customergroupid AS " + customergroupid + ", "
            "pharmacygroupid AS " + pharmacygroupid + " "
		"FROM tenderhead t "
			"LEFT JOIN kunde as k ON t.customerno=k.idfnr AND t.branchno=k.filialnr "
		"WHERE "
			"tenderno=" + sql_tenderid
		);

	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}


BUILDER_DEFINITION( InsertTenderHeader )
	
using namespace properties;

ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( InsertTenderHeader,  
	TENDER_TYPE.toPropertyString() +
	CONTRACT_NO.toPropertyString() +
	DATE_FROM.toPropertyString() +
	DATE_TO.toPropertyString() +
	EXCEEDANCE_PCT.toPropertyString() +
	TENDER_STATE.toPropertyString() +
  //RECALL_FLAG.toPropertyString()+//undo after adding recall flag column in tenderheader table 
	PURCHASE_ORDER_PROPOSAL_FLAG.toPropertyString(),
   

	TENDER_TYPE.toPropertyString() +
	CONTRACT_NO.toPropertyString() +
	DATE_FROM.toPropertyString() +
	DATE_TO.toPropertyString() +
	EXCEEDANCE_PCT.toPropertyString() +
	TENDER_STATE.toPropertyString() +
   //RECALL_FLAG.toPropertyString()+//undo after adding recall flag column in tenderheader table 
	PURCHASE_ORDER_PROPOSAL_FLAG.toPropertyString(),

	basar::SS_INSERT
		)

void InsertTenderHeader::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	// to catch hold of a little bit readability
	static const VarString sql_tendertype( TENDER_TYPE.toSQLReplacementString() );
	static const VarString sql_contractno( CONTRACT_NO.toSQLReplacementString() );
	static const VarString sql_branchno( BRANCH_NO.toSQLReplacementString() );
	static const VarString sql_customerno( CUSTOMER_NO.toSQLReplacementString() );
	static const VarString sql_partnerno( PARTNER_NO.toSQLReplacementString() );
	static const VarString sql_accbranchno( ACC_BRANCH_NO.toSQLReplacementString() );
	static const VarString sql_acccustomerno( ACC_CUSTOMER_NO.toSQLReplacementString() );
	static const VarString sql_begindate( DATE_FROM.toSQLReplacementString() );
	static const VarString sql_enddate( DATE_TO.toSQLReplacementString() );
	static const VarString sql_limitexceedancepercentage( EXCEEDANCE_PCT.toSQLReplacementString() );
	static const VarString sql_tenderstate( TENDER_STATE.toSQLReplacementString() );
	static const VarString sql_pharmacychainno( PHARMACY_CHAIN_NO.toSQLReplacementString() );
	static const VarString sql_deletedflag( DELETED_FLAG.toSQLReplacementString() );
	static const VarString sql_purchaseorderproposalflag( PURCHASE_ORDER_PROPOSAL_FLAG.toSQLReplacementString() );
	static const VarString sql_nameofuser( NAME_OF_USER.toSQLReplacementString() );
	static const VarString sql_customergroupid( CUSTOMERGROUPID.toSQLReplacementString() );
    static const VarString sql_pharmacygroupid( PHARMACY_GROUPID.toSQLReplacementString() );
 // static const VarString sql_recallflag( RECALL_FLAG.toSQLReplacementString() ); //undo after adding recall flag column in tenderheader table 
	static const basar::VarString constSQL(
		"INSERT INTO "
		"tenderhead "

		"( "
		"tendertype, "
		"contractno, "
		"branchno, "
		"customerno, "
		"partnerno, "
		"accbranchno, "
		"acccustomerno, "
		"datefrom, "
		"dateto, "
		"exceedancepct, "
		"tenderstate, "
		"changedate, "
		"pharmacychainno, "
		"purchaseorderproposalflag, "
		"deletedflag,"
     // "recallflag,"   //undo after adding recall flag column in tenderheader table 
		"nameofuser, "
		"customergroupid, "
		"pharmacygroupid "
		") "
		"VALUES "
		"( "
		+ sql_tendertype + ", "
		"'" + sql_contractno + "', "
		+ sql_branchno + ", "
		+ sql_customerno + ", "
		+ sql_partnerno + ", "
		+ sql_accbranchno + ", " 
		+ sql_acccustomerno + ", "			
		"TO_CHAR(TO_DATE('" + sql_begindate + "', '%m/%d/%Y'), '%Y%m%d')::INTEGER, "
		"TO_CHAR(TO_DATE('" + sql_enddate + "', '%m/%d/%Y'), '%Y%m%d')::INTEGER, "
		+ sql_limitexceedancepercentage + ", "
		+ sql_tenderstate + ", "
		"TO_CHAR(CURRENT, '%Y%m%d')::INTEGER, "
		+ sql_pharmacychainno + ", "
		+ sql_purchaseorderproposalflag + ", "
      //+ sql_recallflag + ", " //undo after adding recall flag column in tenderheader table 
		"0, "
		"'" + sql_nameofuser + "', "
		"'" + sql_customergroupid + "', "
        "'" + sql_pharmacygroupid + "' "
		") "
			);

	resolve( constSQL );
	BLOG_TRACE( libtender::LoggerPool::getLoggerDomModules(), getSQLString() );
}	

BUILDER_DEFINITION( UpdateTenderHeader )
	
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( UpdateTenderHeader,  
	TENDER_NO.toPropertyString() +
	TENDER_TYPE.toPropertyString() +
	CONTRACT_NO.toPropertyString() +
	BRANCH_NO.toPropertyString() +
	CUSTOMER_NO.toPropertyString() +
	ACC_BRANCH_NO.toPropertyString() +
	ACC_CUSTOMER_NO.toPropertyString() +
	PARTNER_NO.toPropertyString() +
	DATE_FROM.toPropertyString() +
	DATE_TO.toPropertyString() +
	EXCEEDANCE_PCT.toPropertyString() +
	TENDER_STATE.toPropertyString() +
	PHARMACY_CHAIN_NO.toPropertyString() +
	PURCHASE_ORDER_PROPOSAL_FLAG.toPropertyString() +
  //RECALL_FLAG.toPropertyString()+//undo after adding recall flag column in tenderheader table 
	NAME_OF_USER.toPropertyString() +
	CUSTOMERGROUPID.toPropertyString()+
	PHARMACY_GROUPID.toPropertyString(),

	TENDER_NO.toPropertyString() +
	TENDER_TYPE.toPropertyString() +
	CONTRACT_NO.toPropertyString() +
	BRANCH_NO.toPropertyString() +
	CUSTOMER_NO.toPropertyString() +
	ACC_BRANCH_NO.toPropertyString() +
	ACC_CUSTOMER_NO.toPropertyString() +
	PARTNER_NO.toPropertyString() +
	DATE_FROM.toPropertyString() +
	DATE_TO.toPropertyString() +
	EXCEEDANCE_PCT.toPropertyString() +
	TENDER_STATE.toPropertyString() +
	PHARMACY_CHAIN_NO.toPropertyString() +
	PURCHASE_ORDER_PROPOSAL_FLAG.toPropertyString() +
  //RECALL_FLAG.toPropertyString()+//undo after adding recall flag column in tenderheader table 
	NAME_OF_USER.toPropertyString() +
	CUSTOMERGROUPID.toPropertyString()+
    PHARMACY_GROUPID.toPropertyString(),

	basar::SS_UPDATE
		)

void UpdateTenderHeader::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	// to catch hold of a little bit readability
	static const VarString sql_tenderno( TENDER_NO.toSQLReplacementString() );
	static const VarString sql_tendertype( TENDER_TYPE.toSQLReplacementString() );
	static const VarString sql_contractno( CONTRACT_NO.toSQLReplacementString() );
	static const VarString sql_branchno( BRANCH_NO.toSQLReplacementString() );
	static const VarString sql_customerno( CUSTOMER_NO.toSQLReplacementString() );
	static const VarString sql_partnerno( PARTNER_NO.toSQLReplacementString() );
	static const VarString sql_accbranchno( ACC_BRANCH_NO.toSQLReplacementString() );
	static const VarString sql_acccustomerno( ACC_CUSTOMER_NO.toSQLReplacementString() );
	static const VarString sql_begindate( DATE_FROM.toSQLReplacementString() );
	static const VarString sql_enddate( DATE_TO.toSQLReplacementString() );
	static const VarString sql_limitexceedancepercentage( EXCEEDANCE_PCT.toSQLReplacementString() );
	static const VarString sql_tenderstate( TENDER_STATE.toSQLReplacementString() );
	static const VarString sql_pharmacychainno( PHARMACY_CHAIN_NO.toSQLReplacementString() );
	static const VarString sql_deletedflag( DELETED_FLAG.toSQLReplacementString() );
	static const VarString sql_purchaseorderproposalflag( PURCHASE_ORDER_PROPOSAL_FLAG.toSQLReplacementString() );
	static const VarString sql_nameofuser( NAME_OF_USER.toSQLReplacementString() );
	static const VarString sql_customergroupid( CUSTOMERGROUPID.toSQLReplacementString() );
    static const VarString sql_pharmacygroupid( PHARMACY_GROUPID.toSQLReplacementString() );
 // static const VarString sql_recallflag( RECALL_FLAG.toSQLReplacementString() ); //undo after adding recall flag column in tenderheader table 

	static const basar::VarString constSQL(
		"UPDATE "
		"tenderhead "

		"SET "
		"tendertype = " + sql_tendertype + ", "
		"contractno = '" + sql_contractno + "', "
		"branchno = " + sql_branchno + ", "
		"customerno = " + sql_customerno + ", "
		"acccustomerno = " + sql_acccustomerno + ", "
		"accbranchno = " + sql_accbranchno + ", "
		"partnerno = " + sql_partnerno + ", "
		"datefrom = TO_CHAR(TO_DATE('" + sql_begindate + "', '%m/%d/%Y'), '%Y%m%d')::INTEGER, "
		"dateto = TO_CHAR(TO_DATE('" + sql_enddate + "', '%m/%d/%Y'), '%Y%m%d')::INTEGER, "
		"exceedancepct = " + sql_limitexceedancepercentage + ", "
		"tenderstate = " + sql_tenderstate + ", "
		"changedate = TO_CHAR(CURRENT, '%Y%m%d')::INTEGER, "
		"pharmacychainno = " + sql_pharmacychainno + ", "
		"purchaseorderproposalflag = " + sql_purchaseorderproposalflag + ", "
      //"recallflag = " + sql_recallflag +", " //undo after adding recall flag column in tenderheader table 
		"nameofuser = '" + sql_nameofuser + "', "
		"customergroupid = '" + sql_customergroupid + "', " 
	    "pharmacygroupid = '" + sql_pharmacygroupid + "' " 
		
		"WHERE "
		"tenderno = " + sql_tenderno
		);

	resolve( constSQL );
	BLOG_TRACE( libtender::LoggerPool::getLoggerDomModules(), getSQLString() );
}	


BUILDER_DEFINITION( DeleteTenderHeader )

ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( DeleteTenderHeader,  
	TENDER_NO.toPropertyString(),

	TENDER_NO.toPropertyString(),

	basar::SS_DELETE
		)

void DeleteTenderHeader::buildSQLString()
{
	resolve( 
		"DELETE FROM tenderhead "
		"where tenderno = "			+ properties::TENDER_NO.toSQLReplacementString() 
		);

	BLOG_TRACE( libtender::LoggerPool::getLoggerDomModules(), getSQLString() );
}

} // end namespace tender
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender

