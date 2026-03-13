//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmnaccessor.h"
#include "orderproposalcollectionacc.h"
#include "orderproposalcollectionacc_definitions.h"

namespace libabbauw
{
namespace infrastructure
{
namespace accessor
{
namespace orderProposal
{
	using namespace libabbauw::properties;

    BEGIN_ACCESSOR_DEFINITION( lit::ACC_ORDERPROPOSALCOLLECTION )
        BEGIN_PROPERTY_DESCRIPTION_LIST
            PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNO            ); // for collective order wish view
            PROPERTY_DESCRIPTION_LIST_ADD( PURCHASEORDERSUPPLIERNO   );
			PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENAME               );
			PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENO                 );
            PROPERTY_DESCRIPTION_LIST_ADD( PZN                       );
			PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNO                );
			PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO                  );
			PROPERTY_DESCRIPTION_LIST_ADD( DATE_OF_PURCHASE          );
			PROPERTY_DESCRIPTION_LIST_ADD( ORDEREDQTY                ); // for collective order wish view
			PROPERTY_DESCRIPTION_LIST_ADD( ORDERPROPOSALQTY          );
			PROPERTY_DESCRIPTION_LIST_ADD( ORDERNO                   );
			PROPERTY_DESCRIPTION_LIST_ADD( ORDERTYPE                 );
			PROPERTY_DESCRIPTION_LIST_ADD( ORDERDATE                 );
			PROPERTY_DESCRIPTION_LIST_ADD( EDIDELIVERYDATE           );
			PROPERTY_DESCRIPTION_LIST_ADD( ORDERPROPOSALTIME         );
			PROPERTY_DESCRIPTION_LIST_ADD( ORDER_PROP_PROCESSED_TIME );
			PROPERTY_DESCRIPTION_LIST_ADD( ORDER_PROP_DISCLAIM_NOTE  );
			PROPERTY_DESCRIPTION_LIST_ADD( POSNO                     );
			PROPERTY_DESCRIPTION_LIST_ADD( PURCHASEORDERNO           );
			PROPERTY_DESCRIPTION_LIST_ADD( REASON_ID                 );
			PROPERTY_DESCRIPTION_LIST_ADD( TRANSFER_TIME             );
			PROPERTY_DESCRIPTION_LIST_ADD( UNIT                      );
            PROPERTY_DESCRIPTION_LIST_ADD( EXPECTED_GI_DATE          );
            PROPERTY_DESCRIPTION_LIST_ADD( NEXT_MAIN_ORDER_DATE      );
            PROPERTY_DESCRIPTION_LIST_ADD( SUCCESSOR                 );
            PROPERTY_DESCRIPTION_LIST_ADD( DELIVERY_TIME             );
			PROPERTY_DESCRIPTION_LIST_ADD( DELIVERYNOTE              );
			PROPERTY_DESCRIPTION_LIST_ADD( LOCALARTICLEBITFIELD      );
			PROPERTY_DESCRIPTION_LIST_ADD( LOCALARTICLEBITFIELD2     );
			PROPERTY_DESCRIPTION_LIST_ADD( CENTRALARTICLEBITFIELD    );
			PROPERTY_DESCRIPTION_LIST_ADD( CENTRALARTICLEBITFIELD2   );
			PROPERTY_DESCRIPTION_LIST_ADD( STOCK					 );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( lit::SELECT_ORDER_PROPOSALS_BY_KEY );
            SQL_BUILDER_CREATE( SelectOrderProposalsByKey )
			SQL_BUILDER_PUSH_BACK( SelectOrderProposalsByKey )

    END_ACCESSOR_DEFINITION


	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( SelectOrderProposalsByKey )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( SelectOrderProposalsByKey )

    ENSURE_PROPERTIES_ARE_SET(
		SelectOrderProposalsByKey, BRANCHNO.toPropertyString()
	);

    void SelectOrderProposalsByKey::buildSQLString()
    {
        using namespace libabbauw::properties;
		
		METHODNAME_DEF( SelectOrderProposalsByKey, buildSQLString ) 
		BLOG_TRACE_METHOD( LoggerPool::loggerOrderProposal, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////
		//Get all important values but skip the rows which are already transfered or do have a quantity of 0
        static const basar::VarString sql( "\n"
            "SELECT af.besla_nr AS "                                                                     + PURCHASEORDERSUPPLIERNO.getName()
                + ", az.hersteller_nr AS "                                                               + MANUFACTURERNO.getName()
                + ", az.artikel_name AS "	                                                             + ARTICLENAME.getName()
				+ ", op.articleno AS "                                                                   + ARTICLENO.getName()
                + ", acpzn.article_code AS "                                                             + PZN.getName()
				+ ", oh.pharmacyno AS "																	 + PHARMACYNO.getName()
				+ ", op.orderproposalqty AS "                                                            + ORDERPROPOSALQTY.getName()
				+ ",\n op.ordernobatch AS "                                                              + ORDERNO.getName()
				+ ", oh.ordertype || oh.orderpickingtype || oh.bookentrytype AS "                        + ORDERTYPE.getName() 
				+ ", TO_DATE( CASE WHEN oh.orderdate = 0 THEN " NULLVAL_SQL_DATE_STR
					"ELSE TO_CHAR(oh.orderdate) END, " FORMAT_SQL_DATE_IFX ")::DATE AS "			 + ORDERDATE.getName()	
				+ ", TO_DATE( CASE WHEN oh.edideliverydate = 0 THEN " NULLVAL_SQL_DATE_STR 
					"ELSE TO_CHAR(oh.edideliverydate) END, " FORMAT_SQL_DATE_IFX ")::DATE AS "		 + EDIDELIVERYDATE.getName()
				+ ", op.posnobatch AS "                                                                  + POSNO.getName()
				+ ", op.reasonid AS "                                                                    + REASON_ID.getName()
				+ ", op.orderproposaltime AS "                                                           + ORDERPROPOSALTIME.getName()
				+ ", op.transfertime AS "                                                                + TRANSFER_TIME.getName()
				+ ",\n op.orderproposalprocessedtime AS "                                                + ORDER_PROP_PROCESSED_TIME.getName()
				+ ", op.orderproposaldisclaimnote AS "                                                   + ORDER_PROP_DISCLAIM_NOTE.getName()
				+ ",\n op.purchaseorderno AS "                                                           + PURCHASEORDERNO.getName()
				+ ", op.dateofpurchase AS "                                                              + DATE_OF_PURCHASE.getName()
                + ", TO_DATE( CASE WHEN op.expectedgoodsindate = 0 THEN " NULLVAL_SQL_DATE_STR                         
                         " ELSE TO_CHAR(op.expectedgoodsindate) END," FORMAT_SQL_DATE_IFX")::DATE AS "   + EXPECTED_GI_DATE.getName()
				+ ",\n az.einheit AS "                                                                   + UNIT.getName()
                + ", TO_DATE( CASE WHEN fl.datum_next_bestell = 0 THEN " NULLVAL_SQL_DATE_STR 
                         " ELSE TO_CHAR(fl.datum_next_bestell) END," FORMAT_SQL_DATE_IFX")::DATE AS "    + NEXT_MAIN_ORDER_DATE.getName()
                + ", NVL2((SELECT artikel_nr FROM artikelaltern "
                      "WHERE filialnr = op.branchno AND artikel_nr = op.articleno), 1, 0 )::SMALLINT AS" + SUCCESSOR.getName()
                + ", fl.lz_haupt AS "                                                                    + DELIVERY_TIME.getName()

				+ ", \n al.bestand AS "																  + STOCK.getName()					  
				+  ", \n al.etartschalter1 AS "														  + LOCALARTICLEBITFIELD.getName()	  
				+  ", \n az.etartschalter1 AS "														  + CENTRALARTICLEBITFIELD.getName()  
				+ 
				+  ", \n al.etartschalter2 AS "														  + LOCALARTICLEBITFIELD2.getName()	  
				+  ", \n az.etartschalter2 AS "														  + CENTRALARTICLEBITFIELD2.getName()

			+ "\n FROM orderproposal op \n"
			  " LEFT JOIN orderhead as oh ON op.ordernobatch = oh.orderno \n"
			  " LEFT JOIN artikellokal al ON (op.articleno = al.artikel_nr AND op.branchno = al.filialnr) \n"
			  " LEFT JOIN artikelzentral az ON az.artikel_nr = op.articleno \n"
              " JOIN articlecodes acpzn ON op.articleno = acpzn.articleno AND acpzn.preferred_flag = 1"
              " \nLEFT JOIN artikelf af ON op.branchno = af.filialnr AND op.articleno = af.artikel_nr"
                                     " AND af.filialnr = "                                               + BRANCHNO.toSQLReplacementString()
            + " \nLEFT JOIN fliefer fl ON af.filialnr = fl.filialnr AND af.besla_nr = fl.besla_nr"
                                     " AND fl.filialnr = "                                               + BRANCHNO.toSQLReplacementString()
			+ " \nWHERE ordertypebatch = 1 AND "
              "op.orderproposalqty > 0 AND op.branchno = "	                                             + BRANCHNO.toSQLReplacementString()
		);

        basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
        basar::VarString whereClause;
        if( yit.isContainedAndSet( properties::ORDERNO ) )
        {
            whereClause.append( " AND op.ordernobatch = "            + ORDERNO.toSQLReplacementString() );
        }

    	basar::VarString orderStatement;
		if( isContainedAndSet( SORT ) )
		{
			orderStatement = "\nORDER BY " + getPropertyList().getString( SORT.getName() );
		}

    	resolve( sql + whereClause + orderStatement );
        BLOG_TRACE_SQLSTRING( LoggerPool::loggerOrderProposal );
    }

} // end namespace orderProposal
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw
