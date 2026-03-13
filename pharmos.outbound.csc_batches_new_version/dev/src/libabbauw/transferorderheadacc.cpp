#include "cmnaccessor.h"
#include "orderstatus.h"

#include "transferorderheadacc.h"
#include "transferorderhead_definitions.h"
#include <libbasarcmnutil_logging.h>

#include "transferorderheadpropertystatechecker.h"

namespace libabbauw
{
	namespace acc_transfer_order_head
	{
		using namespace libabbauw::properties;

		BEGIN_ACCESSOR_DEFINITION(domMod::transferOrder::ACC_TRANSFER_ORDER_HEAD)

			BEGIN_PROPERTY_DESCRIPTION_LIST
				PROPERTY_DESCRIPTION_LIST_ADD(BRANCHNO);
				PROPERTY_DESCRIPTION_LIST_ADD(DATASOURCETYPE);
				PROPERTY_DESCRIPTION_LIST_ADD(MANUFACTURERNO);
				PROPERTY_DESCRIPTION_LIST_ADD(MANUFACTURERNAME);
				PROPERTY_DESCRIPTION_LIST_ADD(PHARMACYNO);
				PROPERTY_DESCRIPTION_LIST_ADD(PHARMACYNOMF);
				PROPERTY_DESCRIPTION_LIST_ADD(PHARMACYNAME);
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERNO);
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERNOMF);
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERTYPE);
				PROPERTY_DESCRIPTION_LIST_ADD(DISCOUNTPCT);
				PROPERTY_DESCRIPTION_LIST_ADD(DISCOUNTPCTRANGE);
				PROPERTY_DESCRIPTION_LIST_ADD(STATUS);
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERDATE);
				PROPERTY_DESCRIPTION_LIST_ADD(EDIDELIVERYDATE);
				PROPERTY_DESCRIPTION_LIST_ADD(EDIDELIVERYDATEMF);
				PROPERTY_DESCRIPTION_LIST_ADD(DEFERREDPAYMENTDATE);
				PROPERTY_DESCRIPTION_LIST_ADD(COLOR);  // color of this single order
				PROPERTY_DESCRIPTION_LIST_ADD(HEADERTEXT);
				PROPERTY_DESCRIPTION_LIST_ADD(REFERENCE); // needed for ordersplit
				PROPERTY_DESCRIPTION_LIST_ADD(PURCHASEORDERNO);
			END_PROPERTY_DESCRIPTION_LIST

			ACCESS_METHOD(domMod::transferOrder::SELECT_TRANSFER_ORDER_HEAD)
			SQL_BUILDER_CREATE(SelectTransferOrderHead)
			SQL_BUILDER_PUSH_BACK(SelectTransferOrderHead)

			ACCESS_METHOD(domMod::transferOrder::SAVE_TRANSFER_ORDER_HEAD)
			SQL_BUILDER_CREATE(UpdateTransferOrderHead)
			SQL_BUILDER_PUSH_BACK(UpdateTransferOrderHead)
			SQL_BUILDER_CREATE(InsertTransferOrderHead)
			SQL_BUILDER_PUSH_BACK(InsertTransferOrderHead)
			SQL_BUILDER_CREATE(DeleteOrderHead)
			SQL_BUILDER_PUSH_BACK(DeleteOrderHead)

			END_ACCESSOR_DEFINITION

			BUILDER_DEFINITION(SelectTransferOrderHead)

			bool SelectTransferOrderHead::isExecutable() const
		{
			accessor::transferOrder::HeadPropertyStateChecker checker(getPropertyList());
			return checker.isSelectable();
		}

		void SelectTransferOrderHead::buildSQLString()
		{
			basar::VarString sqlStatement("\n"
				"SELECT oh.branchno AS " + BRANCHNO.getName() +
				", oh.manufacturerno AS " + MANUFACTURERNO.getName() +
				", NVL(h.sortiername, '') AS " + MANUFACTURERNAME.getName() +
				", oh.pharmacyno AS " + PHARMACYNO.getName() +
				", oh.pharmacynomf AS " + PHARMACYNOMF.getName() +
				",\n\t k.nameapo AS " + PHARMACYNAME.getName() +
				", oh.purchaseorderno AS " + PURCHASEORDERNO.getName() +
				", oh.orderno AS " + ORDERNO.getName() +
				", oh.ordernomf AS " + ORDERNOMF.getName() +
				", oh.ordertype || oh.orderpickingtype || oh.bookentrytype AS " + ORDERTYPE.getName() +
				",\nCASE WHEN oh.status='0' THEN 'ZU' "
				"\n     WHEN oh.status='1' THEN 'FE' "
				"\n     WHEN oh.status='2' THEN 'ER' "
				"\n     WHEN oh.status='9' THEN 'ST' "
				"\n     ELSE '?' "
				"\nEND AS " + STATUS.getName() +
				", \nTO_DATE( CASE WHEN oh.orderdate = 0 THEN " NULLVAL_SQL_DATE_STR
				" ELSE TO_CHAR(oh.orderdate) END," FORMAT_SQL_DATE_IFX")::DATE AS " + ORDERDATE.getName() +
				", \nTO_DATE( CASE WHEN oh.EDIDeliveryDate = 0 THEN " NULLVAL_SQL_DATE_STR
				" ELSE TO_CHAR(oh.EDIDeliveryDate) END," FORMAT_SQL_DATE_IFX")::DATE AS " + EDIDELIVERYDATE.getName() +
				", \nTO_DATE( CASE WHEN oh.edideliverydatemf = 0 THEN " NULLVAL_SQL_DATE_STR
				" ELSE TO_CHAR(oh.edideliverydatemf) END," FORMAT_SQL_DATE_IFX")::DATE AS " + EDIDELIVERYDATEMF.getName() +
				", \nTO_DATE( CASE WHEN oh.deferredpaymentdate = 0 THEN " NULLVAL_SQL_DATE_STR
				" ELSE TO_CHAR(oh.deferredpaymentdate) END," FORMAT_SQL_DATE_IFX")::DATE AS " + DEFERREDPAYMENTDATE.getName() +
				", oh.datasourcetype AS " + DATASOURCETYPE.getName() +
				", oh.headertext AS " + HEADERTEXT.getName() +
				", oh.reference AS " + REFERENCE.getName() +
				" \nFROM orderhead oh"
				" \nLEFT JOIN manufacturer h ON oh.manufacturerno = h.hersteller_nr"
				" \nLEFT JOIN kunde  k ON oh.pharmacyno = k.idfnr AND oh.branchno = k.filialnr"
				" WHERE oh.ordertype <> '" + lit::NO_ORDERTYPE + "'"
				" AND oh.branchno = " + BRANCHNO.toSQLReplacementString() +
				" AND oh.orderno = " + ORDERNO.toSQLReplacementString()
			);

			resolve(sqlStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerTransferOrder);
		}

		BUILDER_DEFINITION(UpdateTransferOrderHead)
			bool UpdateTransferOrderHead::isExecutable() const
		{
			accessor::transferOrder::HeadPropertyStateChecker checker(getPropertyList());
			return checker.isUpdatable();
		}

		void UpdateTransferOrderHead::buildSQLString()
		{
			const basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
			const domMod::OrderStatus orderStatus(yit.getString(properties::STATUS));
			basar::I18nString status; status.itos(orderStatus.getStatus());

			basar::VarString sqlStatement(
				"UPDATE orderhead SET "
				"pharmacyno = '" + PHARMACYNO.toSQLReplacementString("", "'") +
				", ordertype = SUBSTRING(" + ORDERTYPE.toSQLReplacementString("'", "'") + " FROM 1 FOR 2)"
				", orderpickingtype = SUBSTRING(" + ORDERTYPE.toSQLReplacementString("'", "'") + " FROM 3 FOR 1)::INT"
				", bookentrytype = SUBSTRING(" + ORDERTYPE.toSQLReplacementString("'", "'") + " FROM 4 FOR 1)::INT"
				", status = '" + status +
				"', datasourcetype = " + DATASOURCETYPE.toSQLReplacementString("'", "'")
			);
			if (getPropertyList().isContainedAndSet(EDIDELIVERYDATE) && yit.getDate(EDIDELIVERYDATE).isValid())
				sqlStatement.append(
					", edideliverydate = CAST( TO_CHAR( TO_DATE( '" + EDIDELIVERYDATE.toSQLReplacementString() +
					"', '%m/%d/%Y')::DATE, '%Y%m%d') AS INTEGER )");
			else
				sqlStatement.append(", edideliverydate = 0");

			if (getPropertyList().isContainedAndSet(DEFERREDPAYMENTDATE) && yit.getDate(DEFERREDPAYMENTDATE).isValid())
				sqlStatement.append(
					", deferredpaymentdate = CAST( TO_CHAR( TO_DATE( '" + DEFERREDPAYMENTDATE.toSQLReplacementString() +
					"', '%m/%d/%Y')::DATE, '%Y%m%d') AS INTEGER )");
			else
				sqlStatement.append(", deferredpaymentdate = 0");

			sqlStatement.append(
				", orderdate = CAST( TO_CHAR( TO_DATE( '" + ORDERDATE.toSQLReplacementString() +
				"', '%m/%d/%Y')::DATE, '%Y%m%d') AS INTEGER )"
				", manufacturerno = '" + MANUFACTURERNO.toSQLReplacementString() +
				"' WHERE orderno = " + ORDERNO.toSQLReplacementString() +
				"  AND branchno = " + BRANCHNO.toSQLReplacementString()
			);

			resolve(sqlStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerTransferOrder);
		}

		BUILDER_DEFINITION(InsertTransferOrderHead)

			bool InsertTransferOrderHead::isExecutable() const
		{
			accessor::transferOrder::HeadPropertyStateChecker checker(getPropertyList());
			return checker.isInsertable();
		}

		void InsertTransferOrderHead::buildSQLString()
		{
			const basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
			const domMod::OrderStatus orderStatus(yit.getString(properties::STATUS));
			basar::I18nString status; status.itos(orderStatus.getStatus());

			const basar::VarString sqlStatement("\n"
				"INSERT INTO orderhead "
				"(branchno, pharmacyno, ordertype, orderpickingtype, bookentrytype, status, datasourcetype, "
				"orderdate, manufacturerno, edideliverydate, deferredpaymentdate, headertext, reference, ordernomf, edideliverydatemf, pharmacynomf) \n"
				"VALUES(" + BRANCHNO.toSQLReplacementString() + ", " +
				PHARMACYNO.toSQLReplacementString() + ", " +
				"SUBSTRING(" + ORDERTYPE.toSQLReplacementString("'", "'") + " FROM 1 FOR 2)" + ", " +
				"SUBSTRING(" + ORDERTYPE.toSQLReplacementString("'", "'") + " FROM 3 FOR 1)::INT" + ", " +
				"SUBSTRING(" + ORDERTYPE.toSQLReplacementString("'", "'") + " FROM 4 FOR 1)::INT" + ", " +
				status + ", " +
				DATASOURCETYPE.toSQLReplacementString("'", "'") + ", " +
				"CAST( TO_CHAR( TO_DATE( '" + ORDERDATE.toSQLReplacementString() + "', '%m/%d/%Y')::DATE, '%Y%m%d') AS INTEGER )" + ", " +
				MANUFACTURERNO.toSQLReplacementString()
			);

			basar::VarString variableStatement;

			if (yit.isContainedAndSet(EDIDELIVERYDATE) && yit.getDate(EDIDELIVERYDATE).isValid())
				variableStatement.append(
					", CAST( TO_CHAR( TO_DATE( '" + EDIDELIVERYDATE.toSQLReplacementString() + "', '%m/%d/%Y')::DATE, '%Y%m%d') AS INTEGER )");
			else
				variableStatement.append(", 0");

			if (yit.isContainedAndSet(DEFERREDPAYMENTDATE) && yit.getDate(DEFERREDPAYMENTDATE).isValid())
				variableStatement.append(
					", CAST( TO_CHAR( TO_DATE( '" + DEFERREDPAYMENTDATE.toSQLReplacementString() + "', '%m/%d/%Y')::DATE, '%Y%m%d') AS INTEGER )");
			else
				variableStatement.append(", 0");

			if (yit.isContainedAndSet(HEADERTEXT))
				variableStatement.append(", " + HEADERTEXT.toSQLReplacementString("'", "'"));
			else
				variableStatement.append(", ''");

			if (yit.isContainedAndSet(REFERENCE))
				variableStatement.append(", " + REFERENCE.toSQLReplacementString("'", "'"));
			else
				variableStatement.append(", ''");

			if (yit.isContainedAndSet(ORDERNOMF))
				variableStatement.append(", " + ORDERNOMF.toSQLReplacementString("'", "'"));
			else
				variableStatement.append(", ''");

			if (yit.isContainedAndSet(EDIDELIVERYDATEMF) && yit.getDate(EDIDELIVERYDATEMF).isValid())
				variableStatement.append(
					", CAST( TO_CHAR( TO_DATE( '" + EDIDELIVERYDATEMF.toSQLReplacementString() + "', '%m/%d/%Y')::DATE, '%Y%m%d') AS INTEGER )");
			else
				variableStatement.append(", 0");

			if (yit.isContainedAndSet(PHARMACYNOMF))
				variableStatement.append(", " + PHARMACYNOMF.toSQLReplacementString("'", "'"));
			else
				variableStatement.append(", ''");

			variableStatement.append(")");

			resolve(sqlStatement + variableStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerTransferOrder);
		}

		BUILDER_DEFINITION(DeleteOrderHead)

			bool DeleteOrderHead::isExecutable() const
		{
			accessor::transferOrder::HeadPropertyStateChecker checker(getPropertyList());
			return checker.isDeletable();
		}

		//!< throw no-throw build sql
		void DeleteOrderHead::buildSQLString()
		{
			const basar::VarString sqlStatement(
				"DELETE FROM orderhead "
				" WHERE orderno = " + ORDERNO.toSQLReplacementString() +
				" AND branchno = " + BRANCHNO.toSQLReplacementString()
			);

			resolve(sqlStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerTransferOrder);
		}
	} // end namespace acc_transfer_order_head_collection
} // end namespace libabbauw
