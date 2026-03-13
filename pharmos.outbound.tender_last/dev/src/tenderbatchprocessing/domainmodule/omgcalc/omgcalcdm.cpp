#include "omgcalcdm.h"
#include "tenderbatchprocessing_property_definitions.h"

#include <loggerpool/loggerpool.h>
#include "infrastructure/omgcalc/omgcalcacc_definitions.h"

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
namespace omgcalc 
{

using namespace basar::db::aspect;

const libutil::misc::ClassInfo& OMGCalcDM::getClassInfo()
{
    static const libutil::misc::ClassInfo info( "OMGCalcDM", libutil::misc::Version( 1, 0, 0, 0 ) );
    return info;
}

//OMGCalcDM::OMGCalcDM( const basar::db::aspect::AccessorPropertyTable_YIterator yit )
//: m_Yit( yit ),
OMGCalcDM::OMGCalcDM()
:m_Logger( tenderbatchprocessing::LoggerPool::getLoggerDomModules() )
{
    METHODNAME_DEF( OMGCalcDM, OMGCalcDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

}

OMGCalcDM::~OMGCalcDM()
{
    METHODNAME_DEF( OMGCalcDM, OMGCalcDM )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

bool OMGCalcDM::isEmpty() const
{
    METHODNAME_DEF( OMGCalcDM, isEmpty )
    BLOG_TRACE_METHOD( m_Logger, fun );

    if( 0 == getOMGCalcAccessor().get() 
        || m_Accessor->getPropertyTable().isNull() 
        || m_Accessor->getPropertyTable().isEmpty() )
    {
        return true;
    }
    return false;
}

void OMGCalcDM::resetSearchYit()
{
    METHODNAME_DEF( OMGCalcDM, resetSearchYit )
    BLOG_TRACE_METHOD(m_Logger, fun );

    getSearchYIterator()->reset();
}

basar::db::aspect::AccessorPropertyTable_YIterator OMGCalcDM::getSearchYit() const
{
    METHODNAME_DEF( OMGCalcDM, getSearchYit )
    BLOG_TRACE_METHOD( m_Logger, fun );

    return getSearchYIterator()->get();
}

basar::db::aspect::AccessorPropertyTableRef OMGCalcDM::get() const
{
    METHODNAME_DEF( OMGCalcDM, get )
    BLOG_TRACE_METHOD(m_Logger, fun );

    return m_Accessor->getPropertyTable();
}

void OMGCalcDM::injectOMGCalcAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
{
    METHODNAME_DEF( OMGCalcDM, injectOMGCalcAccessor )
    BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

    m_Accessor = accessor;
}

libutil::infrastructure::accessor::IAccessorPtr OMGCalcDM::getOMGCalcAccessor() const
{
    CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
    return m_Accessor;
}

libutil::domMod::SearchYIteratorPtr OMGCalcDM::getSearchYIterator() const
{
    METHODNAME_DEF( OMGCalcDM, getSearchYIterator )
    BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

    if( NULL == m_SearchYIterator.get() )
    {
        m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
            new libutil::domMod::SearchYIterator( "") );
    }

    return m_SearchYIterator;
}

void OMGCalcDM::save()
{
    METHODNAME_DEF( OMGCalcDM, save )
    BLOG_TRACE_METHOD( m_Logger, fun );

    using tenderbatchprocessing::infrastructure::accessor::omgcalc::lit::SAVE_OMG_CALC;

    basar::db::aspect::ExecuteResultInfo executeResultInfo;
    try
    {
        //executeResultInfo = m_Accessor->execute( infrastructure::accessor::omgcalc::lit::SAVE_OMG_CALC, m_Yit );
        executeResultInfo = m_Accessor->execute( infrastructure::accessor::omgcalc::lit::SAVE_OMG_CALC );
    }
    catch( basar::Exception& e )
    {
        libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SAVE_OMG_CALC, __FILE__, __LINE__ );
    }
    if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_OMG_CALC, executeResultInfo )
    }
}

void OMGCalcDM::findByID( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch)
{
    METHODNAME_DEF( OMGCalcDM, findByID )
    BLOG_TRACE_METHOD( m_Logger, fun );

    using tenderbatchprocessing::infrastructure::accessor::omgcalc::lit::SELECT_OMG_CALC_BY_ID;
    getSearchYIterator()->reset();

    basar::db::aspect::ExecuteResultInfo executeResultInfo;
    try
    {
        m_Accessor->getPropertyTable().clear();
        executeResultInfo = m_Accessor->execute( SELECT_OMG_CALC_BY_ID, yitSearch );
    }
    catch( basar::Exception & e  )
    {
        libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_OMG_CALC_BY_ID, __FILE__, __LINE__ );
        throw;
    }

    if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_OMG_CALC_BY_ID, executeResultInfo )
    }
}

void OMGCalcDM::findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch)
{
    METHODNAME_DEF( OMGCalcDM, findByPattern )
    BLOG_TRACE_METHOD( m_Logger, fun );

    using tenderbatchprocessing::infrastructure::accessor::omgcalc::lit::SELECT_OMG_CALC_BY_PATTERN;

    getSearchYIterator()->reset();
    basar::db::aspect::ExecuteResultInfo executeResultInfo;
    try
    {
        m_Accessor->getPropertyTable().clear();
        executeResultInfo = m_Accessor->execute( SELECT_OMG_CALC_BY_PATTERN, yitSearch );
    }
    catch( basar::Exception & e  )
    {
        libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_OMG_CALC_BY_PATTERN, __FILE__, __LINE__ );
        throw;
    }

    if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_OMG_CALC_BY_PATTERN, executeResultInfo )
    }
}


} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

//-----------------------------------------------------------------------------------------