//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/accessor.h>
#include <libutil/util.h>

#include "infrastructure/accessor/batchorder/batchordercollectionacc.h"
#include "infrastructure/accessor/batchorder/batchordercollectionacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

#include "orderstatus.h"
#include "sortorder.h"
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
        BEGIN_ACCESSOR_DEFINITION( lit::ACC_BATCH_ORDER_COLLECTION )
            BEGIN_PROPERTY_DESCRIPTION_LIST
                PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCHNO             )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO              )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASEORDERNO      )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACYNO           )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACYNAME         )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::STATUS               )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::POSQTY               )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERACQUISITIONDATE )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::DATASOURCETYPE       )
                PROPERTY_DESCRIPTION_LIST_ADD( properties::SUM_UNKNOWN_ARTICLES )

            ACCESS_METHOD( lit::SELECT_BATCH_ORDER_COLLECTION_BY_PATTERN )
                SQL_BUILDER_CREATE   ( SelectBatchOrderCollectionByPattern )
                SQL_BUILDER_PUSH_BACK( SelectBatchOrderCollectionByPattern )

        END_ACCESSOR_DEFINITION

        //////////////////////////////////////////////////////////////////////////////////////////
        // BUILDER_DEFINITION( SelectBatchOrderCollectionByPattern )
        //////////////////////////////////////////////////////////////////////////////////////////
        BUILDER_DEFINITION( SelectBatchOrderCollectionByPattern )

        bool SelectBatchOrderCollectionByPattern::isExecutable() const
        {
            METHODNAME_DEF( SelectBatchOrderCollectionByPattern, isExecutable );
            static const log4cplus::Logger& logger = LoggerPool::loggerLibAbbaUW;
            BLOG_TRACE_METHOD( logger, fun );

            const bool executable = arePropertiesSet( properties::BRANCHNO.toPropertyString() );
            
            CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
            return executable;
        }

        void SelectBatchOrderCollectionByPattern::buildSQLString()
        {
            //P_17000393
            static const basar::VarString constSQL( "\n"
                "SELECT oh.branchno AS "                                                + properties::BRANCHNO.getName()             +
                ", oh.orderno AS "                                                      + properties::ORDERNO.getName()              +
                ", oh.purchaseorderno AS "                                              + properties::PURCHASEORDERNO.getName()      +
                ", oh.pharmacyno AS "                                                   + properties::PHARMACYNO.getName()           +
                ", k.matchcode AS "                                                     + properties::PHARMACYNAME.getName()         +
                ",\nCASE WHEN oh.status='0' THEN 'ZU' "
			     "\n     WHEN oh.status='1' THEN 'FE' "
			     "\n     WHEN oh.status='2' THEN 'ER' "
			     "\n     WHEN oh.status='9' THEN 'ST' "
                 "\n     ELSE '?' "
                  "\nEND AS "                                                           + properties::STATUS.getName()               +
			    ", \nCASE WHEN oh.status='0' THEN '1' "
                  "\n     WHEN oh.status='1' THEN '0' "
                  "\n     ELSE oh.status"
                "\n END AS sortstatus"
                ", opa.posqty AS "                                                      + properties::POSQTY.getName()               +
                ", \nTO_DATE( CASE WHEN oh.orderacquisitiondate = 0 THEN "
                                   NULLVAL_SQL_DATE_STR
			                " ELSE "
                                  "TO_CHAR(oh.orderacquisitiondate)"
                            " END, "
                            FORMAT_SQL_DATE_IFX")::DATE AS "                            + properties::ORDERACQUISITIONDATE.getName() +
                ", opa.sum_unknown_articles AS "                                        + properties::SUM_UNKNOWN_ARTICLES.getName() +
                ",\n oh.datasourcetype AS "                                             + properties::DATASOURCETYPE.getName()       +
                "\nFROM orderhead oh"
                "\nLEFT JOIN kunde k ON oh.branchno = k.filialnr AND oh.pharmacyno = k.idfnr"
                "\nLEFT JOIN ("
			    " SELECT  op.branchno"
			           ", op.orderno"
				       ", COUNT(op.posno)::INT AS posqty"
                       ", SUM( NVL2(az.artikel_nr, 0, 1) )::INT AS sum_unknown_articles"
                       " FROM orderpos op"
                       " LEFT JOIN artikelzentral az ON op.articleno = az.artikel_nr"
                       " GROUP BY branchno, orderno"
                ") opa \n ON opa.branchno = oh.branchno AND opa.orderno = oh.orderno"
                "\nWHERE oh.branchno = "                                                + properties::BRANCHNO.toSQLReplacementString() +
                " AND oh.datasourcetype IN (SELECT value FROM cscini WHERE branchno = " + properties::BRANCHNO.toSQLReplacementString() +
                " AND key = 'datasourcetype_gui')"
            );

            basar::VarString variableSQL;
            if( isContainedAndSet( properties::PHARMACYNO ) )
            {
                variableSQL.append(" AND oh.pharmacyno = " + properties::PHARMACYNO.toSQLReplacementString() );
            }

            if( isContainedAndSet( properties::PHARMACYNAME ) && ! getPropertyList().getString( properties::PHARMACYNAME ).empty() )
            {
                variableSQL.append(" AND NVL(k.matchcode, '') LIKE UPPER('%" + properties::PHARMACYNAME.toSQLReplacementString() + "%')" );
            }

            if( isContainedAndSet( properties::ORDERNO_FROM ) )
            {
                variableSQL.append(" AND oh.orderno >= " + properties::ORDERNO_FROM.toSQLReplacementString() );
            }

            if( isContainedAndSet( properties::ORDERNO_TO ) )
            {
                variableSQL.append(" AND oh.orderno <= " + properties::ORDERNO_TO.toSQLReplacementString() );
            }

            if( isContainedAndSet( properties::STATUS ) )
		    {
                libabbauw::domMod::OrderStatus status( getPropertyList().getString( properties::STATUS.getName() ) );
                std::ostringstream s;
                s << basar::I18nString(" AND oh.status = '") << status.getStatus() << "'";
                variableSQL.append( s.str().c_str() );
		    }

            if( isContainedAndSet( properties::POS_FROM ) )
		    {
                std::ostringstream s;
                s << basar::I18nString(" AND opa.posqty >= ") << getPropertyList().getInt32( properties::POS_FROM );
                variableSQL.append( s.str().c_str() );
		    }

            if( isContainedAndSet( properties::POS_TO ) )
		    {
                std::ostringstream s;
                s << basar::I18nString(" AND opa.posqty <= ") << getPropertyList().getInt32( properties::POS_TO );
                variableSQL.append( s.str().c_str() );
		    }

            if( isContainedAndSet( properties::ORDERACQUISITIONDATE_FROM ) && getPropertyList().getDate( properties::ORDERACQUISITIONDATE_FROM ).isValid() )
		    {
                basar::I18nString date;
                date.itos( getPropertyList().getDate( properties::ORDERACQUISITIONDATE_FROM ).getDate() );
			    variableSQL.append( " AND (oh.orderacquisitiondate = 0 OR oh.orderacquisitiondate >= " + date + ")" );
		    }

            if( isContainedAndSet( properties::ORDERACQUISITIONDATE_TO ) && getPropertyList().getDate( properties::ORDERACQUISITIONDATE_TO ).isValid() )
		    {
                basar::I18nString date;
                date.itos( getPropertyList().getDate( properties::ORDERACQUISITIONDATE_TO ).getDate() );
			    variableSQL.append( " AND (oh.orderacquisitiondate = 0 OR oh.orderacquisitiondate <= " + date + ")" );
		    }

            if( isContainedAndSet( properties::DATASOURCETYPE ) )
            {
                variableSQL.append( " AND oh.datasourcetype = UPPER('" + properties::DATASOURCETYPE.toSQLReplacementString() + "')" );
            }

            if( isContainedAndSet( properties::SORT ) )
		    {
                basar::VarString orderStatement;
			    orderStatement = "\nORDER BY " + getPropertyList().getString( properties::SORT.getName() );
			    // Replace column 'status' with 'sortstatus' which is sortable
			    orderStatement.exchange( properties::STATUS.getName(), "sortstatus" );
                variableSQL.append( orderStatement );
		    }

            resolve( constSQL + variableSQL );
            BLOG_TRACE_SQLSTRING( LoggerPool::loggerLibAbbaUW );
        }

} // end namespace batchOrder
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw
