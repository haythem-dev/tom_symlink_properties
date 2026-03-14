#include "cmnaccessor.h"

#include "transferorderposcollectionacc.h"
#include "transferorderposcollectionpropertystatechecker.h"

#include "transferorderposcollection_definitions.h"
#include <libbasarcmnutil_logging.h>
#include <libbasarcmnutil_datetime.h>

namespace libabbauw
{
namespace acc_transfer_order_pos
{

	using namespace libabbauw::properties;
	using basar::VarString;

BEGIN_ACCESSOR_DEFINITION( domMod::transferOrder::ACC_TRANSFER_ORDER_POS_COLLECTION )
	
	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO                                  );
		PROPERTY_DESCRIPTION_LIST_ADD( ORDERNO                                   );
		PROPERTY_DESCRIPTION_LIST_ADD( ORDEREDQTY                                );
		PROPERTY_DESCRIPTION_LIST_ADD( PZN                                       );
		PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENO                                 );
        PROPERTY_DESCRIPTION_LIST_ADD( ARTICLEPRICE                              );
		PROPERTY_DESCRIPTION_LIST_ADD( UNIT                                      );
		PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENAME                               );
		PROPERTY_DESCRIPTION_LIST_ADD( PHARMACEUTICALFORM                        );
		PROPERTY_DESCRIPTION_LIST_ADD( POSNO                                     );
		PROPERTY_DESCRIPTION_LIST_ADD( STORAGELOCATIONSLOT                       );
		PROPERTY_DESCRIPTION_LIST_ADD( EXPIRYDATE                                );
        PROPERTY_DESCRIPTION_LIST_ADD( NOTICE                                    );
		PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTPCT                               );
		PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTTYPE                              );
		PROPERTY_DESCRIPTION_LIST_ADD( NONCHARGEDQTY                             );
		PROPERTY_DESCRIPTION_LIST_ADD( WHOLESALERPURCHASEPRICE                   );
		PROPERTY_DESCRIPTION_LIST_ADD( ORDERPROPOSALTIME                         );
		PROPERTY_DESCRIPTION_LIST_ADD( ORDERPROPOSALQTY                          );
		PROPERTY_DESCRIPTION_LIST_ADD( PURCHASEORDERNO                           );
        PROPERTY_DESCRIPTION_LIST_ADD( NEXT_MAIN_ORDER_DATE                      );
        PROPERTY_DESCRIPTION_LIST_ADD( SUCCESSOR                                 );
        PROPERTY_DESCRIPTION_LIST_ADD( DELIVERY_TIME                             );
		PROPERTY_DESCRIPTION_LIST_ADD( SHORTFALLQTY                              );
		PROPERTY_DESCRIPTION_LIST_ADD( SHORTFALLQTYACCUMULATED                   );
		PROPERTY_DESCRIPTION_LIST_ADD( TRANSFER_TIME                             );
        PROPERTY_DESCRIPTION_LIST_ADD( TEXTFIELD                                 );
        PROPERTY_DESCRIPTION_LIST_ADD( CUSTOMERPOSNO                             );
		PROPERTY_DESCRIPTION_LIST_ADD( COLOR                                     );	// color of traffic lights
		PROPERTY_DESCRIPTION_LIST_ADD( LOCALARTICLEBITFIELD                      );	
		PROPERTY_DESCRIPTION_LIST_ADD( CENTRALARTICLEBITFIELD                    );
		PROPERTY_DESCRIPTION_LIST_ADD( LOCALARTICLEBITFIELD2                     );	
		PROPERTY_DESCRIPTION_LIST_ADD( CENTRALARTICLEBITFIELD2                   );	
		PROPERTY_DESCRIPTION_LIST_ADD( ARTICLECLASS                              );	
		PROPERTY_DESCRIPTION_LIST_ADD( STOCK                                     );	
		PROPERTY_DESCRIPTION_LIST_ADD( DELIVERYNOTE                              );
		PROPERTY_DESCRIPTION_LIST_ADD( ENTRYTYPE                                 );
		PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTCALCFROM							 );
		PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTAPPLYTO							 );
		PROPERTY_DESCRIPTION_LIST_ADD( AGP										 );
		PROPERTY_DESCRIPTION_LIST_ADD( GEP										 );
		PROPERTY_DESCRIPTION_LIST_ADD( POSITIONSTATUS                            );

	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( domMod::transferOrder::SELECT_TRANSFER_ORDER_POS_COLLECTION )
		SQL_BUILDER_CREATE( SelectTransferOrderPosCollection )
		SQL_BUILDER_PUSH_BACK( SelectTransferOrderPosCollection )

    ACCESS_METHOD( domMod::transferOrder::SAVE_TRANSFER_ORDER_POS_COLLECTION )
		SQL_BUILDER_CREATE( InsertTransferOrderPosCollection )
		SQL_BUILDER_PUSH_BACK( InsertTransferOrderPosCollection )
        SQL_BUILDER_CREATE( UpdateTransferOrderPosCollection )
        SQL_BUILDER_PUSH_BACK( UpdateTransferOrderPosCollection )

	ACCESS_METHOD( domMod::transferOrder::DELETE_POSITION_FROM_POS_COLLECTION )
		SQL_BUILDER_CREATE( DeleteTransferOrderPos )
		SQL_BUILDER_PUSH_BACK( DeleteTransferOrderPos )

