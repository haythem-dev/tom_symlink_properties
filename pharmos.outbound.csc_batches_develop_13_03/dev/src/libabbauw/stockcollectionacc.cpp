#include "cmnaccessor.h"

#include "stockcollectionacc.h"
#include "stockcollection_definitions.h"
#include <libbasarcmnutil_logging.h>

#include "libabbauw_definitions.h"

namespace libabbauw
{
	namespace acc_stock
	{
		using namespace libabbauw::properties;
		using basar::VarString;

		BEGIN_ACCESSOR_DEFINITION(domMod::stock::ACC_STOCK)

		BEGIN_PROPERTY_DESCRIPTION_LIST
			PROPERTY_DESCRIPTION_LIST_ADD(BRANCHNO);
			PROPERTY_DESCRIPTION_LIST_ADD(ORDERNO);
			PROPERTY_DESCRIPTION_LIST_ADD(ORDERTYPE);
			PROPERTY_DESCRIPTION_LIST_ADD(ARTICLENO);
			PROPERTY_DESCRIPTION_LIST_ADD(POSNO);
			PROPERTY_DESCRIPTION_LIST_ADD(STOCK);
			PROPERTY_DESCRIPTION_LIST_ADD(STOCKACCUMULATED);
			PROPERTY_DESCRIPTION_LIST_ADD(ORDEREDQTY);
			PROPERTY_DESCRIPTION_LIST_ADD(DISCOUNTPCT);
			PROPERTY_DESCRIPTION_LIST_ADD(BACKORDER);
			PROPERTY_DESCRIPTION_LIST_ADD(BACKORDER_NOT_EFFECTIVE);
			PROPERTY_DESCRIPTION_LIST_ADD(MINSTOCK);
			PROPERTY_DESCRIPTION_LIST_ADD(RESERVEDQTY);
			PROPERTY_DESCRIPTION_LIST_ADD(CMIRESERVEDQTY);
			PROPERTY_DESCRIPTION_LIST_ADD(PREDICTION);
			PROPERTY_DESCRIPTION_LIST_ADD(STORAGELOCATIONSLOT);
			PROPERTY_DESCRIPTION_LIST_ADD(COLOR);
			PROPERTY_DESCRIPTION_LIST_ADD(PSEUDOSTORAGELOCATIONSLOT);
		END_PROPERTY_DESCRIPTION_LIST

		ACCESS_METHOD(domMod::stock::SELECT_PSEUDO_STORAGE_LOCATION_SLOT)
		SQL_BUILDER_CREATE(SelectPseudoStorageLocationSlot)
		SQL_BUILDER_PUSH_BACK(SelectPseudoStorageLocationSlot)

		ACCESS_METHOD(domMod::stock::SELECT_PREDICTION_DATE)
		SQL_BUILDER_CREATE(SelectPredictionDate)
		SQL_BUILDER_PUSH_BACK(SelectPredictionDate)

		ACCESS_METHOD(domMod::stock::SELECT_STOCK)
		SQL_BUILDER_CREATE(SelectStock)
		SQL_BUILDER_PUSH_BACK(SelectStock)

		ACCESS_METHOD(domMod::stock::SELECT_CMI_RESERVED_QUANTITY)
		SQL_BUILDER_CREATE(SelectCMIReservedQuantity)
		SQL_BUILDER_PUSH_BACK(SelectCMIReservedQuantity)

		END_ACCESSOR_DEFINITION

		BUILDER_DEFINITION(SelectPseudoStorageLocationSlot)

		ENSURE_PROPERTIES_ARE_SET(SelectPseudoStorageLocationSlot, BRANCHNO.toPropertyString())

		void SelectPseudoStorageLocationSlot::buildSQLString()
		{
			static const VarString sqlStatement("\n"
				"SELECT pseudolagerort AS " + PSEUDOSTORAGELOCATIONSLOT.getName() +
				"\nFROM pseudolagerorte "
				"\nWHERE filialnr = " + BRANCHNO.toSQLReplacementString()
			);

			resolve(sqlStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerStock);
		}


		BUILDER_DEFINITION(SelectPredictionDate)

		ENSURE_PROPERTIES_ARE_SET(SelectPredictionDate, BRANCHNO.toPropertyString())

		void SelectPredictionDate::buildSQLString()
		{
			static const VarString sqlStatement("\n"
				"SELECT MAX (datum) "
				"\nFROM artprog "
				"\nWHERE filialnr = " + BRANCHNO.toSQLReplacementString() +
				"\n AND artikel_nr = "
				"(SELECT * FROM (SELECT FIRST 1 artikel_nr FROM artikelf WHERE filialnr = " + BRANCHNO.toSQLReplacementString() +
				"AND artikelaktiv = 1) )"
			);

			resolve(sqlStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerStock);
		}


		BUILDER_DEFINITION(SelectStock)
		// Description SelectStock:
		// Gets a list of all articles of all orders with status 'ZU' (cause ER and ST orders are colourless without stockinfo).
		// For each of these articles it acquires the following information:
		//      order number,
		//		article_nr, 
		//		stock (= available qty in stock),
		//		orderedqty, 
		//		discountpct
		//		backorder (=OBM-W, qty that is already ordered from manufacturer but not delivered) 

		ENSURE_PROPERTIES_ARE_SET(SelectStock, BRANCHNO.toPropertyString())

		void SelectStock::buildSQLString()
		{
			// This select gets all orders (except NO) in ZU state  and their orderpos etc. data for all orders (no filters no sort)
			// It fetches the data that form the base for the color calculations.
			// The calculation of colors itself is in transferorderheadcollectiondm.cpp
			static const VarString sqlStatement("\n"
				"SELECT oh.orderno AS " + ORDERNO.getName() +
				", oh.ordertype || oh.orderpickingtype || oh.bookentrytype AS " + ORDERTYPE.getName() +
				", op.articleno AS " + ARTICLENO.getName() +
				", op.posno AS " + POSNO.getName() +
				", NVL(al.bestand, 0) AS " + STOCK.getName() +
				", \nop.orderedqty AS " + ORDEREDQTY.getName() +
				", \nop.discountpct AS " + DISCOUNTPCT.getName() +
				", NVL(af.offene_bestmenge, 0) AS " + BACKORDER.getName() +
				", NVL(af.openorderqty_noteffective, 0) AS " + BACKORDER_NOT_EFFECTIVE.getName() +
				", NVL(af.bestand_min, 0) AS " + MINSTOCK.getName() +
				", NVL(ar.reservedqty, 0) AS " + RESERVEDQTY.getName() +
				", \nCASE WHEN NVL(ap.wprog_s, 0) <> 0 THEN ap.wprog_s ELSE NVL(ap.prog_s, 0) * 5 END AS " + PREDICTION.getName() +
				", NVL(al.lagerfachnr, psls.pseudolagerort ) AS " + STORAGELOCATIONSLOT.getName() +
				" \n FROM orderhead oh\n"
				" INNER JOIN orderpos op ON oh.branchno = op.branchno AND oh.orderno = op.orderno\n"
				" INNER JOIN pseudolagerorte psls ON psls.filialnr = op.branchno\n"
				" LEFT JOIN artikellokal al ON (op.articleno = al.artikel_nr AND op.branchno = al.filialnr AND al.artikelaktiv = 1)\n"
				" LEFT JOIN articlereservation ar ON ar.pharmacygroupid IN\n"
				" (SELECT cpg.pharmacygroupid FROM customerpharmacygr cpg WHERE cpg.customerno = oh.pharmacyno and cpg.branchno = oh.branchno)\n"
				" AND ar.articleno = op.articleno AND ar.branchno = oh.branchno AND ar.reservtype = 0 AND ar.reservedqty > 0\n"
				/*" LEFT JOIN (SELECT cphg.customerno AS customerno, ar.articleno AS articleno, "
								   "ar.reservedqty AS reservedqty, ar.reservepct AS reservepct, "
								   "cphg.branchno AS branchno FROM customerpharmacygr cphg "
							 "INNER JOIN articlereservation ar ON "
							 "(cphg.pharmacygroupid = ar.pharmacygroupid AND cphg.branchno = ar.branchno) "
							 "WHERE ar.reservtype = 0 AND ar.reservedqty > 0) reserv"
				" ON reserv.customerno = oh.pharmacyno AND reserv.articleno = op.articleno\n"
				" AND reserv.branchno = oh.branchno\n"*/
				" LEFT JOIN artikelf af ON (op.articleno = af.artikel_nr AND op.branchno = af.filialnr)\n"
				" LEFT JOIN artprog ap ON (op.branchno = ap.filialnr AND op.articleno = ap.artikel_nr"
				" AND ap.datum = " + PREDICTIONDATE.toSQLReplacementString() +
				" )\n"
				" WHERE oh.ordertype <> '" + lit::NO_ORDERTYPE +
				"' AND oh.status = '0' AND oh.branchno = " + BRANCHNO.toSQLReplacementString() +
				" \nORDER BY oh.orderno ASC \n"
			);

			resolve(sqlStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerStock);
		}

		BUILDER_DEFINITION(SelectCMIReservedQuantity)

		ENSURE_PROPERTIES_ARE_SET(SelectCMIReservedQuantity,
			BRANCHNO.toPropertyString() +
			ARTICLENO.toPropertyString())

		void SelectCMIReservedQuantity::buildSQLString()
		{
			static const VarString sqlStatement(
				"SELECT "
				"\nar.reservedqty AS " + RESERVEDQTY.getName() +
				"\nFROM articlereservation ar "
				"\nWHERE ar.branchno = " + BRANCHNO.toSQLReplacementString() +
				"\nAND ar.reservtype = '16' " +
				"\nAND ar.articleno = " + ARTICLENO.toSQLReplacementString() + "\n; "
			);

			resolve(sqlStatement);
			BLOG_TRACE_SQLSTRING(libabbauw::LoggerPool::loggerStock);
		}

	} // end namespace acc_stock
} // end namespace libabbauw
