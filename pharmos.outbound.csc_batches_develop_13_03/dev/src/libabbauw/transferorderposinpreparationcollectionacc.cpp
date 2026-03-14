#include "cmnaccessor.h"

#include "transferorderposinpreparationcollectionacc.h"
#include "transferorderposinpreparationcollectionpropertystatechecker.h"

#include "transferorderposinpreparationcollection_definitions.h"
#include <libbasarcmnutil_logging.h>

namespace libabbauw
{
namespace acc_transfer_order_in_preparation
{
	using namespace libabbauw::properties;
	using basar::VarString;

BEGIN_ACCESSOR_DEFINITION( domMod::transferOrder::ACC_TRANSFER_ORDER_POS_IN_PREPARATION_COLLECTION )
	
	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO           );
		PROPERTY_DESCRIPTION_LIST_ADD( USER               );
        PROPERTY_DESCRIPTION_LIST_ADD( POSNO              );
        PROPERTY_DESCRIPTION_LIST_ADD( POSNO_ORIG         );
        PROPERTY_DESCRIPTION_LIST_ADD( PZN                );
        PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENO          );
        PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENAME        );
        PROPERTY_DESCRIPTION_LIST_ADD( UNIT               );
        PROPERTY_DESCRIPTION_LIST_ADD( PHARMACEUTICALFORM );
        PROPERTY_DESCRIPTION_LIST_ADD( ORDEREDQTY         );
        PROPERTY_DESCRIPTION_LIST_ADD( NONCHARGEDQTY      );
		PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTPCT        );
		PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTTYPE       );
        PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTCALCFROM   );
        PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTAPPLYTO    );
        PROPERTY_DESCRIPTION_LIST_ADD( GEP                );
        PROPERTY_DESCRIPTION_LIST_ADD( AGP                );
        PROPERTY_DESCRIPTION_LIST_ADD( POSITIONSTATUS     );
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( domMod::transferOrder::SELECT_CURRENT_TRANSFER_ORDER_POS_IN_PREPARATION_COLLECTION )
		SQL_BUILDER_CREATE(    SelectCurrentTransferOrderPosInPreparationCollection )
		SQL_BUILDER_PUSH_BACK( SelectCurrentTransferOrderPosInPreparationCollection )

    ACCESS_METHOD( domMod::transferOrder::SAVE_TRANSFER_ORDER_POS_IN_PREPARATION_COLLECTION )
		SQL_BUILDER_CREATE(    InsertTransferOrderPosInPreparationCollection )
		SQL_BUILDER_PUSH_BACK( InsertTransferOrderPosInPreparationCollection )
        SQL_BUILDER_CREATE(    UpdateTransferOrderPosInPreparationCollection )
        SQL_BUILDER_PUSH_BACK( UpdateTransferOrderPosInPreparationCollection )
		SQL_BUILDER_CREATE(    DeleteTransferOrderPosInPreparationCollection )
		SQL_BUILDER_PUSH_BACK( DeleteTransferOrderPosInPreparationCollection )

END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectCurrentTransferOrderPosInPreparationCollection )

    bool SelectCurrentTransferOrderPosInPreparationCollection::isExecutable() const
    {
        accessor::transferOrder::PosInPreparationCollectionPropertyStateChecker checker( getPropertyList() );
        return checker.isSelectable();
    }

	void SelectCurrentTransferOrderPosInPreparationCollection::buildSQLString()
	{
		static const VarString sqlStatement(
            "SELECT "
            "op.branchno AS "                                                                + BRANCHNO.getName()                +
            ", op.user AS "                                                                  + USER.getName()                    +
            ", op.posno AS "                                                                 + POSNO.getName()                   +
            ", op.posno AS "                                                                 + POSNO_ORIG.getName()              +
            ", op.articleno AS "                                                             + ARTICLENO.getName()               +
            ", az.artikel_name AS "                                                          + ARTICLENAME.getName()             +
            ", az.darreichform AS "                                                          + PHARMACEUTICALFORM.getName()      +
            ", TRIM(az.einheit) AS "                                                         + UNIT.getName()                    +
            ", acpzn.article_code AS "                                                       + PZN.getName()                     +
            ", op.orderedqty AS "                                                            + ORDEREDQTY.getName()              +
            ", op.nonchargedqty AS "                                                         + NONCHARGEDQTY.getName()           +
            ", op.discountpct AS "                                                           + DISCOUNTPCT.getName()             +
            //TODO DZ ", op.discounttype AS "                                                          + DISCOUNTTYPE.getName()            +
			", 'BR' AS "																	+ DISCOUNTTYPE.getName()            +
            ", op.discountcalcfrom AS "                                                      + DISCOUNTCALCFROM.getName()        +
            ", op.discountapplyto AS "                                                       + DISCOUNTAPPLYTO.getName()         +
            ", NVL(ap.preisekgrosso,0.0) AS "                                                + GEP.getName()                     +
            ", NVL(ap.preisagp, 0.0) AS "                                                    + AGP.getName()                     +
            ", '' AS "                                                                       + POSITIONSTATUS.getName()          +
			" FROM orderposinprep op"
            " JOIN articlecodes acpzn ON op.articleno = acpzn.articleno AND acpzn.preferred_flag=1"
            " LEFT OUTER JOIN artikelpreis ap ON ap.artikel_nr = op.articleno "
            " JOIN artikelzentral az ON az.artikel_nr = op.articleno "
            " WHERE branchno = "                                                             + BRANCHNO.toSQLReplacementString() +
            " AND op.user = '"                                                               + USER.toSQLReplacementString()     + "'"
		);

		resolve( sqlStatement );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
	}


BUILDER_DEFINITION( InsertTransferOrderPosInPreparationCollection )

    bool InsertTransferOrderPosInPreparationCollection::isExecutable() const
    {
        accessor::transferOrder::PosInPreparationCollectionPropertyStateChecker checker( getPropertyList() );
        return checker.isInsertable();
    }

	void InsertTransferOrderPosInPreparationCollection::buildSQLString()
	{
        static const VarString sqlStatement(
            "INSERT INTO orderposinprep"
            //"(branchno, user, posno, articleno, orderedqty, nonchargedqty, discountpct) \n"
            "(branchno, user, posno, articleno, orderedqty, nonchargedqty, discountpct, discountcalcfrom, discountapplyto) \n"
            //TODO DZ "(branchno, user, posno, articleno, orderedqty, nonchargedqty, discountpct, discounttype) \n"
			" VALUES( " + 
                        BRANCHNO.toSQLReplacementString()                                        +
                        USER.toSQLReplacementString( ", '", "'")                                 +
                        POSNO.toSQLReplacementString( ", ", "" )                                 +   
                        ", (SELECT articleno FROM articlecodes WHERE preferred_flag=1 AND article_code = " + PZN.toSQLReplacementString( "'", "')") +
                        ORDEREDQTY.toSQLReplacementString( ", ", "" )                            +   
                        NONCHARGEDQTY.toSQLReplacementString( ",  ", "")                         +
                        DISCOUNTPCT.toSQLReplacementString( ", ", "" )                           +
						//TODO DZ DISCOUNTTYPE.toSQLReplacementString()								     +
                        DISCOUNTCALCFROM.toSQLReplacementString(", ", "")                                +
                        DISCOUNTAPPLYTO.toSQLReplacementString(", ", "")                                 +

            ")"
	    );

		resolve( sqlStatement );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
	}

BUILDER_DEFINITION( UpdateTransferOrderPosInPreparationCollection )

    bool UpdateTransferOrderPosInPreparationCollection::isExecutable() const
    {
        accessor::transferOrder::PosInPreparationCollectionPropertyStateChecker checker( getPropertyList() );
        return checker.isUpdatable();
    }

	void UpdateTransferOrderPosInPreparationCollection::buildSQLString()
	{
        static const VarString sqlStatement(
            "UPDATE orderposinprep op SET "
			"posno = "																			+ POSNO.toSQLReplacementString()			+
			", articleno = "
			"(SELECT articleno FROM articlecodes WHERE preferred_flag=1 AND article_code = "	+ PZN.toSQLReplacementString("'", "')" )	+
			", orderedqty = "																	+ ORDEREDQTY.toSQLReplacementString()		+
			", nonchargedqty = "																+ NONCHARGEDQTY.toSQLReplacementString()	+
			", discountpct = "																	+ DISCOUNTPCT.toSQLReplacementString()		+
			//TODO DZ ", discounttype = "																	+ DISCOUNTTYPE.toSQLReplacementString()		+
            ", discountcalcfrom = "                                                               + DISCOUNTCALCFROM.toSQLReplacementString()		+
            ", discountapplyto = "                                                               + DISCOUNTAPPLYTO.toSQLReplacementString()		+
            " WHERE branchno = "																+ BRANCHNO.toSQLReplacementString()			+
            " AND op.user = "																	+ USER.toSQLReplacementString( "'", "'" )	+
            " AND posno = "																		+ POSNO_ORIG.toSQLReplacementString()
		);

		resolve( sqlStatement );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
	}

BUILDER_DEFINITION( DeleteTransferOrderPosInPreparationCollection )

    bool DeleteTransferOrderPosInPreparationCollection::isExecutable() const
    {
        accessor::transferOrder::PosInPreparationCollectionPropertyStateChecker checker( getPropertyList() );

        bool deleteable =  checker.isDeletable();
        return deleteable;
    }

	void DeleteTransferOrderPosInPreparationCollection::buildSQLString()
	{
		static const VarString sqlStatement(
			"DELETE FROM orderposinprep op"
            " WHERE op.branchno = "                                                             + BRANCHNO.toSQLReplacementString()      +
            " AND op.user = "                                                                   + USER.toSQLReplacementString("'", "'")  +
            " AND op.posno = "                                                                  + POSNO_ORIG.toSQLReplacementString()         
		);

		resolve( sqlStatement );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
	}

} // end namespace acc_transfer_order_in_preparation
} // end namespace libabbauw