END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectTransferOrderPosCollection )

    bool SelectTransferOrderPosCollection::isExecutable() const
    {
        accessor::transferOrder::PosCollectionPropertyStateChecker checker( getPropertyList() );
        return checker.isSelectable();
    }

	void SelectTransferOrderPosCollection::buildSQLString()
	{
		static const VarString sqlStatement( "\n"
            "SELECT op.branchno AS "                                                              + BRANCHNO.getName()                +
            ", op.orderno AS "                                                                    + ORDERNO.getName()                 +
            ", (op.orderedqty - op.nonchargedqty)::INT AS "                                       + ORDEREDQTY.getName()              +
			", NVL(ac.article_code, 0) AS "                                                       + PZN.getName()                     +
			", op.articleno AS "                                                                  + ARTICLENO.getName()               +
			", az.einheit AS "                                                                    + UNIT.getName()                    +
			",\nCASE WHEN NVL(ac.article_code, 0) > 0 THEN az.artikel_name"
			" \n     ELSE op.textfield END AS "                                                   + ARTICLENAME.getName()             +
			", az.darreichform AS "                                                               + PHARMACEUTICALFORM.getName()      +
			", op.posno AS "                                                                      + POSNO.getName()                   +
			", NVL(al.lagerfachnr, (SELECT pseudolagerort FROM pseudolagerorte WHERE filialnr = " + BRANCHNO.toSQLReplacementString() +
            ")) AS "                                                                              + STORAGELOCATIONSLOT.getName()     +
            ", \nCASE WHEN ao.datumverfall = 0 THEN '' "
			"  \n     ELSE (SUBSTRING(CAST(ao.datumverfall AS char(8)) FROM 3 FOR 2) || '/'"
			" || SUBSTRING(CAST(ao.datumverfall AS char(8)) FROM 1 FOR 2))"
			"  \n     END AS "                                                                    + EXPIRYDATE.getName()              +

			", \n al.bestand AS "																  + STOCK.getName()					  +
			", \n al.etartschalter1 AS "														  + LOCALARTICLEBITFIELD.getName()	  +
			", \n az.etartschalter1 AS "														  + CENTRALARTICLEBITFIELD.getName()  +

			", \n al.etartschalter2 AS "														  + LOCALARTICLEBITFIELD2.getName()	  +
			", \n az.etartschalter2 AS "														  + CENTRALARTICLEBITFIELD2.getName() +

			", \n az.etartklasse1 AS "														      + ARTICLECLASS.getName()			  +
			//", \n al.wafotyp AS "																  + WAFOTYPE.getName()				  +	
			", \nop.discountpct AS "                                                              + DISCOUNTPCT.getName()             +
			//TODO DZ", \nop.discounttype AS "                                                             + DISCOUNTTYPE.getName()            +
			", 'BR' AS "																		  + DISCOUNTTYPE.getName()            +
			", op.nonchargedqty AS "                                                              + NONCHARGEDQTY.getName()           +
            ", op.wholesalerpurchaseprice AS "                                                    + ARTICLEPRICE.getName()            +
			", op.wholesalerpurchaseprice * op.orderedqty AS "                                    + WHOLESALERPURCHASEPRICE.getName() +
            ", op.textfield AS "                                                                  + TEXTFIELD.getName()               +
            ", op.customerposno AS "                                                              + CUSTOMERPOSNO.getName()           +
			", op.entrytype AS "																  + ENTRYTYPE.getName()				  +
			", oprop.maxorderproposaltime AS "                                                    + ORDERPROPOSALTIME.getName()       +
			", oprop.orderproposalqty AS "                                                        + ORDERPROPOSALQTY.getName()        +
			", oprop.purchaseorderno AS "                                                         + PURCHASEORDERNO.getName()         +
			", oprop.transfertime AS "                                                            + TRANSFER_TIME.getName()           +
			", op.discountapplyto AS "                                                            + DISCOUNTAPPLYTO.getName()         +
			", op.discountcalcfrom AS "                                                           + DISCOUNTCALCFROM.getName()        +
			", '' AS "																			  + POSITIONSTATUS.getName()         +
			", ap.preisekgrosso AS "															  + GEP.getName()					  +
			 ", ap.preisagp AS "																  + AGP.getName()					  +
            ", TO_DATE( CASE WHEN fl.datum_next_bestell = 0 THEN " NULLVAL_SQL_DATE_STR 
                   " ELSE TO_CHAR(fl.datum_next_bestell) END," FORMAT_SQL_DATE_IFX")::DATE AS "   + NEXT_MAIN_ORDER_DATE.getName()    +
            ",\nNVL2((SELECT artikel_nr FROM artikelaltern "
             " WHERE filialnr = op.branchno AND artikel_nr = op.articleno), 1, 0 )::SMALLINT AS " + SUCCESSOR.getName() +
            ", fl.lz_haupt AS "                                                                   + DELIVERY_TIME.getName()           +
			" \nFROM orderhead oh"
			" \nJOIN orderpos op ON oh.branchno = op.branchno AND oh.orderno = op.orderno"
			" \nLEFT JOIN artikelzentral az ON op.articleno = az.artikel_nr"
			" \nLEFT JOIN artikellokal al ON (op.articleno = al.artikel_nr AND op.branchno = al.filialnr)"
			" \nLEFT JOIN artikellagerort ao ON (op.branchno = ao.filialnr AND op.articleno = ao.artikel_nr)"
			" \nLEFT JOIN articlecodes ac ON (op.articleno = ac.articleno AND ac.preferred_flag = '1')"
            " \nLEFT JOIN artikelf af ON op.branchno = af.filialnr AND op.articleno = af.artikel_nr"
            " \nLEFT JOIN artikelpreis ap on az.artikel_nr = ap.artikel_nr"
            " \nLEFT JOIN fliefer fl ON fl.filialnr = "                                           + BRANCHNO.toSQLReplacementString() +
            " AND af.filialnr = fl.filialnr AND af.besla_nr = fl.besla_nr"
			" \nLEFT JOIN"
			" (SELECT MAX(orderproposaltime) as maxorderproposaltime, branchno,"
			        " ordernobatch, posnobatch, orderproposalqty, purchaseorderno, transfertime"
			 " FROM orderproposal"
			 " GROUP BY branchno, ordernobatch, posnobatch, orderproposalqty, purchaseorderno, transfertime"
			 ") AS oprop ON (op.branchno = oprop.branchno AND op.orderno = oprop.ordernobatch AND op.posno = oprop.posnobatch)"
			" \nWHERE op.branchno = "                                                             + BRANCHNO.toSQLReplacementString() +
			" AND oh.orderno = "                                                                  + ORDERNO.toSQLReplacementString()  +
			" \nORDER BY op.posno ASC"
		);
        
		resolve( sqlStatement );
		BLOG_TRACE_SQLSTRING( LoggerPool::loggerTransferOrder );
	}
	
