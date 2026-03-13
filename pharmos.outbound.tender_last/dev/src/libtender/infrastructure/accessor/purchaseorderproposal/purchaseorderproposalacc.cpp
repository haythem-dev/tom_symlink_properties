// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/purchaseorderproposal/purchaseorderproposalacc.h>
#include <infrastructure/accessor/purchaseorderproposal/purchaseorderproposalacc_definitions.h>

//-------------------------------------------------------------------------------------------------//

namespace libtender {
namespace infrastructure {
namespace accessor {
namespace purchaseOrderProposal {

//-------------------------------------------------------------------------------------------------//

BEGIN_ACCESSOR_DEFINITION( lit::PURCHASEORDERPROPOSAL_ACC )

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( properties::POS_ID )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NO )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_PROPOSAL_QUANTITY )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_NO_BATCH )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_TYPE_BATCH )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_PROPOSAL_TIME )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TRANSFER_TIME )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_PROPOSAL_NO )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASE_ORDER_NO )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::EXPECTED_GOODSIN_DATE )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::SUPPLIER_NO )

		PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASE_DBSRV )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASE_DB )
	END_PROPERTY_DESCRIPTION_LIST
  
	ACCESS_METHOD( lit::SELECT_PURCHASEORDERPROPOSAL_BY_KEY	);
		SQL_BUILDER_CREATE( SelectPurchaseOrderProposalByKey )
		SQL_BUILDER_PUSH_BACK( SelectPurchaseOrderProposalByKey )

	ACCESS_METHOD( lit::SAVE_PURCHASEORDERPROPOSAL );
		SQL_BUILDER_CREATE( InsertPurchaseOrderProposal )
		SQL_BUILDER_PUSH_BACK( InsertPurchaseOrderProposal )
		SQL_BUILDER_CREATE( UpdatePurchaseOrderProposal )
		SQL_BUILDER_PUSH_BACK( UpdatePurchaseOrderProposal )
		SQL_BUILDER_CREATE( DeletePurchaseOrderProposal )
		SQL_BUILDER_PUSH_BACK( DeletePurchaseOrderProposal )
  
END_ACCESSOR_DEFINITION
  
//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//

BUILDER_DEFINITION( SelectPurchaseOrderProposalByKey )
  
ENSURE_PROPERTIES_ARE_SET(SelectPurchaseOrderProposalByKey,  
		//properties::BRANCH_NO.toPropertyString() +
		properties::ARTICLE_NO.toPropertyString() +
		properties::ORDER_NO_BATCH.toPropertyString() +
		properties::ORDER_TYPE_BATCH.toPropertyString()
		)
  
void SelectPurchaseOrderProposalByKey::buildSQLString()
{
	resolve(
		"SELECT "
			"posid AS "					+ properties::POS_ID.getName()					+ ", "
			"branchno AS "				+ properties::BRANCH_NO.getName()				+ ", "
			"articleno AS "				+ properties::ARTICLE_NO.getName()				+ ", "
			"orderproposalqty AS "		+ properties::ORDER_PROPOSAL_QUANTITY.getName()	+ ", "
			"ordernobatch AS "			+ properties::ORDER_NO_BATCH.getName()			+ ", "
			"ordertypebatch AS "		+ properties::ORDER_TYPE_BATCH.getName()		+ ", "
			"orderproposaltime AS "		+ properties::ORDER_PROPOSAL_TIME.getName()		+ ", "
			"orderproposalno AS "		+ properties::ORDER_PROPOSAL_NO.getName()		+ ", "
			"purchaseorderno AS "		+ properties::PURCHASE_ORDER_NO.getName()		+ ", "
			"expectedgoodsindate AS "	+ properties::EXPECTED_GOODSIN_DATE.getName()	+ ", "
			"supplierno AS "			+ properties::SUPPLIER_NO.getName()				+ " "
		"FROM "
			+ properties::PURCHASE_DB.toSQLReplacementString() + "@" 
			+ properties::PURCHASE_DBSRV.toSQLReplacementString() + 
			":pubatchorderpos "
		"WHERE "
			//"branchno="			+ properties::BRANCH_NO.toSQLReplacementString()		+ " AND "
			"articleno="		+ properties::ARTICLE_NO.toSQLReplacementString()		+ " AND "
			"ordernobatch="		+ properties::ORDER_NO_BATCH.toSQLReplacementString()	+ " AND "
			"ordertypebatch="	+ properties::ORDER_TYPE_BATCH.toSQLReplacementString()
		);
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//

BUILDER_DEFINITION( InsertPurchaseOrderProposal )
  
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( InsertPurchaseOrderProposal,  
	properties::BRANCH_NO.toPropertyString() +
	properties::ARTICLE_NO.toPropertyString() +
	properties::ORDER_PROPOSAL_QUANTITY.toPropertyString() +
	properties::ORDER_NO_BATCH.toPropertyString() +
	properties::ORDER_TYPE_BATCH.toPropertyString() +
	properties::EXPECTED_GOODSIN_DATE.toPropertyString() +
	properties::SUPPLIER_NO.toPropertyString(),

	properties::BRANCH_NO.toPropertyString() +
	properties::ARTICLE_NO.toPropertyString() +
	properties::ORDER_PROPOSAL_QUANTITY.toPropertyString() +
	properties::ORDER_NO_BATCH.toPropertyString() +
	properties::ORDER_TYPE_BATCH.toPropertyString() +
	properties::EXPECTED_GOODSIN_DATE.toPropertyString() +
	properties::SUPPLIER_NO.toPropertyString(),

	basar::SS_INSERT
	)
  
void InsertPurchaseOrderProposal::buildSQLString()
{
	resolve(
		"INSERT INTO " 
			+ properties::PURCHASE_DB.toSQLReplacementString() + "@" 
			+ properties::PURCHASE_DBSRV.toSQLReplacementString() + 
			":pubatchorderpos (branchno, articleno, orderproposalqty, ordernobatch, ordertypebatch, orderproposaltime, expectedgoodsindate, supplierno) "
		"VALUES ("
			+ properties::BRANCH_NO.toSQLReplacementString() + ", "
			+ properties::ARTICLE_NO.toSQLReplacementString() + ", "
			+ properties::ORDER_PROPOSAL_QUANTITY.toSQLReplacementString() + ", "
			+ properties::ORDER_NO_BATCH.toSQLReplacementString() + ", "
			+ properties::ORDER_TYPE_BATCH.toSQLReplacementString() + ", "
			"CURRENT, "
			+ properties::EXPECTED_GOODSIN_DATE.toSQLReplacementString() + ", "
			+ properties::SUPPLIER_NO.toSQLReplacementString() +
			")"
		);
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//

BUILDER_DEFINITION( UpdatePurchaseOrderProposal )
  
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( UpdatePurchaseOrderProposal,  
	properties::POS_ID.toPropertyString(),

	properties::ORDER_PROPOSAL_QUANTITY.toPropertyString() +
	properties::EXPECTED_GOODSIN_DATE.toPropertyString() +
	properties::BRANCH_NO.toPropertyString(),

	basar::SS_UPDATE
	)
  
void UpdatePurchaseOrderProposal::buildSQLString()
{
	resolve(
		"UPDATE "
			+ properties::PURCHASE_DB.toSQLReplacementString() + "@"
			+ properties::PURCHASE_DBSRV.toSQLReplacementString() +
			":pubatchorderpos SET "
			"branchno="						+ properties::BRANCH_NO.toSQLReplacementString()				+ ", "
			"orderproposalqty="				+ properties::ORDER_PROPOSAL_QUANTITY.toSQLReplacementString()	+ ", "
			"orderproposaltime=CURRENT, "
			"expectedgoodsindate="			+ properties::EXPECTED_GOODSIN_DATE.toSQLReplacementString()		+ " "
		"WHERE "
			"posid="						+ properties::POS_ID.toSQLReplacementString()
		);
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-------------------------------------------------------------------------------------------------//

BUILDER_DEFINITION( DeletePurchaseOrderProposal )
  
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( DeletePurchaseOrderProposal,  
	properties::POS_ID.toPropertyString(),

	properties::POS_ID.toPropertyString(),

	basar::SS_DELETE
	)
  
void DeletePurchaseOrderProposal::buildSQLString()
{
	resolve(
		"DELETE FROM "
			+ properties::PURCHASE_DB.toSQLReplacementString() + "@"
			+ properties::PURCHASE_DBSRV.toSQLReplacementString() +
			":pubatchorderpos "
		"WHERE "
			"posid=" + properties::POS_ID.toSQLReplacementString()
		);
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-------------------------------------------------------------------------------------------------//

} // end namespace purchaseOrderProposal
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender

//-------------------------------------------------------------------------------------------------//
