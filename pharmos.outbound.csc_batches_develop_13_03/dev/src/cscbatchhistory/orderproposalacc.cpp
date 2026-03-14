//-------------------------------------------------------------------------------------------------//
/*! \file	orderproposalacc.cpp
 *  \brief  string builder definition for orderproposal-accessor + registration of orderproposal-accessor
 *  \author	Xin Zhou
 *  \date   22.05.2013
 */
//-------------------------------------------------------------------------------------------------//

//**************************************************************************************************
//
// Pay attention here! 
// It is  necessary to copy (and delete) the orderproposal according to minimumagefororderproposal!
// otherwise, the datasets of orderproposal which are ready to edit could be deleted
//
//**************************************************************************************************

//**************************************************************************************************
// includes
//**************************************************************************************************
#include "orderproposalacc.h"
#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertydescriptionlistref.h>
#include <libbasarcmnutil_i18nstring.h>

namespace orderproposalacc {
	//************************************ saveArchiveOrderProposal ************************************************
	BUILDER_DEFINITION(saveArchiveOrderProposal)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(saveArchiveOrderProposal,"branchno;minimumagefororderproposal;")

	//************ buildSQLString
	void saveArchiveOrderProposal::buildSQLString() {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "orderproposalacc::saveArchiveOrderProposal::buildSQLString().");
		try {		
			basar::I18nString sqlStatement = "";
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			// Insert
			basar::I18nString sqlStatementInsert = "INSERT INTO aOrderProposal" ;

			// Columns
			basar::I18nString sqlStatementColumns = "("
				"  BranchNo"
				", ArticleNo"
				", OrderProposalQty"
				", OrderNoBatch"
				", PosNoBatch"
				", OrderTypeBatch"
				", ReasonId"
				", OrderProposalTime"
				", TransferTime"
				", OrderProposalProcessedTime"
				", OrderProposalDisclaimNote"
				", OrderProposalNo"
				", PurchaseOrderNo"
				", DateOfPurchase"
				", TransferTimeBi"
                ", completed "
				")";

			// Values
			basar::I18nString sqlStatementValues = " SELECT "
				"opp.BranchNo"
				", opp.ArticleNo"
				", opp.OrderProposalQty"
				", opp.OrderNoBatch"
				", opp.PosNoBatch"
				", opp.OrderTypeBatch"
				", opp.ReasonId"
				", opp.OrderProposalTime"
				", opp.TransferTime"
				", opp.OrderProposalProcessedTime"
				", opp.OrderProposalDisclaimNote"
				", opp.OrderProposalNo"
				", opp.PurchaseOrderNo"
				", opp.DateOfPurchase"
				", opp.TransferTimeBi"
                ", opp.completed";

			// FromWhere
			basar::I18nString sqlStatementFromWhere = " FROM orderproposal opp WHERE opp.branchno=#branchno# ";
			
			sqlStatementFromWhere.append(" AND (orderproposaltime < (TODAY - #minimumagefororderproposal#) OR completed = 1)");

			
			sqlStatementFromWhere.append(";");

			sqlStatement.append(sqlStatementInsert);
			sqlStatement.append(sqlStatementColumns);
			sqlStatement.append(sqlStatementValues);
			sqlStatement.append(sqlStatementFromWhere);
			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString());
		}// try
		catch(...) {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "orderproposalacc::saveArchiveOrderProposal()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// saveArchiveOrderProposal

	//************************************ deleteOrderProposal ************************************************
	BUILDER_DEFINITION(deleteOrderProposal)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(deleteOrderProposal,"branchno;minimumagefororderproposal;")

	//************ buildSQLString
	void deleteOrderProposal::buildSQLString() {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "orderproposalacc::deleteOrderProposal::buildSQLString().");
		try {		
			basar::I18nString sqlStatement = "";
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			// Delete
			basar::I18nString sqlStatementDelete = "DELETE ";

			// FromWhere
			basar::I18nString sqlStatementFromWhere = " FROM orderproposal WHERE branchno=#branchno# ";
			sqlStatementFromWhere.append(" AND (orderproposaltime < (TODAY - #minimumagefororderproposal#) OR completed = 1);");
			
			sqlStatement.append(sqlStatementDelete);
			sqlStatement.append(sqlStatementFromWhere);

			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}// try
		catch(...) {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "orderproposalacc::deleteOrderProposal()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}// catch
	}// deleteOrderProposal


	//************************************ OrderProposalAcc ************************************************
	//
	// Accessor definitions are registerd and manageed by the Manager.
	// Instantiating necessary!
	//
	//*******************************************************************************************************
	BEGIN_ACCESSOR_DEFINITION("OrderProposalAcc")
		PROPERTY_DESCRIPTION_LIST("\
			int16 branchno; \
			int32 minimumagefororderproposal; \
			")

			ACCESS_METHOD("saveArchiveOrderProposal")
			SQL_BUILDER_CREATE(saveArchiveOrderProposal)
			SQL_BUILDER_PUSH_BACK(saveArchiveOrderProposal)

			ACCESS_METHOD("deleteOrderProposal")
			SQL_BUILDER_CREATE(deleteOrderProposal)
			SQL_BUILDER_PUSH_BACK(deleteOrderProposal)
			
	END_ACCESSOR_DEFINITION			
}// namespace