BUILDER_DEFINITION( DeleteTransferOrderPos)

    bool DeleteTransferOrderPos::isExecutable() const
    {
        accessor::transferOrder::PosCollectionPropertyStateChecker checker( getPropertyList() );
        return checker.isDeletable();
    }

	void DeleteTransferOrderPos::buildSQLString()
	{
		static const VarString sqlStatement( 
			"DELETE FROM orderpos WHERE " + BRANCHNO.getName()   +  " = "   +   BRANCHNO.toSQLReplacementString() +
			                      " AND " + ORDERNO.getName()    +  " = "   +   ORDERNO.toSQLReplacementString()  +
								  " AND posno "                  +  " = "   +   POSNO.toSQLReplacementString()
			);

		resolve( sqlStatement );
		BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
	}

BUILDER_DEFINITION( InsertTransferOrderPosCollection )

    bool InsertTransferOrderPosCollection::isExecutable() const
    {
        accessor::transferOrder::PosCollectionPropertyStateChecker checker( getPropertyList() );
        return checker.isInsertable();
    }

    void InsertTransferOrderPosCollection::buildSQLString()
    {
        const VarString sqlStatement(
            "INSERT INTO orderpos"
            "(branchno, orderno, posno, articleno, article_code, orderedqty, nonchargedqty, discountpct, wholesalerpurchaseprice, textfield, customerposno, entrytype, discountapplyto, discountcalcfrom)"
            //TODO DZ"(branchno, orderno, posno, articleno, article_code, orderedqty, nonchargedqty, discountpct, discounttype, wholesalerpurchaseprice, textfield, customerposno)"
			" VALUES (" + 
							BRANCHNO.toSQLReplacementString()                                                    + ", " +
							ORDERNO.toSQLReplacementString()                                                     + ", " +
							POSNO.toSQLReplacementString()                                                       + ", " +
							ARTICLENO.toSQLReplacementString()                                                   + ", " +
							PZN.toSQLReplacementString()                                                         + ", " +
							ORDEREDQTY.toSQLReplacementString()													 + " + " + 
							NONCHARGEDQTY.toSQLReplacementString()												 + ", " +
							NONCHARGEDQTY.toSQLReplacementString()                                               + ", " +
							DISCOUNTPCT.toSQLReplacementString("'", "'")                                         + ", " +
							//TODO DZ DISCOUNTTYPE.toSQLReplacementString()												 + ", " +
							ARTICLEPRICE.toSQLReplacementString("'", "'")                                        + ", " +
							TEXTFIELD.toSQLReplacementString("'", "'" )                                          + ", " + 
							CUSTOMERPOSNO.toSQLReplacementString()                                               + ", " +
							ENTRYTYPE.toSQLReplacementString("'", "'")											 + ", " +
							DISCOUNTAPPLYTO.toSQLReplacementString()									         + ", " +
							DISCOUNTCALCFROM.toSQLReplacementString()									         + ") " 
                     );

        resolve( sqlStatement );
		BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
    }

