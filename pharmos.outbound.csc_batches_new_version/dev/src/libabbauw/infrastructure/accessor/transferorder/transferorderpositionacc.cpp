//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/accessor.h>
#include <libutil/util.h>

#include "transferorderpositionacc.h"
#include "transferorderpositionacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

//---------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace infrastructure
{
namespace accessor
{
namespace transferOrder
{

    //-------------------------------------------------------------------------------------------------//
    // Accessor Definition
    //-------------------------------------------------------------------------------------------------//
    BEGIN_ACCESSOR_DEFINITION( lit::TRANSFER_ORDER_POSITION_ACC )

        BEGIN_PROPERTY_DESCRIPTION_LIST
            PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCHNO                         );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO                          );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::POSNO                            );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLENO                        );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDEREDQTY                       );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::UNIT                             );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLENAME                      );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACEUTICALFORM               );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNTPCT                      );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::NONCHARGEDQTY                    );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::WHOLESALERPURCHASEPRICE          );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ENTRYTYPE						);
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERPROPOSALTIME                );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERPROPOSALQTY                 );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASEORDERNO                  );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNTCALCFROM                 );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNTAPPLYTO                  );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::GEP                              );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::AGP                              );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::POSITIONSTATUS                   );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( lit::SELECT_TRANSFER_ORDER_POSITION_BY_KEY )
            SQL_BUILDER_CREATE   ( SelectTransferOrderPositionByKey )
            SQL_BUILDER_PUSH_BACK( SelectTransferOrderPositionByKey )

    END_ACCESSOR_DEFINITION

    //-------------------------------------------------------------------------------------------------//
    // Builder Definition 'SelectTransferOrderPositionByKey'
    //-------------------------------------------------------------------------------------------------//
    BUILDER_DEFINITION( SelectTransferOrderPositionByKey )

    bool SelectTransferOrderPositionByKey::isExecutable() const
    {
        METHODNAME_DEF( SelectTransferOrderPositionByKey, isExecutable );
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;

        bool executable = isContainedAndSet( properties::BRANCHNO.getName() ) 
                       && isContainedAndSet( properties::ORDERNO ) 
                       && isContainedAndSet( properties::POSNO );

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

        return executable;
    }

    void SelectTransferOrderPositionByKey::buildSQLString()
    {
        METHODNAME_DEF( SelectTransferOrderPositionByKey, buildSQLString );
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;
        BLOG_TRACE_METHOD( logger, fun );

        static const basar::I18nString stmt( 
            "SELECT op.branchno AS "           + properties::BRANCHNO.getName()                +
            ", op.orderno AS "                 + properties::ORDERNO.getName()                 +
            ", op.posno AS "                   + properties::POSNO.getName()                   +
            ", op.articleno AS "               + properties::ARTICLENO.getName()               +
            ", op.orderedqty AS "              + properties::ORDEREDQTY.getName()              +
		    ", op.discountpct AS "             + properties::DISCOUNTPCT.getName()             +
		    ", op.nonchargedqty AS "           + properties::NONCHARGEDQTY.getName()           +
            ", op.wholesalerpurchaseprice AS " + properties::WHOLESALERPURCHASEPRICE.getName() +
			", op.entrytype AS "			   + properties::ENTRYTYPE.getName()			   +
            ", az.einheit AS "                 + properties::UNIT.getName()                    + 
		    ", CASE WHEN NVL(az.artikel_nr, 0) > 0 THEN az.artikel_name"
			" ELSE op.textfield END AS "       + properties::ARTICLENAME.getName()             +
            ", az.darreichform AS "            + properties::PHARMACEUTICALFORM.getName()      +
		    ", oprop.maxorderproposaltime AS " + properties::ORDERPROPOSALTIME.getName()       +
		    ", oprop.orderproposalqty AS "     + properties::ORDERPROPOSALQTY.getName()        +
		    ", oprop.purchaseorderno AS "      + properties::PURCHASEORDERNO.getName()         +
            ", op.discountcalcfrom AS "        + properties::DISCOUNTCALCFROM.getName()        +
            ", op.discountapplyto AS "         + properties::DISCOUNTAPPLYTO.getName()         +
            ", NVL(ap.preisekgrosso,0.0) AS "  + properties::GEP.getName()                     +
            ", NVL(ap.preisagp, 0.0) AS "      + properties::AGP.getName()                     +
            ", '' AS "                         + properties::POSITIONSTATUS.getName()          +
            " FROM orderpos op"
            " LEFT JOIN artikelzentral az ON op.articleno = az.artikel_nr"
            " LEFT OUTER JOIN artikelpreis ap ON ap.artikel_nr = op.articleno "
            " LEFT JOIN"
			" (SELECT MAX(orderproposaltime) as maxorderproposaltime, branchno,"
			        " ordernobatch, posnobatch, orderproposalqty, purchaseorderno, transfertime"
			 " FROM orderproposal"
			 " GROUP BY branchno, ordernobatch, posnobatch, orderproposalqty, purchaseorderno, transfertime"
			 ") AS oprop ON (op.branchno = oprop.branchno AND op.orderno = oprop.ordernobatch AND op.posno = oprop.posnobatch)"
            " WHERE branchno = "               + properties::BRANCHNO.toSQLReplacementString() +
            " AND orderno = "                  + properties::ORDERNO.toSQLReplacementString()  +
            " AND posno = "                    + properties::POSNO.toSQLReplacementString()
        );
        resolve( stmt );

        BLOG_TRACE_SQLSTRING( logger );
    }

} // end namespace transferOrder
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw
