#include "omgtransferacc.h"
#include "omgtransferacc_definitions.h"
#include <loggerpool/loggerpool.h>


namespace tenderbatchprocessing
{
namespace infrastructure 
{
namespace accessor 
{
namespace omgtransfer 
{

BEGIN_ACCESSOR_DEFINITION( lit::OMGTRANSFER_ACC )

    BEGIN_PROPERTY_DESCRIPTION_LIST
    //PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO );
    END_PROPERTY_DESCRIPTION_LIST


    ACCESS_METHOD( lit::SELECT_OMG_TRANSFER_BY_ID );
    SQL_BUILDER_CREATE   ( SelectOMGTransferById )
    SQL_BUILDER_PUSH_BACK( SelectOMGTransferById )

    ACCESS_METHOD( lit::SELECT_OMG_TRANSFER_BY_PATTERN );
    SQL_BUILDER_CREATE   ( SelectOMGTransferByPattern )
    SQL_BUILDER_PUSH_BACK( SelectOMGTransferByPattern )

    ACCESS_METHOD( lit::SAVE_OMG_TRANSFER )
    SQL_BUILDER_CREATE( UpdateOMGTransfer )
    SQL_BUILDER_PUSH_BACK( UpdateOMGTransfer )
    SQL_BUILDER_CREATE( InsertOMGTransfer )
    SQL_BUILDER_PUSH_BACK( InsertOMGTransfer )

END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectOMGTransferById )

bool SelectOMGTransferById::isExecutable() const
{
    basar::cmnutil::ParameterList searchValues;
    //searchValues.push_back( properties::KSC_FIL.getName() );
    return arePropertiesSet( searchValues );
}

void SelectOMGTransferById::buildSQLString()
{
    static const basar::VarString sql( "" );
        resolve( sql );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( SelectOMGTransferByPattern )

bool SelectOMGTransferByPattern::isExecutable() const
{
    basar::cmnutil::ParameterList searchValues;
    //searchValues.push_back( properties::KSC_FIL.getName() );
    return arePropertiesSet( searchValues );
}

void SelectOMGTransferByPattern::buildSQLString()
{
    static const basar::VarString sql( "" );
        resolve( sql );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( UpdateOMGTransfer )
bool UpdateOMGTransfer::isExecutable() const
{
    const bool executable = arePropertiesSet( "properties::BRANCH_NO.toPropertyString()" ) ;
    return executable;
}

void UpdateOMGTransfer::buildSQLString()
{
    //using namespace properties;
    static const basar::VarString constSQL( "UPDATE " );
    resolve( constSQL );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( InsertOMGTransfer )
bool InsertOMGTransfer::isExecutable() const
{
    const bool executable = arePropertiesSetAndAllInState( "properties::BRANCH_NO.toPropertyString()", basar::SS_INSERT ) ;
    return executable;
}

void InsertOMGTransfer::buildSQLString()
{
    //using namespace properties;
    static const basar::VarString constSQL( "INSERT " );
    resolve( constSQL );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

} //end namespace omgtransfer
} //end namespace accessor
} //end namespace infrastructure
}