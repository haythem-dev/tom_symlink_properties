#include "cmnaccessor.h"

#include "transferorderheadcollectionacc.h"
#include "transferorderheadcollection_definitions.h"
#include "orderstatus.h"
#include <libbasarcmnutil_logging.h>
#include <libbasarcmnutil_datetime.h>

namespace libabbauw
{
	namespace acc_transfer_order_head_collection
	{
		using namespace libabbauw::properties;
		using basar::VarString;

		BEGIN_ACCESSOR_DEFINITION(domMod::transferOrder::ACC_TRANSFER_ORDERS)

			BEGIN_PROPERTY_DESCRIPTION_LIST
				PROPERTY_DESCRIPTION_LIST_ADD(BRANCHNO);
				PROPERTY_DESCRIPTION_LIST_ADD(MANUFACTURERNO);
				PROPERTY_DESCRIPTION_LIST_ADD(MANUFACTURERNAME);
				PROPERTY_DESCRIPTION_LIST_ADD(PZN);
				PROPERTY_DESCRIPTION_LIST_ADD(PHARMACYNO);
				PROPERTY_DESCRIPTION_LIST_ADD(PHARMACYNAME);
				PROPERTY_DESCRIPTION_LIST_ADD(HAS_TELEPHONY_INFO);
				PROPERTY_DESCRIPTION_LIST_ADD(LOCATION);
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERNO);
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERTYPE);
				PROPERTY_DESCRIPTION_LIST_ADD(DISCOUNTPCT);
				PROPERTY_DESCRIPTION_LIST_ADD(DISCOUNTPCTRANGE);
				PROPERTY_DESCRIPTION_LIST_ADD(STATUS);
				PROPERTY_DESCRIPTION_LIST_ADD(POSQTY);
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERDATE);
				PROPERTY_DESCRIPTION_LIST_ADD(EDIDELIVERYDATE);
				PROPERTY_DESCRIPTION_LIST_ADD(EDIDELIVERYDATEMF);
				PROPERTY_DESCRIPTION_LIST_ADD(NOTICE);
				PROPERTY_DESCRIPTION_LIST_ADD(LARGESCALEORDER);
				PROPERTY_DESCRIPTION_LIST_ADD(NONSTOCKITEM);
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERREQUEST);
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERVALUE);
				PROPERTY_DESCRIPTION_LIST_ADD(SHORTFALLPOS);
				PROPERTY_DESCRIPTION_LIST_ADD(COLOR);	// Farbe eines Auftragsampelfelds
				PROPERTY_DESCRIPTION_LIST_ADD(TRAFFIC_LIGHT_INFO);
				PROPERTY_DESCRIPTION_LIST_ADD(DATASOURCETYPE);
				PROPERTY_DESCRIPTION_LIST_ADD(HEADERTEXT);
				PROPERTY_DESCRIPTION_LIST_ADD(REFERENCE); // needed for ordersplit
				PROPERTY_DESCRIPTION_LIST_ADD(ORDERNOCSC); // needed for protocol
				PROPERTY_DESCRIPTION_LIST_ADD(DEFERREDPAYMENTDATE);
				PROPERTY_DESCRIPTION_LIST_ADD(AGP);
				PROPERTY_DESCRIPTION_LIST_ADD(GEP);
			END_PROPERTY_DESCRIPTION_LIST

			ACCESS_METHOD(domMod::transferOrder::SELECT_TRANSFER_ORDERS)
			SQL_BUILDER_CREATE(SelectTransferOrders)
			SQL_BUILDER_PUSH_BACK(SelectTransferOrders)

			ACCESS_METHOD(domMod::transferOrder::SELECT_TRANSFER_ORDER_TOTAL_NUMBER)
			SQL_BUILDER_CREATE(SelectTransferOrderTotalNumber)
			SQL_BUILDER_PUSH_BACK(SelectTransferOrderTotalNumber)

			ACCESS_METHOD(domMod::transferOrder::SELECT_TRANSFER_ORDER_BY_KEY)
			SQL_BUILDER_CREATE(SelectTransferOrderByKey)
			SQL_BUILDER_PUSH_BACK(SelectTransferOrderByKey)

			END_ACCESSOR_DEFINITION

			BUILDER_DEFINITION(SelectTransferOrders)
			// Description SelectTransferOrders:
			// Gets the data of all by one columns 
			// for all orders in the transfer-order gui-screen.
			// The data for the column shortfall-pos including the color-settings
			// is not acquired here.

			ENSURE_PROPERTIES_ARE_SET(SelectTransferOrders, BRANCHNO.toPropertyString())

			void SelectTransferOrders::buildSQLString()
		{
			static const VarString simpleWhereCondition(
				" WHERE oh.ordertype <>'" + lit::NO_ORDERTYPE +
				"' AND oh.branchno = " + BRANCHNO.toSQLReplacementString()
			);

			VarString sqlStatement("\n"
				"SELECT DISTINCT oh.branchno AS " + BRANCHNO.getName() +
				", oh.manufacturerno AS " + MANUFACTURERNO.getName() +
				", NVL(h.sortiername, '') AS " + MANUFACTURERNAME.getName() +
				", oh.pharmacyno AS " + PHARMACYNO.getName() +
				",\n\t k.nameapo AS " + PHARMACYNAME.getName() +
				", k.ort AS " + LOCATION.getName() +
				", (CASE WHEN (k.kundetext MATCHES '!*') THEN 1 ELSE 0 END)::SMALLINT " + HAS_TELEPHONY_INFO.getName() +
				", \n\t oh.orderno AS " + ORDERNO.getName() +
				", oh.ordertype || oh.orderpickingtype || oh.bookentrytype AS " + ORDERTYPE.getName() +
				",\nCASE WHEN oh.status='0' THEN 'ZU' "
				"\n     WHEN oh.status='1' THEN 'FE' "
				"\n     WHEN oh.status='2' THEN 'ER' "
				"\n     WHEN oh.status='9' THEN 'ST' "
				"\n     ELSE '?' "
				"\nEND AS " + STATUS.getName() +
				", \nCASE WHEN oh.status='0' THEN '1' "
				"\n     WHEN oh.status='1' THEN '0' "
				"\nEND AS sortstatus"
				", opa.posqty AS " + POSQTY.getName() +
				", \nTO_DATE( CASE WHEN oh.orderdate = 0 THEN " NULLVAL_SQL_DATE_STR
				" ELSE TO_CHAR(oh.orderdate) END," FORMAT_SQL_DATE_IFX")::DATE AS " + ORDERDATE.getName() +
				", \nTO_DATE( CASE WHEN oh.EDIDeliveryDate = 0 THEN " NULLVAL_SQL_DATE_STR
				" ELSE TO_CHAR(oh.EDIDeliveryDate) END," FORMAT_SQL_DATE_IFX")::DATE AS " + EDIDELIVERYDATE.getName() +
				", \nTO_DATE( CASE WHEN oh.edideliverydatemf = 0 THEN " NULLVAL_SQL_DATE_STR
				" ELSE TO_CHAR(oh.edideliverydatemf) END," FORMAT_SQL_DATE_IFX")::DATE AS " + EDIDELIVERYDATEMF.getName() +
				", opa.ordervalue AS " + ORDERVALUE.getName() +
				", \nCASE WHEN opa.narcotic = 1 THEN 'N' "                                                   // TODO remove 'N' and 'K' and use enum instead
				"\n     WHEN opa.refrigerated > 0 THEN 'K' END AS " + NOTICE.getName() +
				", opa.nonstockitem AS " + NONSTOCKITEM.getName() +
				", opa.orderrequest AS " + ORDERREQUEST.getName() +
				", \nCASE WHEN opa.largestPosQty >= " + LARGESCALEORDERLIMIT.toSQLReplacementString() +
				"\n     THEN opa.largestPosQty END AS " + LARGESCALEORDER.getName() +
				", oh.datasourcetype AS " + DATASOURCETYPE.getName() +
				", oh.headertext AS " + HEADERTEXT.getName() +
				", oh.reference AS " + REFERENCE.getName() +
				", \nTO_DATE( CASE WHEN oh.deferredpaymentdate = 0 THEN " NULLVAL_SQL_DATE_STR
				" ELSE TO_CHAR(oh.deferredpaymentdate) END," FORMAT_SQL_DATE_IFX")::DATE AS " + DEFERREDPAYMENTDATE.getName() +
				" \nFROM orderhead oh");

			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			//Append this left join only if PZN is not empty (performance reasons)!
			if (isContainedAndSet(PZN) && !yit.getString(PZN).empty())
			{
				sqlStatement.append(" \nLEFT JOIN orderpos op ON oh.orderno = op.orderno");
			}

			sqlStatement.append(
				" \nLEFT JOIN manufacturer h ON oh.manufacturerno = h.hersteller_nr"
				" \nLEFT JOIN kunde  k ON oh.pharmacyno = k.idfnr AND oh.branchno = k.filialnr"
				" \nLEFT JOIN ("
				" SELECT  oh.branchno"
				", oh.orderno"
				", COUNT(op.posno)::INT AS posqty"
				",\n\t SUM(op.wholesalerpurchaseprice * op.orderedqty) AS ordervalue"
				",\n\t MAX(op.orderedqty) AS largestPosQty"
				",\n\t MAX(TRUNC(az.etartklasse1 / 512) - 2 * TRUNC(az.etartklasse1 / (512 * 2) ))::INT AS narcotic"
				",\n\t MAX(TRUNC(al.etartschalter1 /  32)  - 2 * TRUNC(al.etartschalter1 / ( 32 * 2)) "
				"\n\t   + TRUNC(al.etartschalter1 /  64)  - 2 * TRUNC(al.etartschalter1 / ( 64 * 2)) "
				"\n\t   + TRUNC(al.etartschalter1 / 128)  - 2 * TRUNC(al.etartschalter1 / (128 * 2)))::INT AS refrigerated"
				",\n\t MAX(CASE WHEN al.lagerfachnr IS NULL THEN 1"
				"\n\t          WHEN al.lagerfachnr = (SELECT pseudolagerort FROM pseudolagerorte WHERE filialnr = oh.branchno) THEN 1"
				"\n\t          ELSE 0"
				"\n\t          END)::smallint AS nonstockitem"
				",\n\t COUNT(oprop.posnobatch)::INT AS orderrequest"
				"\n\t   FROM orderhead oh"
				"\n\t   JOIN orderpos op ON op.branchno = oh.branchno AND op.orderno = oh.orderno"
				"\n\t   LEFT JOIN orderproposal oprop ON op.branchno = oprop.branchno AND op.orderno = oprop.ordernobatch AND op.posno = oprop.posnobatch"
				"\n\t   LEFT JOIN artikelzentral az ON op.articleno = az.artikel_nr"
				"\n\t   LEFT JOIN artikellokal al ON (op.articleno = al.artikel_nr AND op.branchno = al.filialnr)"
				+ simpleWhereCondition +
				"\n\t   GROUP BY oh.branchno, oh.orderno) AS opa"
				"\n ON opa.branchno = oh.branchno AND opa.orderno = oh.orderno\n"
				+ simpleWhereCondition
			);


			basar::VarString whereClause;
			if (isContainedAndSet(MANUFACTURERNO))
				whereClause.append(" AND oh.manufacturerno = " + MANUFACTURERNO.toSQLReplacementString());

			if (isContainedAndSet(MANUFACTURERNAME) && !yit.getString(MANUFACTURERNAME).empty())
				whereClause.append(" AND NVL(h.sortiername, '') LIKE UPPER('%" + MANUFACTURERNAME.toSQLReplacementString() + "%')");

			if (isContainedAndSet(PZN) && !yit.getString(PZN).empty())
			{
				whereClause.append(" AND (op.article_code = '" + PZN.toSQLReplacementString() +
					"' OR op.articleno = (SELECT articleno FROM articlecodes WHERE article_code = '" + PZN.toSQLReplacementString() + "'))");
			}

			if (isContainedAndSet(PHARMACYNO))
				whereClause.append(" AND oh.pharmacyno = " + PHARMACYNO.toSQLReplacementString());

			if (isContainedAndSet(PHARMACYNAME) && !yit.getString(PHARMACYNAME).empty())
				whereClause.append(" AND k.nameapo LIKE UPPER('%" + PHARMACYNAME.toSQLReplacementString() + "%')");

			if (isContainedAndSet(LOCATION) && !yit.getString(LOCATION).empty())
				whereClause.append(" AND k.ort LIKE UPPER('%" + LOCATION.toSQLReplacementString() + "%')");

			if (isContainedAndSet(ORDERNO_FROM))
				whereClause.append(" AND oh.orderno >= " + ORDERNO_FROM.toSQLReplacementString());

			if (isContainedAndSet(ORDERNO_TO))
				whereClause.append(" AND oh.orderno <= " + ORDERNO_TO.toSQLReplacementString());

			if (isContainedAndSet(POS_FROM))
				whereClause.append(" AND opa.posqty >= " + POS_FROM.toSQLReplacementString());

			if (isContainedAndSet(POS_TO))
				whereClause.append(" AND opa.posqty <= " + POS_TO.toSQLReplacementString());

			if (isContainedAndSet(ORDERVALUE_FROM))
				whereClause.append(" AND opa.ordervalue >= " + ORDERVALUE_FROM.toSQLReplacementString());

			if (isContainedAndSet(ORDERVALUE_TO))
				whereClause.append(" AND opa.ordervalue <= " + ORDERVALUE_TO.toSQLReplacementString());

			if (isContainedAndSet(ORDERTYPE) && yit.getString(ORDERTYPE).length() == 4)
			{
				basar::I18nString ot = yit.getString(ORDERTYPE.getName());
				basar::I18nString orderType = ot.substr(0, 2);
				char orderPickingType = ot[2];
				char bookEntryType = ot[3];
				whereClause.append(" AND oh.ordertype = '" + orderType + "' AND oh.orderpickingtype = '" + orderPickingType +
					"' AND bookentrytype = '" + bookEntryType + "'");
			}

			if (isContainedAndSet(STATUS))
			{
				libabbauw::domMod::OrderStatus status(yit.getString(STATUS.getName()));
				std::ostringstream s;
				s << basar::I18nString(" AND oh.status = '") << status.getStatus() << "'";
				whereClause.append(s.str().c_str());
			}

			if (isContainedAndSet(ORDERDATE_FROM))
			{
				basar::I18nString date = basarDateToDBDateStr(yit.getDate(ORDERDATE_FROM.getName()));
				whereClause.append(" AND ( oh.orderdate = 0 OR oh.orderdate >= " + date + ")");
			}

			if (isContainedAndSet(ORDERDATE_TO))
			{
				basar::I18nString date = basarDateToDBDateStr(yit.getDate(ORDERDATE_TO));
				whereClause.append(" AND ( oh.orderdate = 0 OR oh.orderdate <= " + date + ")");
			}

			if (isContainedAndSet(EDIDELIVERYDATE_FROM))
			{
				basar::I18nString date = basarDateToDBDateStr(yit.getDate(EDIDELIVERYDATE_FROM));
				whereClause.append(" AND ( oh.edideliverydate = 0 OR oh.edideliverydate >= " + date + ")");
			}

			if (isContainedAndSet(EDIDELIVERYDATE_TO))
			{
				basar::I18nString date = basarDateToDBDateStr(yit.getDate(EDIDELIVERYDATE_TO));
				whereClause.append(" AND ( oh.edideliverydate = 0 OR oh.edideliverydate <= " + date + ")");
			}

			if (isContainedAndSet(DEFERREDPAYMENTDATE_FROM))
			{
				basar::I18nString date = basarDateToDBDateStr(yit.getDate(DEFERREDPAYMENTDATE_FROM));
				whereClause.append(" AND ( oh.deferredpaymentdate = 0 OR oh.deferredpaymentdate >= " + date + ")");
			}

			if (isContainedAndSet(DEFERREDPAYMENTDATE_TO))
			{
				basar::I18nString date = basarDateToDBDateStr(yit.getDate(DEFERREDPAYMENTDATE_TO));
				whereClause.append(" AND ( oh.deferredpaymentdate = 0 OR oh.deferredpaymentdate <= " + date + ")");
			}

			if (isContainedAndSet(NONSTOCKITEM))
			{
				whereClause.append(" AND opa.nonstockitem = " + NONSTOCKITEM.toSQLReplacementString());
			}

			if (isContainedAndSet(LARGESCALEORDER)) // filter is set
			{
				if (yit.getInt32(LARGESCALEORDER) == 1) // filter positions with exceed limit
					whereClause.append(" AND opa.largestPosQty >= " + LARGESCALEORDERLIMIT.toSQLReplacementString());
				else // filter positions which are below limit
					whereClause.append(" AND opa.largestPosQty < " + LARGESCALEORDERLIMIT.toSQLReplacementString());
			}

			basar::VarString orderStatement;
			if (isContainedAndSet(SORT))
			{
				orderStatement = "\nORDER BY " + yit.getString(SORT.getName());
				// Column status is not sortable by default!
				orderStatement.exchange(STATUS.getName(), "sortstatus");
			}

			resolve(sqlStatement + whereClause + orderStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerTransferOrder);
		}

		basar::I18nString SelectTransferOrders::basarDateToDBDateStr(const basar::Date& date)
		{
			basar::I18nString dateStr;
			dateStr.itos(date.getDate());

			return dateStr;
		}


		BUILDER_DEFINITION(SelectTransferOrderTotalNumber)

			ENSURE_PROPERTIES_ARE_SET(SelectTransferOrderTotalNumber, BRANCHNO.toPropertyString())

			void SelectTransferOrderTotalNumber::buildSQLString()
		{
			static const VarString sqlStatement("\n"
				"SELECT COUNT(*) FROM orderhead WHERE "
				"(   ordertype = '" + lit::UW_ORDERTYPE + "'"
				" OR ordertype = '" + lit::OMG_ORDERTYPE + "'"
				" OR ordertype = '" + lit::UN_ORDERTYPE + "'"
				" OR ordertype = '" + lit::MD_ORDERTYPE + "'"
				" OR ordertype = '" + lit::AK_ORDERTYPE + "'"
				" OR ordertype = '" + lit::HZ_ORDERTYPE + "'"
				" OR ordertype = '" + lit::DG_ORDERTYPE + "'"
				" OR ordertype = '" + lit::GM_ORDERTYPE + "'"
				") AND branchno = " + BRANCHNO.toSQLReplacementString()
			);

			resolve(sqlStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerTransferOrder);
		}

		//////////////////////////////////////////////////////////////////////////////////
		BUILDER_DEFINITION(SelectTransferOrderByKey)

			ENSURE_PROPERTIES_ARE_SET(SelectTransferOrderByKey, BRANCHNO.toPropertyString())

			void SelectTransferOrderByKey::buildSQLString()
		{
			static const VarString sqlStatement(
				"SELECT "
				"branchno AS " + BRANCHNO.getName() + ", "
				"orderno AS " + ORDERNO.getName() + ", "
				"ordernocsc AS " + ORDERNOCSC.getName() + " "
				"FROM orderhead "
				"WHERE "
				"branchno = " + BRANCHNO.toSQLReplacementString() +
				"AND orderno = " + ORDERNO.toSQLReplacementString()
			);

			resolve(sqlStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerTransferOrder);
		}

	} // end namespace acc_transfer_order_head_collection
} // end namespace libabbauw
