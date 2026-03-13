#include "omgcustomercalcacc.h"
#include "omgcustomercalcacc_definitions.h"
#include "omgcustomercalcacc_definitions.h"
#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/loggerpool.h>

namespace tenderbatchprocessing
{
namespace infrastructure 
{
namespace accessor 
{
namespace omgcustomercalc 
{


BEGIN_ACCESSOR_DEFINITION( lit::OMGCUSTOMERCALC_ACC )

    BEGIN_PROPERTY_DESCRIPTION_LIST
    PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_NO );
    PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO );
    PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NO );
    PROPERTY_DESCRIPTION_LIST_ADD( properties::PRICE_DIFFERENCE );
    PROPERTY_DESCRIPTION_LIST_ADD( properties::TRANSFERED_FLAG );
    END_PROPERTY_DESCRIPTION_LIST

    ACCESS_METHOD( lit::SELECT_OMG_CUSTOMER_CALC_BY_ID );
    SQL_BUILDER_CREATE   ( SelectOMGCustomerCalcById )
    SQL_BUILDER_PUSH_BACK( SelectOMGCustomerCalcById )

    ACCESS_METHOD( lit::SELECT_OMG_CUSTOMER_CALC_BY_PATTERN );
    SQL_BUILDER_CREATE   ( SelectOMGCustomerCalcByPattern )
    SQL_BUILDER_PUSH_BACK( SelectOMGCustomerCalcByPattern )

    ACCESS_METHOD( lit::SAVE_OMG_CUSTOMER_CALC )
    SQL_BUILDER_CREATE( UpdateOMGCustomerCalc )
    SQL_BUILDER_PUSH_BACK( UpdateOMGCustomerCalc )
    SQL_BUILDER_CREATE( InsertOMGCustomerCalc )
    SQL_BUILDER_PUSH_BACK( InsertOMGCustomerCalc )


END_ACCESSOR_DEFINITION

//////////////////////////////////////////////////////////////////////////////////////////
// BUILDER_DEFINITION( SelectOMGCalcById )
//////////////////////////////////////////////////////////////////////////////////////////
BUILDER_DEFINITION( SelectOMGCustomerCalcById )

bool SelectOMGCustomerCalcById::isExecutable() const
{
    METHODNAME_DEF( SelectTenderProgressByPattern, isExecutable );
    static const log4cplus::Logger& logger = LoggerPool::getLoggerDomModules();
    BLOG_TRACE_METHOD( logger, fun );

    bool executable = false;

    if( arePropertiesSet( properties::TENDER_NO.toPropertyString() ) )
    {
        executable = true;
    }

    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    return executable;
}

void SelectOMGCustomerCalcById::buildSQLString()
{
   static const basar::VarString constSQL(
                "SELECT tc.tenderno AS "                      + properties::TENDER_NO.getName() + ", "
                "tc.articleno AS "                            + properties::ARTICLE_NO.getName() + ", "
                "tc.customerno AS  "                          + properties::CUSTOMER_NO.getName() + ", "
                "tc.priceDifference AS  "                     + properties::PRICE_DIFFERENCE.getName() + ", "
                "tc.transferredflag AS  "                     + properties::TRANSFERED_FLAG.getName() + " "
                "FROM tendercustomercalc AS tc "
                "WHERE "
                "tc.tenderno = "                                 + properties::TENDER_NO.toSQLReplacementString()
                );
    resolve( constSQL );

    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( SelectOMGCustomerCalcByPattern )

bool SelectOMGCustomerCalcByPattern::isExecutable() const
{
    METHODNAME_DEF( SelectTenderProgressByPattern, isExecutable );
    static const log4cplus::Logger& logger = LoggerPool::getLoggerDomModules();
    BLOG_TRACE_METHOD( logger, fun );

    bool executable = false;

    if( arePropertiesSet( properties::TENDER_NO.toPropertyString()  ) ||
        arePropertiesSet( properties::ARTICLE_NO.toPropertyString() ) || 
        arePropertiesSet( properties::CUSTOMER_NO.toPropertyString() ) 
    )
    {
        executable = true;
    }

    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    return executable;
}

void SelectOMGCustomerCalcByPattern::buildSQLString()
{
   static const basar::VarString constSQL(
                "SELECT tc.tenderno AS "                      + properties::TENDER_NO.getName() + ", "
                "tc.articleno AS "                            + properties::ARTICLE_NO.getName() + ", "
                "tc.customerno AS  "                          + properties::CUSTOMER_NO.getName() + ", "
                "tc.priceDifference AS  "                     + properties::PRICE_DIFFERENCE.getName() + ", "
                "tc.transferredflag AS  "                     + properties::TRANSFERED_FLAG.getName() + " "
                "FROM tendercustomercalc AS tc "              +
                "WHERE 1=1 ");

    basar::VarString variableSQL;

    if( arePropertiesSetAndOneMustBeInState(properties::TENDER_NO.toPropertyString(), basar::SS_CLEAN) )
    {
        variableSQL.append( " AND tc.tenderno = "  + properties::TENDER_NO.toSQLReplacementString() );
    }

    if( arePropertiesSetAndOneMustBeInState(properties::ARTICLE_NO.toPropertyString(), basar::SS_CLEAN) )
    {
        variableSQL.append( " AND tc.articleno = " + properties::ARTICLE_NO.toSQLReplacementString() );
    }

    if( arePropertiesSetAndOneMustBeInState(properties::CUSTOMER_NO.toPropertyString(), basar::SS_CLEAN) )
    {
        variableSQL.append( " AND tc.customerno = " + properties::CUSTOMER_NO.toSQLReplacementString() );
    }

    resolve( constSQL + variableSQL );

    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( UpdateOMGCustomerCalc )
bool UpdateOMGCustomerCalc::isExecutable() const
{
    METHODNAME_DEF( SelectTenderProgressByPattern, isExecutable );
    static const log4cplus::Logger& logger = LoggerPool::getLoggerDomModules();
    BLOG_TRACE_METHOD( logger, fun );

    bool executable = false;

    if( (
        arePropertiesSet( properties::TENDER_NO.toPropertyString() ) &&
        arePropertiesSet( properties::ARTICLE_NO.toPropertyString() ) && 
        arePropertiesSet( properties::CUSTOMER_NO.toPropertyString() ) &&
        arePropertiesSet( properties::PRICE_DIFFERENCE.toPropertyString() ) &&
        arePropertiesSet( properties::TRANSFERED_FLAG.toPropertyString() ) 
        ) &&
        (
        arePropertiesSetAndAllInState( properties::PRICE_DIFFERENCE.toPropertyString(), basar::SS_UPDATE ) ||
        arePropertiesSetAndAllInState( properties::TRANSFERED_FLAG.toPropertyString(), basar::SS_UPDATE ) 
        )
    )
    {
        executable = true;
    }

    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    return executable;
}

void UpdateOMGCustomerCalc::buildSQLString()
{
    //using namespace properties;
   static const basar::VarString constSQL( 
        "UPDATE tendercustomercalc "
        "SET "
        "tenderNo = "                  + properties::TENDER_NO.toSQLReplacementString() + ", "
        "customerNo = "                + properties::CUSTOMER_NO.toSQLReplacementString() + ", "
        "articleno = "                 + properties::ARTICLE_NO.toSQLReplacementString() + ", "
        "pricedifference = "           + properties::PRICE_DIFFERENCE.toSQLReplacementString() + ", "
        "transferredflag = "           + properties::TRANSFERED_FLAG.toSQLReplacementString() + " "
        "WHERE "
        "tenderno = "                 + properties::TENDER_NO.toSQLReplacementString() + " "
        "AND "
        "articleno = "                + properties::ARTICLE_NO.toSQLReplacementString() + " "
        "AND "
        "customerNo = "               + properties::CUSTOMER_NO.toSQLReplacementString() + " "
        );
    resolve( constSQL );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( InsertOMGCustomerCalc )
bool InsertOMGCustomerCalc::isExecutable() const
{
    METHODNAME_DEF( SelectTenderProgressByPattern, isExecutable );
    static const log4cplus::Logger& logger = LoggerPool::getLoggerDomModules();
    BLOG_TRACE_METHOD( logger, fun );

    bool executable = false;

    if( arePropertiesSetAndAllInState( properties::TENDER_NO.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::ARTICLE_NO.toPropertyString(), basar::SS_INSERT ) && 
        arePropertiesSetAndAllInState( properties::CUSTOMER_NO.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::PRICE_DIFFERENCE.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::TRANSFERED_FLAG.toPropertyString(), basar::SS_INSERT ) 
    )
    {
        executable = true;
    }

    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    return executable;
}

void InsertOMGCustomerCalc::buildSQLString()
{
    //using namespace properties;
    static const basar::VarString constSQL
        ( 
            "INSERT INTO tendercustomercalc "
            "( "
            "tenderNo, "
            "customerNo, "
            "articleno, "
            "pricedifference, "
            "transferredflag "
            ") "
            "VALUES "
            "( "
                + properties::TENDER_NO.toSQLReplacementString()           +", "
                + properties::CUSTOMER_NO.toSQLReplacementString()         +", "
                + properties::ARTICLE_NO.toSQLReplacementString()          +", "
                + properties::PRICE_DIFFERENCE.toSQLReplacementString()    +", "
                + properties::TRANSFERED_FLAG.toSQLReplacementString()     +" "
            ") "
         );
    resolve( constSQL );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}


} //end namespace omgcalc
} //end namespace accessor
} //end namespace infrastructure
}
