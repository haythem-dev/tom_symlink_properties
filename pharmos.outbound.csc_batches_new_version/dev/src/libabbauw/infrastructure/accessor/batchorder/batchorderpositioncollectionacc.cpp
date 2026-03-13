//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/accessor.h>
#include <libutil/util.h>

#include "infrastructure/accessor/batchorder/batchorderpositioncollectionacc.h"
#include "infrastructure/accessor/batchorder/batchorderpositioncollectionacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

#include "orderstatus.h"
#include "accessor_datetypes_common.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace infrastructure
{
namespace accessor
{
namespace batchOrder
{
    //---------------------------------------------------------------------------------------------//
    // accessor definition
    //---------------------------------------------------------------------------------------------//
        BEGIN_ACCESSOR_DEFINITION( lit::ACC_BATCH_ORDER_POSITION_COLLECTION )
            BEGIN_PROPERTY_DESCRIPTION_LIST
                PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCHNO             )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO              )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::POSNO                )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::PZN                  )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDEREDQTY           )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::UNIT                 )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLENAME          )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLEPRICE         )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACEUTICALFORM   )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::EXPIRYDATE           )

            ACCESS_METHOD( lit::SELECT_BATCH_ORDER_POSITION_COLLECTION_BY_KEY  )
                SQL_BUILDER_CREATE   ( SelectBatchOrderPositionCollectionByKey )
                SQL_BUILDER_PUSH_BACK( SelectBatchOrderPositionCollectionByKey )

        END_ACCESSOR_DEFINITION

        //////////////////////////////////////////////////////////////////////////////////////////
        // BUILDER_DEFINITION( SelectBatchOrderPositionCollectionByKey )
        //////////////////////////////////////////////////////////////////////////////////////////
        BUILDER_DEFINITION( SelectBatchOrderPositionCollectionByKey )

        bool SelectBatchOrderPositionCollectionByKey::isExecutable() const
        {
            METHODNAME_DEF( SelectBatchOrderPositionCollectionByKey, isExecutable );
            static const log4cplus::Logger& logger = LoggerPool::loggerLibAbbaUW;
            BLOG_TRACE_METHOD( logger, fun );

            const bool executable = arePropertiesSet( properties::BRANCHNO.toPropertyString() );
            
            CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
            return executable;
        }

        void SelectBatchOrderPositionCollectionByKey::buildSQLString()
        {
            static const basar::VarString constSQL( "\n"
                "SELECT op.branchno AS "                                                + properties::BRANCHNO.getName()             +
                ", op.orderno AS "                                                      + properties::ORDERNO.getName()              +
                ", op.posno AS "                                                        + properties::POSNO.getName()                +
                ", ac.article_code AS "                                                 + properties::PZN.getName()                  +
                ", op.orderedqty AS "                                                   + properties::ORDEREDQTY.getName()           +
                ", TRIM(az.einheit) AS "                                                + properties::UNIT.getName()                 +
                ", az.artikel_name AS "                                                 + properties::ARTICLENAME.getName()          +
                ", NVL(ap.preisekapo, 0) AS "                                           + properties::ARTICLEPRICE.getName()         +
                ", az.darreichform AS "                                                 + properties::PHARMACEUTICALFORM.getName()   +
                ", CASE WHEN NVL(ao.datumverfall, 0) = 0 THEN ''"
			    " \n      ELSE (SUBSTRING(CAST(ao.datumverfall AS char(8)) FROM 3 FOR 2) || '/'"
			    " || SUBSTRING(CAST(ao.datumverfall AS char(8)) FROM 1 FOR 2))"
			    " END AS "                                                              + properties::EXPIRYDATE.getName()           +
                "\n FROM orderpos op "
                "\n LEFT JOIN artikelzentral az ON op.articleno = az.artikel_nr"
                "\n JOIN articlecodes ac ON op.articleno = ac.articleno AND ac.preferred_flag = 1"
                "\n LEFT JOIN artikellagerort ao ON op.articleno = ao.artikel_nr AND op.branchno = ao.filialnr"
                "\n LEFT JOIN artikelpreis ap ON op.articleno = ap.artikel_nr"
                "\n WHERE op.branchno = "                                               + properties::BRANCHNO.toSQLReplacementString() +
                " AND op.orderno = "                                                    + properties::ORDERNO.toSQLReplacementString()
            );

            resolve( constSQL );
            BLOG_TRACE_SQLSTRING( LoggerPool::loggerLibAbbaUW );
        }

} // end namespace batchOrder
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw
