#include "omgcustomercalcdm.h"
#include <domainmodule/libtender_properties_definitions.h>

#include <loggerpool/loggerpool.h>
#include "infrastructure/omgcustomercalc/omgcustomercalcacc_definitions.h"

#include <libutil/searchyiterator.h>
#include <libutil/exception.h>
#include <libutil/util.h>
#include <libutil/accessor.h>

#include <libbasar_definitions.h>
#include <libbasardbaspect.h>
#include <libbasarcmnutil_exceptions.h>
#include <libbasardbaspect_exceptions.h>
#include <libbasardbsql_exceptions.h>

namespace tenderbatchprocessing
{
namespace domMod 
{
namespace omgcustomercalc 
{

using namespace basar::db::aspect;

const libutil::misc::ClassInfo& OMGCustomerCalcDM::getClassInfo()
{
    static const libutil::misc::ClassInfo info( "OMGCustomerCalcDM", libutil::misc::Version( 1, 0, 0, 0 ) );
    return info;
}

//OMGCalcDM::OMGCalcDM( const basar::db::aspect::AccessorPropertyTable_YIterator yit )
//: m_Yit( yit ),
OMGCustomerCalcDM::OMGCustomerCalcDM()
:m_Logger( tenderbatchprocessing::LoggerPool::getLoggerDomModules() )
{
    METHODNAME_DEF( OMGCustomerCalcDM, OMGCustomerCalcDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

}

OMGCustomerCalcDM::~OMGCustomerCalcDM()
{
    METHODNAME_DEF( OMGCustomerCalcDM, OMGCustomerCalcDM )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

bool OMGCustomerCalcDM::isEmpty() const
{
    METHODNAME_DEF( OMGCustomerCalcDM, isEmpty )
    BLOG_TRACE_METHOD( m_Logger, fun );

    if( 0 == getOMGCalcAccessor().get() 
        || m_Accessor->getPropertyTable().isNull() 
        || m_Accessor->getPropertyTable().isEmpty() )
    {
        return true;
    }
    return false;
}

void OMGCustomerCalcDM::resetSearchYit()
{
    METHODNAME_DEF( OMGCustomerCalcDM, resetSearchYit )
    BLOG_TRACE_METHOD(m_Logger, fun );

    getSearchYIterator()->reset();
}

basar::db::aspect::AccessorPropertyTable_YIterator OMGCustomerCalcDM::getSearchYit() const
{
    METHODNAME_DEF( OMGCustomerCalcDM, getSearchYit )
    BLOG_TRACE_METHOD( m_Logger, fun );

    return getSearchYIterator()->get();
}

basar::db::aspect::AccessorPropertyTableRef OMGCustomerCalcDM::get() const
{
    METHODNAME_DEF( OMGCustomerCalcDM, get )
    BLOG_TRACE_METHOD(m_Logger, fun );

    return m_Accessor->getPropertyTable();
}

void OMGCustomerCalcDM::injectOMGCalcAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
{
    METHODNAME_DEF( OMGCustomerCalcDM, injectOMGCalcAccessor )
    BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

    m_Accessor = accessor;
}

libutil::infrastructure::accessor::IAccessorPtr OMGCustomerCalcDM::getOMGCalcAccessor() const
{
    CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
    return m_Accessor;
}

libutil::domMod::SearchYIteratorPtr OMGCustomerCalcDM::getSearchYIterator() const
{
    METHODNAME_DEF( OMGCustomerCalcDM, getSearchYIterator )
    BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

    if( NULL == m_SearchYIterator.get() )
    {
        m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
                        new libutil::domMod::SearchYIterator( properties::TENDER_NO.toDescriptionString() +
                                                              properties::CUSTOMER_NO.toDescriptionString() +
                                                              properties::ARTICLE_NO.toDescriptionString() 
                                                            )
                    );
    }

    return m_SearchYIterator;
}

void OMGCustomerCalcDM::save( basar::db::aspect::AccessorPropertyTable_YIterator yitSave )
{
    METHODNAME_DEF( OMGCustomerCalcDM, save )
    BLOG_TRACE_METHOD( m_Logger, fun );

    using tenderbatchprocessing::infrastructure::accessor::omgcustomercalc::lit::SAVE_OMG_CUSTOMER_CALC;

    basar::db::aspect::ExecuteResultInfo executeResultInfo;
    try
    {
        executeResultInfo = m_Accessor->execute( SAVE_OMG_CUSTOMER_CALC, yitSave );
    }
    catch( basar::Exception& e )
    {
        libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SAVE_OMG_CUSTOMER_CALC, __FILE__, __LINE__ );
    }
    if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_OMG_CUSTOMER_CALC, executeResultInfo )
    }
}

void OMGCustomerCalcDM::findByID( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch)
{
    METHODNAME_DEF( OMGCustomerCalcDM, findByID )
    BLOG_TRACE_METHOD( m_Logger, fun );

    using tenderbatchprocessing::infrastructure::accessor::omgcustomercalc::lit::SELECT_OMG_CUSTOMER_CALC_BY_ID;
    getSearchYIterator()->reset();

    basar::db::aspect::ExecuteResultInfo executeResultInfo;
    try
    {
        m_Accessor->getPropertyTable().clear();
        executeResultInfo = m_Accessor->execute( SELECT_OMG_CUSTOMER_CALC_BY_ID, yitSearch );
    }
    catch( basar::Exception & e  )
    {
        libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_OMG_CUSTOMER_CALC_BY_ID, __FILE__, __LINE__ );
        throw;
    }

    if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_OMG_CUSTOMER_CALC_BY_ID, executeResultInfo )
    }
}

void OMGCustomerCalcDM::findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch)
{
    METHODNAME_DEF( OMGCustomerCalcDM, findByPattern )
    BLOG_TRACE_METHOD( m_Logger, fun );

    using tenderbatchprocessing::infrastructure::accessor::omgcustomercalc::lit::SELECT_OMG_CUSTOMER_CALC_BY_PATTERN;

    getSearchYIterator()->reset();
    basar::db::aspect::ExecuteResultInfo executeResultInfo;
    try
    {
        m_Accessor->getPropertyTable().clear();
        executeResultInfo = m_Accessor->execute( SELECT_OMG_CUSTOMER_CALC_BY_PATTERN, yitSearch );
    }
    catch( basar::Exception & e  )
    {
        libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_OMG_CUSTOMER_CALC_BY_PATTERN, __FILE__, __LINE__ );
        throw;
    }

    if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_OMG_CUSTOMER_CALC_BY_PATTERN, executeResultInfo )
    }
}


} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

//-----------------------------------------------------------------------------------------