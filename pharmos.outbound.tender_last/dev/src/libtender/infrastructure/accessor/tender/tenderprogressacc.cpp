// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/tender/tenderprogressacc.h>
#include <infrastructure/accessor/tender/tenderprogressacc_definitions.h>

namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace tenderprogress
            {
                BEGIN_ACCESSOR_DEFINITION( accessor::tenderprogress::lit::TENDERPROGRESS_ACC )

                    BEGIN_PROPERTY_DESCRIPTION_LIST
						PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_NO		  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NO		  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NAME		  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_NAME	  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_NO			  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_DATE		  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY			  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY_CHARGED	  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY_DELIVERED )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY_TO_CHARGE )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ACC_CUSTOMER_NO	  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ACC_BRANCH_NO	  )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::DELIVERY_QTY    )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::RETURN_QUANTITY      )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::CHARGED_PRICE        )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::INVOICE_NO           )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::INVOICE_DATE         )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::GEP                  )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER                )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_TIME           )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::INVOICE_TIME         )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::BOOKING_TENDERNO     )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO          )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO            )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNT_PCT         )

                    END_PROPERTY_DESCRIPTION_LIST

                        ACCESS_METHOD( accessor::tenderprogress::lit::SELECT_TENDERPROGRESS_BY_ARTICLEQUANTITY );
                        SQL_BUILDER_CREATE   ( SelectArticleQuantity )
                        SQL_BUILDER_PUSH_BACK( SelectArticleQuantity )

                        ACCESS_METHOD( accessor::tenderprogress::lit::SELECT_TENDERPROGRESS_BY_PATTERN );
                        SQL_BUILDER_CREATE   ( SelectTenderProgressByPattern )
                        SQL_BUILDER_PUSH_BACK( SelectTenderProgressByPattern )

						ACCESS_METHOD( accessor::tenderprogress::lit::SELECT_TENDERPROGRESS_BY_TENDER );
                        SQL_BUILDER_CREATE   ( SelectTenderProgressByTender )
                        SQL_BUILDER_PUSH_BACK( SelectTenderProgressByTender )

                        ACCESS_METHOD( accessor::tenderprogress::lit::SELECT_TENDERPROGRESS_FOR_OMGCALCULATION );
                        SQL_BUILDER_CREATE( SelectTenderProgressForOMGCalculation )
                        SQL_BUILDER_PUSH_BACK( SelectTenderProgressForOMGCalculation )

                        ACCESS_METHOD( accessor::tenderprogress::lit::SAVE_TENDER_PROGRESS )
                        SQL_BUILDER_CREATE( UpdateTenderProgress )
                        SQL_BUILDER_PUSH_BACK( UpdateTenderProgress )
                        SQL_BUILDER_CREATE( InsertTenderProgress )
                        SQL_BUILDER_PUSH_BACK( InsertTenderProgress )
                        SQL_BUILDER_CREATE( DeleteTenderProgress )
                        SQL_BUILDER_PUSH_BACK( DeleteTenderProgress )


                END_ACCESSOR_DEFINITION


                BUILDER_DEFINITION( SelectTenderProgressByPattern )
                bool SelectTenderProgressByPattern::isExecutable() const
                {
                    METHODNAME_DEF( SelectTenderProgressByPattern, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

                    bool executable = false;

                    if( arePropertiesSet( properties::ORDER_NO.toPropertyString()  ) &&
                        arePropertiesSet( properties::ARTICLE_NO.toPropertyString() ) && 
                        arePropertiesSet( properties::ORDER_DATE.toPropertyString() ) 
                    )
                    {
                        executable = true;
                    }

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
                    return executable;
                }

                void SelectTenderProgressByPattern::buildSQLString()
                {
                    static const basar::VarString constSQL(
                            "SELECT p.tenderno AS "                         + properties::TENDER_NO.getName()   +
                            ", p.articleno AS "                             + properties::ARTICLE_NO.getName()   +
                            ", p.kdauftragnr AS  "                          + properties::ORDER_NO.getName() +
                            ", p.orderdate AS  "                            + properties::ORDER_DATE.getName() +
                            ", p.deliveryqty AS  "                          + properties::DELIVERY_QTY.getName() +
                            ", p.chargedprice AS  "                         + properties::CHARGED_PRICE.getName() +
                            ", p.posnr AS  "                                + properties::POS_ID.getName() +
                            ", p.returnqty AS  "                            + properties::RETURN_QUANTITY.getName() +
                            " FROM tenderprogress AS p"                     +
                            " WHERE 1=1 ");

                    basar::VarString variableSQL;

                    if( arePropertiesSetAndOneMustBeInState(properties::ORDER_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        variableSQL.append( " AND p.kdauftragnr = "  + properties::ORDER_NO.toSQLReplacementString() );
                    }

                    if( arePropertiesSetAndOneMustBeInState(properties::ARTICLE_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        variableSQL.append( " AND p.articleno = " + properties::ARTICLE_NO.toSQLReplacementString() );
                    }

                    if( arePropertiesSetAndOneMustBeInState(properties::ORDER_DATE.toPropertyString(), basar::SS_CLEAN) )
                    {
                        variableSQL.append( " AND p.orderdate = " + properties::ORDER_DATE.toSQLReplacementString() );
                    }

                   resolve( constSQL + variableSQL );

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );

                }

                BUILDER_DEFINITION( SelectArticleQuantity )
                bool SelectArticleQuantity::isExecutable() const
                {
                    METHODNAME_DEF( SelectArticleQuantity, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = false;

					if( arePropertiesSet( properties::TENDER_NO.toPropertyString() ) || arePropertiesSet( properties::ARTICLE_NO.toPropertyString() ) )
					{
						executable = true;
					}

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectArticleQuantity::buildSQLString()
                {
                    static const basar::VarString constSQL(
                            "SELECT p.tenderno AS "															+ properties::TENDER_NO.getName()   +
							", p.articleno AS "																+ properties::ARTICLE_NO.getName() +
							", CAST(CAST(SUM( p.deliveryqty ) AS INT) - CAST(SUM( p.returnqty ) AS INT) AS INT) AS  "	+ properties::QUANTITY_DELIVERED.getName() +
							" FROM tenderprogress AS p"														+
                            " WHERE 1=1 ");

                    basar::VarString variableSQL;

                    if( arePropertiesSetAndOneMustBeInState(properties::TENDER_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        variableSQL.append( " AND p.tenderno = "  + properties::TENDER_NO.toSQLReplacementString() );
                    }

					if( arePropertiesSetAndOneMustBeInState(properties::ARTICLE_NO.toPropertyString(), basar::SS_CLEAN) )
					{
						variableSQL.append( " AND p.articleno = " + properties::ARTICLE_NO.toSQLReplacementString() );
					}

                    basar::VarString organizeResult(" GROUP BY p.tenderno, p.articleno ");

                    resolve( constSQL + variableSQL + organizeResult);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }


				BUILDER_DEFINITION( SelectTenderProgressByTender )

                bool SelectTenderProgressByTender::isExecutable() const
                {
                    METHODNAME_DEF( SelectTenderProgressByTender, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = false;

					if( arePropertiesSet( properties::TENDER_NO.toPropertyString() ) && 
						arePropertiesSet( properties::SHOW_INVOICED_ORDERS.toPropertyString() ) )
					{
						executable = true;
					}

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectTenderProgressByTender::buildSQLString()
                {
                    static const basar::VarString constSQL(
                            "SELECT "
							"prog.tenderno AS "																									+ properties::TENDER_NO.getName()		   + ", "
							"prog.articleno AS "																								+ properties::ARTICLE_NO.getName()	       + ", "
							"prog.kdauftragnr AS "																								+ properties::ORDER_NO.getName()		   + ", "
							"TO_DATE(prog.orderdate::CHAR(8), '%Y%m%d')::DATE AS "																+ properties::ORDER_DATE.getName()		   + ", "
							"prog.deliveryqty AS "																								+ properties::QUANTITY.getName()		   + ", "
							"NVL(SUM(p.chargedqty)::INT, 0) AS "																				+ properties::QUANTITY_CHARGED.getName()   + ", "
							"az.artikel_name AS "																								+ properties::ARTICLE_NAME.getName()	   + ", "
							"kd.nameapo AS "																									+ properties::PHARMACY_NAME.getName()      + ", "
							"(SELECT acccustomerno FROM tenderhead WHERE tenderno=" + properties::TENDER_NO.toSQLReplacementString() + ") AS "	+ properties::ACC_CUSTOMER_NO.getName()	   + ", "
							"(SELECT accbranchno FROM tenderhead WHERE tenderno=" + properties::TENDER_NO.toSQLReplacementString() + ") AS "	+ properties::ACC_BRANCH_NO.getName()	   + ", "
							"0 AS "																												+ properties::QUANTITY_TO_CHARGE.getName() + " "
							"FROM tenderprogress AS prog "												
                            "LEFT JOIN tenderprotocol as p on p.kdauftragnr = prog.kdauftragnr " 
							"LEFT JOIN artikelzentral az ON az.artikel_nr=prog.articleno "
							"LEFT JOIN kdauftrag ka ON ka.kdauftragnr=prog.kdauftragnr "
							"LEFT JOIN akdauftrag aka ON aka.kdauftragnr=prog.kdauftragnr AND aka.datum=prog.orderdate "
							"LEFT JOIN kunde kd ON (kd.vertriebszentrumnr=ka.vertriebszentrumnr AND kd.idfnr=ka.idfnr) OR (kd.vertriebszentrumnr=aka.vertriebszentrumnr AND kd.idfnr=aka.idfnr) "
							"WHERE ");

                    basar::VarString variableSQL;

                    if( arePropertiesSetAndOneMustBeInState(properties::TENDER_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        variableSQL.append( "prog.tenderno = "  + properties::TENDER_NO.toSQLReplacementString() );
                    }

					basar::VarString organizeResult( " GROUP BY prog.tenderno, prog.orderdate, prog.kdauftragnr, prog.articleno, prog.deliveryqty, az.artikel_name, kd.nameapo, aka.idfnr " );

					if( 1 ==  getPropertyList().getInt16( properties::SHOW_INVOICED_ORDERS ) )
					{
						organizeResult.append( " HAVING prog.deliveryqty - NVL(sum(p.chargedqty),0) > 0 " );
					}
                    else
                    {
                        organizeResult.append( " HAVING prog.deliveryqty = NVL(sum(p.chargedqty), 0) " );
                    }

					organizeResult.append(" ORDER BY TO_DATE(prog.orderdate::CHAR(8), '%Y%m%d')::DATE, prog.kdauftragnr" );
										

                    resolve( constSQL + variableSQL + organizeResult );

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

                BUILDER_DEFINITION( UpdateTenderProgress )

                bool UpdateTenderProgress::isExecutable() const
                {

                    const bool executable = arePropertiesSet( properties::TENDER_NO.toPropertyString() ) &&
                                            arePropertiesSet( properties::ORDER_NO.toPropertyString() ) &&
                                            arePropertiesSet( properties::POS_ID.toPropertyString() ) &&
                                            arePropertiesSet( properties::ORDER_DATE.toPropertyString() ) &&
                                            arePropertiesSet( properties::ARTICLE_NO.toPropertyString() ) &&
                                            arePropertiesSetAndAllInState( properties::RETURN_QUANTITY.toPropertyString(), basar::SS_UPDATE );
                    return executable;
                }

                void UpdateTenderProgress::buildSQLString()
                {
                    using namespace properties;
                    static const basar::VarString constSQL( 
						"UPDATE tenderprogress "
						" set returnqty = " + properties::RETURN_QUANTITY.toSQLReplacementString() +
						" , changedate = TO_CHAR(TODAY, '%Y%m%d')::INTEGER "
						" WHERE tenderno = " + TENDER_NO.toSQLReplacementString()      +
						" and kdauftragnr = " + ORDER_NO.toSQLReplacementString()  + 
						" AND posnr = " + properties::POS_ID.toSQLReplacementString() +
						" AND articleno = " + properties::ARTICLE_NO.toSQLReplacementString() +
						" and orderdate = '" + ORDER_DATE.toSQLReplacementString()        + "' ;"
							);
                    resolve( constSQL );
                }

                BUILDER_DEFINITION( InsertTenderProgress )

                bool InsertTenderProgress::isExecutable() const
                {
                    const bool executable = arePropertiesSetAndAllInState( properties::TENDER_NO.toPropertyString(),basar::SS_INSERT) &&
                                        arePropertiesSetAndAllInState( properties::ARTICLE_NO.toPropertyString(),basar::SS_INSERT) &&
                                        arePropertiesSetAndAllInState( properties::ORDER_NO.toPropertyString(),basar::SS_INSERT) &&
                                        arePropertiesSetAndAllInState( properties::ORDER_DATE.toPropertyString(), basar::SS_INSERT) &&
                                        arePropertiesSetAndAllInState( properties::DELIVERY_QTY.toPropertyString(), basar::SS_INSERT) &&
                                        arePropertiesSetAndAllInState( properties::CHARGED_PRICE.toPropertyString(), basar::SS_INSERT) &&
                                        arePropertiesSetAndAllInState( properties::POS_ID.toPropertyString(), basar::SS_INSERT) &&
                                        arePropertiesSetAndAllInState( properties::RETURN_QUANTITY.toPropertyString(), basar::SS_INSERT);

                    return executable;
                }

                void InsertTenderProgress::buildSQLString()
                {
                    using namespace properties;
                    static const basar::VarString insertSQL ( 
                            "INSERT INTO tenderprogress "
                            "( "
                            "tenderno, "
                            "articleno, "
                            "kdauftragnr, "
                            "orderdate, "
                            "deliveryqty, "
                            "chargedprice, "
                            "posnr, "
                            "returnqty, "
                            "ordertime, "
                            "customerno, "
                            "bookingtenderno, "
                            "invoiceno, "
                            "invoicedate, "
                            "gep, "
                            "invoicetime, "
                            "discountpct, "
                            "branchno, "
                            "changedate "
                            ") "
                            "VALUES "
                            "("
                                + TENDER_NO.toSQLReplacementString()                    +", "
                                + ARTICLE_NO.toSQLReplacementString()                   +", "
                                + ORDER_NO.toSQLReplacementString()                     +", "
                                + "TO_CHAR(TO_DATE('" + properties::ORDER_DATE.toSQLReplacementString() + "', '%m/%d/%Y'), '%Y%m%d')::INTEGER, " 
                                + DELIVERY_QTY.toSQLReplacementString()            +", "
                                + CHARGED_PRICE.toSQLReplacementString()                +", "
                                + POS_ID.toSQLReplacementString()                       +", "
                                + RETURN_QUANTITY.toSQLReplacementString()              +", "
                                + ORDER_TIME .toSQLReplacementString()                  +", "
                                + CUSTOMER_NO .toSQLReplacementString()                 +", "
                                + BOOKING_TENDERNO.toSQLReplacementString()             +", "
                                + INVOICE_NO.toSQLReplacementString()                   +", "
                                + "TO_CHAR(TO_DATE('" + properties::INVOICE_DATE.toSQLReplacementString() + "', '%m/%d/%Y'), '%Y%m%d')::INTEGER, "
                                + GEP.toSQLReplacementString()                          +", "
                                + ORDER_TIME .toSQLReplacementString()                  +", "
                                + DISCOUNT_PCT.toSQLReplacementString()                 +", "
                                + BRANCH_NO.toSQLReplacementString()                    +", "
								"TO_CHAR(TODAY, '%Y%m%d')::INTEGER "
                            ")");

                    resolve( insertSQL );
                }

                BUILDER_DEFINITION( SelectTenderProgressForOMGCalculation )
                bool SelectTenderProgressForOMGCalculation::isExecutable() const
                {
                    METHODNAME_DEF( SelectTenderProgressForOMGCalculation, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

                    bool executable = false;

                    if( arePropertiesSet( properties::TENDER_NO.toPropertyString() ) )
                    {
                        executable = true;
                    }

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
                    return executable;
                }

                void SelectTenderProgressForOMGCalculation::buildSQLString()
                {
                    static const basar::VarString constSQL(
                            "SELECT "
                            "prog.tenderno AS "                                       + properties::TENDER_NO.getName() + ", "
                            "prog.articleno AS "                                      + properties::ARTICLE_NO.getName() + ", "
                            "prog.kdauftragnr AS "                                    + properties::ORDER_NO.getName() + ", "
                            "TO_DATE(prog.orderdate::CHAR(8), '%Y%m%d')::DATE AS "    + properties::ORDER_DATE.getName() + ", "
                            "prog.deliveryqty AS "                                    + properties::DELIVERY_QTY.getName() + ", "
                            "CAST(prog.deliveryqty - prog.returnqty  AS INT) AS "     + properties::QUANTITY.getName() + ", "
                            "NVL(kd.idfnr,akd.idfnr) AS "                             + properties::CUSTOMER_NO.getName() + ", "
                            "NVL(kd.filialnr,akd.filialnr) AS "                       + properties::BRANCH_NO.getName() + ", "
                            "prog.chargedprice AS "                                   + properties::CHARGED_PRICE.getName() + ", "
                            "prog.posnr AS "                                          + properties::POS_ID.getName() + ", "
                            //"prog.ordertime AS "                                      + properties::ORDER_TIME.getName() + ", "
                            "prog.gep AS "                                            + properties::GEP.getName() + ", "
                            "prog.bookingtenderno AS "                                + properties::BOOKING_TENDERNO.getName() + ", "
                            "prog.invoiceno AS "                                      + properties::INVOICE_NO.getName() + ", "
                            "prog.discountpct AS "                                    + properties::DISCOUNT_PCT.getName() + ", "
                           // "TO_DATE(prog.invoicedate::CHAR(8), '%Y%m%d')::DATE AS "  + properties::INVOICE_DATE.getName() + ", "
                            "prog.returnqty AS "                                      + properties::RETURN_QUANTITY.getName() + " "
                            "FROM tenderprogress AS prog "
                            "LEFT JOIN kdauftrag kd ON kd.kdauftragnr = prog.kdauftragnr "
                            "LEFT JOIN akdauftrag akd ON akd.kdauftragnr = prog.kdauftragnr "
                            "WHERE "
                            "prog.tenderno = "  + properties::TENDER_NO.toSQLReplacementString() + " "
                            "ORDER BY prog.articleno, prog.orderdate, prog.ordertime "
                            );

                    resolve( constSQL );
                }

                BUILDER_DEFINITION( DeleteTenderProgress )

                bool DeleteTenderProgress::isExecutable() const
                {

                    const bool executable = arePropertiesSetAndAllInState( properties::TENDER_NO.toPropertyString(), basar::SS_DELETE ) &&
                                            arePropertiesSetAndAllInState( properties::ORDER_NO.toPropertyString(), basar::SS_DELETE ) &&
                                            arePropertiesSetAndAllInState( properties::POS_ID.toPropertyString(), basar::SS_DELETE ) &&
                                            arePropertiesSetAndAllInState( properties::ORDER_DATE.toPropertyString(), basar::SS_DELETE ) &&
                                            arePropertiesSetAndAllInState( properties::ARTICLE_NO.toPropertyString(), basar::SS_DELETE );
                    return executable;
                }

                void DeleteTenderProgress::buildSQLString()
                {
                    using namespace properties;
                    static const basar::VarString constSQL
                        ( 
                            "DELETE from tenderprogress "
                            "WHERE "
                            "tenderno = "                                             + TENDER_NO.toSQLReplacementString() + " "
                            "AND "
                            "kdauftragnr = "                                          + ORDER_NO.toSQLReplacementString() + " "
                            "AND "
                            "posnr = "                                                + properties::POS_ID.toSQLReplacementString() + " "
                            "AND "
                            "articleno = "                                            + properties::ARTICLE_NO.toSQLReplacementString() + " "
                            "AND "
                            "TO_DATE(orderdate::CHAR(8), '%Y%m%d')::DATE = TO_DATE('" + properties::ORDER_DATE.toSQLReplacementString() + "', '%m/%d/%Y') "
                        );
                    resolve( constSQL );
                }

            } // end namespace tender
        }	  // end namespace accessor
    }		  // end namespace infrastructure
}			  // end namespace libtender
