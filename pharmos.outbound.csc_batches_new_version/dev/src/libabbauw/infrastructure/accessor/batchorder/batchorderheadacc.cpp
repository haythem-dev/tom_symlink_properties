//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/accessor.h>
#include <libutil/util.h>

#include "infrastructure/accessor/batchorder/batchorderheadacc.h"
#include "infrastructure/accessor/batchorder/batchorderheadacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

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
        BEGIN_ACCESSOR_DEFINITION( lit::ACC_BATCH_ORDER_HEAD )
            BEGIN_PROPERTY_DESCRIPTION_LIST
                PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCHNO             )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCHNO_ORIG        )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO              )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACYNO           )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERTYPE            )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNTPCTRANGE     )
                PROPERTY_DESCRIPTION_LIST_ADD (properties::DISCOUNTPCT          )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::STATUS               )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::POSQTY               )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASEORDERNO      ) //P_17000393
                PROPERTY_DESCRIPTION_LIST_ADD( properties::DEFERREDPAYMENTDATE  )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERACQUISITIONDATE )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERDATE            )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::DATASOURCETYPE       )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::EDIDELIVERYDATE      )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::HEADERTEXT           )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERVALUE           )

            ACCESS_METHOD( lit::SELECT_BATCH_ORDER_HEAD_BY_KEY   )
                SQL_BUILDER_CREATE   ( SelectBatchOrderHeadByKey )
                SQL_BUILDER_PUSH_BACK( SelectBatchOrderHeadByKey )

        END_ACCESSOR_DEFINITION

        //////////////////////////////////////////////////////////////////////////////////////////
        // BUILDER_DEFINITION( SelectBatchOrderHeadByKey )
        //////////////////////////////////////////////////////////////////////////////////////////
        BUILDER_DEFINITION( SelectBatchOrderHeadByKey )

        bool SelectBatchOrderHeadByKey::isExecutable() const
        {
            METHODNAME_DEF( SelectBatchOrderHeadByKey, isExecutable );
            static const log4cplus::Logger& logger = LoggerPool::loggerLibAbbaUW;
            BLOG_TRACE_METHOD( logger, fun );

            const bool executable = arePropertiesSet( properties::BRANCHNO.toPropertyString() + 
                                                      properties::ORDERNO.toPropertyString() 
                                                    );
            
            CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
            return executable;
        }

        void SelectBatchOrderHeadByKey::buildSQLString()
        {
            static const basar::VarString constSQL( "\n"
                "SELECT oh.branchno AS "                                                + properties::BRANCHNO.getName()             +
                ", oh.branchnoorig AS "                                                 + properties::BRANCHNO_ORIG.getName()        +
                ", oh.orderno AS "                                                      + properties::ORDERNO.getName()              +
                ", oh.purchaseorderno AS "                                              + properties::PURCHASEORDERNO.getName()      + //P_17000393
                ", oh.pharmacyno AS "                                                   + properties::PHARMACYNO.getName()           +
                ", oh.ordertype || oh.orderpickingtype || oh.bookentrytype AS "         + properties::ORDERTYPE.getName()            +
                ",\nCASE WHEN oh.status='0' THEN 'ZU' "
			     "\n     WHEN oh.status='1' THEN 'FE' "
			     "\n     WHEN oh.status='2' THEN 'ER' "
			     "\n     WHEN oh.status='9' THEN 'ST' "
                 "\n     ELSE '?' "
                  "\nEND AS "                                                           + properties::STATUS.getName()               +
                ", opa.posqty AS "                                                      + properties::POSQTY.getName()               +
                ", \nTO_DATE( CASE WHEN oh.deferredpaymentdate = 0 THEN "
                                   NULLVAL_SQL_DATE_STR
			                " ELSE "
                                  "TO_CHAR(oh.deferredpaymentdate)"
                            " END, "
                            FORMAT_SQL_DATE_IFX")::DATE AS "                            + properties::DEFERREDPAYMENTDATE.getName()  +
                ", \nTO_DATE( CASE WHEN oh.orderacquisitiondate = 0 THEN "
                                   NULLVAL_SQL_DATE_STR
			                " ELSE "
                                  "TO_CHAR(oh.orderacquisitiondate)"
                            " END, "
                            FORMAT_SQL_DATE_IFX")::DATE AS "                            + properties::ORDERACQUISITIONDATE.getName() +
                ", \nTO_DATE( CASE WHEN oh.orderdate = 0 THEN "
                                   NULLVAL_SQL_DATE_STR
			                " ELSE "
                                  "TO_CHAR(oh.orderdate)"
                            " END, "
                            FORMAT_SQL_DATE_IFX")::DATE AS "                            + properties::ORDERDATE.getName()            +
                ",\n oh.datasourcetype AS "                                             + properties::DATASOURCETYPE.getName()       +
                ", \nTO_DATE( CASE WHEN oh.edideliverydate = 0 THEN "
                                   NULLVAL_SQL_DATE_STR
			                " ELSE "
                                  "TO_CHAR(oh.edideliverydate)"
                            " END, "
                            FORMAT_SQL_DATE_IFX")::DATE AS "                            + properties::EDIDELIVERYDATE.getName()      +
                ", oh.headertext AS "                                                   + properties::HEADERTEXT.getName()           +
                "\nFROM orderhead oh"
                "\nLEFT JOIN ("
			    " SELECT  op.branchno"
			           ", op.orderno"
				       ", COUNT(op.posno)::INT AS posqty"
                       " FROM orderpos op"
                       " GROUP BY branchno, orderno"
                ") opa \n ON opa.branchno = oh.branchno AND opa.orderno = oh.orderno"
                "\nWHERE oh.branchno = "                                                + properties::BRANCHNO.toSQLReplacementString() +
                " AND oh.orderno = "                                                    + properties::ORDERNO.toSQLReplacementString()
            );

            resolve( constSQL );
            BLOG_TRACE_SQLSTRING( LoggerPool::loggerLibAbbaUW );
        }

} // end namespace batchOrder
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw
