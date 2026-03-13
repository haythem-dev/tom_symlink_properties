// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/tender/tenderprotocolacc.h>
#include <infrastructure/accessor/tender/tenderprotocolacc_definitions.h>

namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace tenderprotocol
            {
                BEGIN_ACCESSOR_DEFINITION( accessor::tenderprotocol::lit::TENDERPROTOCOL_ACC )

                    BEGIN_PROPERTY_DESCRIPTION_LIST
						PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_NO            )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_DATE           )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_NO             )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_NAME        )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NO		    )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NAME         )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY			    )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY_CHARGED	    )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY_TO_CHARGE   )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::SHOW_INVOICED_ORDERS )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ACC_CUSTOMER_NO		)
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ACC_BRANCH_NO		)
                    END_PROPERTY_DESCRIPTION_LIST

						ACCESS_METHOD( accessor::tenderprotocol::lit::INSERT_TENDERPROTOCOL );
                        SQL_BUILDER_CREATE   ( InsertTenderProtocol )
                        SQL_BUILDER_PUSH_BACK( InsertTenderProtocol )

                END_ACCESSOR_DEFINITION


				BUILDER_DEFINITION( InsertTenderProtocol )

                bool InsertTenderProtocol::isExecutable() const
                {
                    METHODNAME_DEF( InsertTenderProtocolBG, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = true;

					if( !arePropertiesSet( properties::ORDER_NO.toPropertyString() ) )
					{
						executable = false;
					}

					if( !arePropertiesSet( properties::ORDER_DATE.toPropertyString() ) )
					{
						executable = false;
					}

					if( !arePropertiesSet( properties::ARTICLE_NO.toPropertyString() ) )
					{
						executable = false;
					}

					if( !arePropertiesSet( properties::QUANTITY_TO_CHARGE.toPropertyString() ) )
					{
						executable = false;
					}

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

				void InsertTenderProtocol::buildSQLString()
                {
					using namespace properties;
					using basar::VarString;

					static const VarString sql_kdauftragnr( ORDER_NO.toSQLReplacementString()		  );
					static const VarString sql_orderdate  ( ORDER_DATE.toSQLReplacementString()		  );
					static const VarString sql_articleno  ( ARTICLE_NO.toSQLReplacementString()		  );
					static const VarString sql_qtytocharge( QUANTITY_TO_CHARGE.toSQLReplacementString() );

                    resolve(
                            "INSERT INTO tenderprotocol "
							"("
							"kdauftragnr, "
							"orderdate, "
							"articleno, "
							"chargedqty, "
							"dateinvoice, "
							"invoiceno "
							") "
							"VALUES " 
							"("																			+
							sql_kdauftragnr																+ ", " +
							"TO_CHAR(TO_DATE('" + sql_orderdate + "', '%m/%d/%Y'), '%Y%m%d')::INTEGER"	+ ", " +
							sql_articleno																+ ", " +
							sql_qtytocharge																+ ", " +
							"CAST( TO_CHAR( TODAY, '%Y%m%d' ) AS INT ) "								+ ", " + 
							"0"
							");"
						   );

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

            } // end namespace tenderprotocol
        }	  // end namespace accessor
    }		  // end namespace infrastructure
}			  // end namespace libtender
