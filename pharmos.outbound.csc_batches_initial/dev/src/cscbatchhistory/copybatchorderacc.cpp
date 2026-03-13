#include "copybatchorderacc.h"
#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertydescriptionlistref.h>
#include <libbasarcmnutil_i18nstring.h>

namespace copybatchorderacc 
{
	//************************************ saveArchiveBatchOrderHeads ************************************************
	BUILDER_DEFINITION(saveArchiveBatchOrderHeads)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(saveArchiveBatchOrderHeads,"branchno;lessthanorderdate;")

	//************ buildSQLString
	void saveArchiveBatchOrderHeads::buildSQLString() 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::saveArchiveBatchOrderHeads::buildSQLString().");

		try 
		{		
			basar::I18nString sqlStatement = "";
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			// Insert
			basar::I18nString sqlStatementInsert = "INSERT INTO aOrderHead ";

			// Columns
			basar::I18nString sqlStatementColumns = "("
				"BranchNo, "
				"OrderNo, "
				"Status, "
				"OrderType, "
				"OrderPickingType, "
				"BookEntryType, "
				"OrderDate, "
				"OrderValue, "
				"DeferredPaymentDate, "
				"Containers, "
				"NarcoticsQty, "
				"TotalWeight, "
				"PharmacyNo, "
				"HeaderText, "
				"DeliveryRunReference, "
				"DeliveryRun, "
				"ReadoutStatus, "
				"DeliveryRunSequenceNo, "
				"CustomerGroup, "
				"SalesmanNo, "
				"IMSAreaNo, "
				"TaxType, "
				"OrderAcquisitionDate, "
				"DataSourceType, "
				"RecordType, "
				"BranchNoOrig, "
				"CustomerType, "
				"RekBNo, "
				"MainWarehouseFlag, "
				"EDIDeliveryDate, "
				"IsDeliveryNoteFlag, "
				"IsEDICopyFlag, "
				"IsLKMailFlag, "
				"EstorelFlashFlag, "
				"IsEstorelBulkOrderFlag, "
				"IsInvoicedFlag, "
				"SplittedIntoQty, "
				"Reference, "
				"OrderNoCsc, "
				"OrderNoMf, "
				"ManufacturerNo, "
				"EdiDeliveryDateMf, "
				"PharmacyNoMf, "
				"purchaseorderno"
				")";

			// Values
			basar::I18nString sqlStatementValues = " SELECT "
				"BranchNo, "
				"OrderNo, "
				"Status, "
				"OrderType, "
				"OrderPickingType, "
				"BookEntryType, "
				"OrderDate, "
				"OrderValue, "
				"DeferredPaymentDate, "
				"Containers, "
				"NarcoticsQty, "
				"TotalWeight, "
				"PharmacyNo, "
				"HeaderText, "
				"DeliveryRunReference, "
				"DeliveryRun, "
				"ReadoutStatus, "
				"DeliveryRunSequenceNo, "
				"CustomerGroup, "
				"SalesmanNo, "
				"IMSAreaNo, "
				"TaxType, "
				"OrderAcquisitionDate, "
				"DataSourceType, "
				"RecordType, "
				"BranchNoOrig, "
				"CustomerType, "
				"RekBNo, "
				"MainWarehouseFlag, "
				"EDIDeliveryDate, "
				"IsDeliveryNoteFlag, "
				"IsEDICopyFlag, "
				"IsLKMailFlag, "
				"EstorelFlashFlag, "
				"IsEstorelBulkOrderFlag, "
				"IsInvoicedFlag, "
				"SplittedIntoQty, "
				"Reference, "
				"OrderNoCsc, "
				"OrderNoMf, "
				"ManufacturerNo, "
				"EdiDeliveryDateMf, "
				"PharmacyNoMf, "
				"purchaseorderno";

			// FromWhere
			basar::I18nString sqlStatementFromWhere = " FROM orderhead WHERE branchno=#branchno# AND (status='2' OR status='9') ";
			if(0!=yit.getInt32("lessthanorderdate")){
				sqlStatementFromWhere.append(" AND orderdate <= #lessthanorderdate# ");
			}
			sqlStatementFromWhere.append(";");

			sqlStatement.append(sqlStatementInsert);
			sqlStatement.append(sqlStatementColumns);
			sqlStatement.append(sqlStatementValues);
			sqlStatement.append(sqlStatementFromWhere);
			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::saveArchiveBatchOrderHeads()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	//************************************ deleteBatchOrderHeads ************************************************
	BUILDER_DEFINITION(deleteBatchOrderHeads)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(deleteBatchOrderHeads,"branchno;lessthanorderdate;")

	//************ buildSQLString
	void deleteBatchOrderHeads::buildSQLString()
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::deleteBatchOrderHeads::buildSQLString().");
		try 
		{		
			basar::I18nString sqlStatement = "";
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			// Delete
			basar::I18nString sqlStatementDelete = "DELETE ";

			// FromWhere
			basar::I18nString sqlStatementFromWhere = " FROM orderhead WHERE branchno=#branchno# AND (status='2' OR status='9') ";
			if(0!=yit.getInt32("lessthanorderdate")) {
				sqlStatementFromWhere.append(" AND orderdate <= #lessthanorderdate# ");
			}
			sqlStatementFromWhere.append(";");

			sqlStatement.append(sqlStatementDelete);
			sqlStatement.append(sqlStatementFromWhere);

			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::deleteBatchOrderHeads()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	//************************************ saveArchiveBatchOrderPos ************************************************
	BUILDER_DEFINITION(saveArchiveBatchOrderPos)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(saveArchiveBatchOrderPos,"branchno;lessthanorderdate;")

	void saveArchiveBatchOrderPos::buildSQLString() 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::saveArchiveBatchOrderPos::buildSQLString().");
		try 
		{		
			basar::I18nString sqlStatement = "";
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			// Insert
			basar::I18nString sqlStatementInsert = "INSERT INTO aOrderPos";

			// Columns
			basar::I18nString sqlStatementColumns = "("
				"BranchNo"
				", OrderNo"
				", PosNo"
				", ArticleNo"
				", article_code"
				", OrderedQty"
				", NonChargedQty"
				", TextField"
				", ibtrecordtype"
				", selloffriseflag"
				", CustomerOrderNo" //FIXME mme: needed?
				", CustomerPosNo" //FIXME mme: needed?
				", subsequentdeliveryqty"
				", bookingflag"
				", WholesalerPurchasePrice"
				", subsequentdeliveryflag"
				", customerordernoadd"
				", estorelflashcode"
				", lkmailshortageqty"
				", DiscountPct"
				//TODO DZ ", DiscountType"
				", batch"
				", expirydate"
				", entrytype"
				", discountcalcfrom"
				", discountapplyto"
                ")";

			// Values
			basar::I18nString sqlStatementValues = " SELECT "
				"op.BranchNo"
				", op.OrderNo"
				", op.PosNo"
				", op.ArticleNo"
				", op.article_code"
				", op.OrderedQty"
				", op.NonChargedQty"
				", op.TextField" 
				", op.ibtrecordtype"
				", op.selloffriseflag"
				", op.CustomerOrderNo"  //FIXME mme: needed?
				", op.CustomerPosNo"  //FIXME mme: needed?
				", op.subsequentdeliveryqty"
				", op.bookingflag"
				", op.WholesalerPurchasePrice"
				", op.subsequentdeliveryflag"
				", op.customerordernoadd"
				", op.estorelflashcode"
				", op.lkmailshortageqty"
				", op.DiscountPct"
				//TODO DZ ", op.DiscountType"
				", op.batch"
				", op.expirydate"
				", op.entrytype"
				", op.discountcalcfrom"
				", op.discountapplyto"
				;

			// FromWhere
			basar::I18nString sqlStatementFromWhere = " FROM orderpos op LEFT JOIN orderhead oh ON op.branchno=oh.branchno AND op.orderno=oh.orderno WHERE op.branchno=#branchno# AND (oh.status='2' OR oh.status='9') ";
			if(0!=yit.getInt32("lessthanorderdate")) 
			{
				sqlStatementFromWhere.append(" AND oh.orderdate <= #lessthanorderdate# ");
			}

			sqlStatement.append(sqlStatementInsert);
			sqlStatement.append(sqlStatementColumns);
			sqlStatement.append(sqlStatementValues);
			sqlStatement.append(sqlStatementFromWhere);
			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::saveArchiveBatchOrderPos()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	BUILDER_DEFINITION(deleteBatchOrderPos)
	//************ isExecutable
	ENSURE_PROPERTIES_ARE_SET(deleteBatchOrderPos,"branchno;lessthanorderdate;")

	void deleteBatchOrderPos::buildSQLString() {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::deleteBatchOrderPos::buildSQLString().");
		try {		
			basar::I18nString sqlStatement = "";
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			// Delete
			basar::I18nString sqlStatementDelete = "DELETE ";

			// FromWherePos
			basar::I18nString sqlStatementFromWhere = " FROM orderpos WHERE branchno=#branchno# AND orderno in (SELECT orderno from orderhead WHERE branchno=#branchno# AND (status='2' OR status='9') ";
			if(0!=yit.getInt32("lessthanorderdate")) {
				sqlStatementFromWhere.append(" AND orderdate <= #lessthanorderdate# ");
			}
			sqlStatementFromWhere.append(");");

			sqlStatement.append(sqlStatementDelete);
			sqlStatement.append(sqlStatementFromWhere);
			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::deleteBatchOrderPos()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	BUILDER_DEFINITION(selectBatchOrderHead)
	ENSURE_PROPERTIES_ARE_SET(selectBatchOrderHead,"branchno;lessthanorderdate;")

	void selectBatchOrderHead::buildSQLString() 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::selectBatchOrderHead::buildSQLString().");

		try 
        {		
            basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			basar::I18nString sqlStatement = "SELECT branchno, orderno, headertext FROM orderhead WHERE branchno=#branchno# AND (status='2' OR status='9') ";

			if( 0 != yit.getInt32("lessthanorderdate") ) 
            {
				sqlStatement.append(" AND orderdate <= #lessthanorderdate# ");
			}

            sqlStatement.append( "ORDER BY orderno ASC ");

			resolve(sqlStatement);

			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::deleteBatchOrderPos()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	BUILDER_DEFINITION(selectBatchOrderPos)
	ENSURE_PROPERTIES_ARE_SET(selectBatchOrderPos,"branchno;lessthanorderdate;")

	void selectBatchOrderPos::buildSQLString() 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::selectBatchOrderPos::buildSQLString().");

		try 
        {		
            basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			basar::I18nString sqlStatement = "SELECT oh.branchno, oh.orderno, op.posno, op.articleno, op.orderedqty, op.nonchargedqty, op.discountpct, op.discountapplyto, op.discountcalcfrom, op.entrytype "
                              "FROM orderpos op LEFT JOIN orderhead oh ON op.branchno=oh.branchno AND op.orderno=oh.orderno WHERE op.branchno=#branchno# AND (oh.status='2' OR oh.status='9') ";

			//TODO DZ
			//basar::I18nString sqlStatement = "SELECT oh.branchno, oh.orderno, op.posno, op.articleno, op.orderedqty, op.nonchargedqty, op.discountpct, op.discountapplyto, op.discountcalcfrom, op.discounttype, op.entrytype "
            //                  "FROM orderpos op LEFT JOIN orderhead oh ON op.branchno=oh.branchno AND op.orderno=oh.orderno WHERE op.branchno=#branchno# AND (oh.status='2' OR oh.status='9') ";


			if( 0 != yit.getInt32("lessthanorderdate") ) 
            {
				sqlStatement.append(" AND orderdate <= #lessthanorderdate# ");
			}

             sqlStatement.append( "ORDER BY oh.orderno ASC ");

			resolve(sqlStatement);

			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
		}
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "copybatchorderacc::deleteBatchOrderPos()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}
	
	//TODO DZ add discounttype
	BEGIN_ACCESSOR_DEFINITION("CopyBatchOrdersAcc")
		PROPERTY_DESCRIPTION_LIST("\
			int16 branchno; \
			int32 lessthanorderdate; \
            int32 orderno; \
            string activitytype; \
            int16 eventtype; \
            int16 processedby; \
            int32 splitfrom; \
            int32 posno; \
			int32 articleno; \
            int32 orderedqty; \
            int32 nonchargedqty; \
            decimal discountpct; \
			int16 discountapplyto; \
			int16 discountcalcfrom; \
            string view; \
            int32 orderrequestqty; \
            string headertext; \
			string entrytype; \
			")

			ACCESS_METHOD("saveArchiveBatchOrderHeads")
			SQL_BUILDER_CREATE(saveArchiveBatchOrderHeads)
			SQL_BUILDER_PUSH_BACK(saveArchiveBatchOrderHeads)

			ACCESS_METHOD("deleteBatchOrderHeads")
			SQL_BUILDER_CREATE(deleteBatchOrderHeads)
			SQL_BUILDER_PUSH_BACK(deleteBatchOrderHeads)

			ACCESS_METHOD("saveArchiveBatchOrderPos")
			SQL_BUILDER_CREATE(saveArchiveBatchOrderPos)
			SQL_BUILDER_PUSH_BACK(saveArchiveBatchOrderPos)

			ACCESS_METHOD("deleteBatchOrderPos")
			SQL_BUILDER_CREATE(deleteBatchOrderPos)
			SQL_BUILDER_PUSH_BACK(deleteBatchOrderPos)

            //Needed for protocol! Otherwise we don't know what has been archived!
            ACCESS_METHOD("selectBatchOrderHead")
			SQL_BUILDER_CREATE(selectBatchOrderHead)
			SQL_BUILDER_PUSH_BACK(selectBatchOrderHead)

            ACCESS_METHOD("selectBatchOrderPos")
			SQL_BUILDER_CREATE(selectBatchOrderPos)
			SQL_BUILDER_PUSH_BACK(selectBatchOrderPos)
			
	END_ACCESSOR_DEFINITION			
}