BUILDER_DEFINITION( UpdateTransferOrderPosCollection )

    bool UpdateTransferOrderPosCollection::isExecutable() const
    {
        accessor::transferOrder::PosCollectionPropertyStateChecker checker( getPropertyList() );
        return checker.isUpdatable();
    }

    void UpdateTransferOrderPosCollection::buildSQLString()
    {
        static const VarString sqlStatement(
            "UPDATE orderpos SET "
            " orderedqty = "                    + ORDEREDQTY.toSQLReplacementString()          + " + " + NONCHARGEDQTY.toSQLReplacementString() +
            ", nonchargedqty = "                + NONCHARGEDQTY.toSQLReplacementString()       +
            ", discountpct = "                  + DISCOUNTPCT.toSQLReplacementString("'", "'") +
			//TODO DZ", discounttype = "                 + DISCOUNTTYPE.toSQLReplacementString()		   +
			", discountcalcfrom = "             + DISCOUNTCALCFROM.toSQLReplacementString()    +
			", discountapplyto = "              + DISCOUNTAPPLYTO.toSQLReplacementString()     +
            ", textfield = "                    + TEXTFIELD.toSQLReplacementString("'", "'")   +
			", entrytype = "					+ ENTRYTYPE.toSQLReplacementString("'", "'")   +
            " WHERE branchno = "                + BRANCHNO.toSQLReplacementString()            +
            " AND orderno = "                   + ORDERNO.toSQLReplacementString()             +
            " AND posno = "                     + POSNO.toSQLReplacementString()
	    );
        resolve( sqlStatement );
		BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
    }

} // end namespace acc_transfer_order_pos
} // end namespace libabbauw
