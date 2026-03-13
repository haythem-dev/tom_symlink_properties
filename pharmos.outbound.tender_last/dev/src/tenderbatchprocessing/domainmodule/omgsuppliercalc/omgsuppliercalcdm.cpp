#include "omgsuppliercalcdm.h"
#include <domainmodule/libtender_properties_definitions.h>
#include <tenderbatchprocessing_property_definitions.h>

#include <loggerpool/loggerpool.h>
#include "infrastructure/omgsuppliercalc/omgsuppliercalcacc_definitions.h"

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
namespace omgsuppliercalc 
{

using namespace basar::db::aspect;

const libutil::misc::ClassInfo& OMGSupplierCalcDM::getClassInfo()
{
    static const libutil::misc::ClassInfo info( "OMGSupplierCalcDM", libutil::misc::Version( 1, 0, 0, 0 ) );
    return info;
}

OMGSupplierCalcDM::OMGSupplierCalcDM()
:m_Logger( tenderbatchprocessing::LoggerPool::getLoggerDomModules() )
{
    METHODNAME_DEF( OMGSupplierCalcDM, OMGSupplierCalcDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

}

OMGSupplierCalcDM::~OMGSupplierCalcDM()
{
    METHODNAME_DEF( OMGSupplierCalcDM, OMGSupplierCalcDM )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

bool OMGSupplierCalcDM::isEmpty() const
{
    METHODNAME_DEF( OMGSupplierCalcDM, isEmpty )
    BLOG_TRACE_METHOD( m_Logger, fun );

    if( 0 == getOMGCalcAccessor().get() 
        || m_Accessor->getPropertyTable().isNull() 
        || m_Accessor->getPropertyTable().isEmpty() )
    {
        return true;
    }
    return false;
}

void OMGSupplierCalcDM::resetSearchYit()
{
    METHODNAME_DEF( OMGSupplierCalcDM, resetSearchYit )
    BLOG_TRACE_METHOD(m_Logger, fun );

    getSearchYIterator()->reset();
}

basar::db::aspect::AccessorPropertyTable_YIterator OMGSupplierCalcDM::getSearchYit() const
{
    METHODNAME_DEF( OMGSupplierCalcDM, getSearchYit )
    BLOG_TRACE_METHOD( m_Logger, fun );

    return getSearchYIterator()->get();
}

basar::db::aspect::AccessorPropertyTableRef OMGSupplierCalcDM::get() const
{
    METHODNAME_DEF( OMGSupplierCalcDM, get )
    BLOG_TRACE_METHOD(m_Logger, fun );

    return m_Accessor->getPropertyTable();
}

void OMGSupplierCalcDM::injectOMGCalcAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
{
    METHODNAME_DEF( OMGSupplierCalcDM, injectOMGCalcAccessor )
    BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

    m_Accessor = accessor;
}

libutil::infrastructure::accessor::IAccessorPtr OMGSupplierCalcDM::getOMGCalcAccessor() const
{
    CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
    return m_Accessor;
}

libutil::domMod::SearchYIteratorPtr OMGSupplierCalcDM::getSearchYIterator() const
{
    METHODNAME_DEF( OMGSupplierCalcDM, getSearchYIterator )
    BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

    if( NULL == m_SearchYIterator.get() )
    {
        m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
                        new libutil::domMod::SearchYIterator( properties::BRANCH_NO.toDescriptionString() +
                                                              properties::TENDER_NO.toDescriptionString() +
                                                              properties::ORDER_NO.toDescriptionString() +
                                                              properties::ORDER_DATE.toDescriptionString() +
                                                              properties::ARTICLE_NO.toDescriptionString() 
                                                            )
                    );
    }

    return m_SearchYIterator;
}

void OMGSupplierCalcDM::markAsTransferred( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd )
{
    METHODNAME_DEF( OMGSupplierCalcDM, markAsTransferred )
    BLOG_TRACE_METHOD( m_Logger, fun );

	for( AccessorPropertyTable_YIterator yit = yitStart; yit != yitEnd; ++yit )
	{
		using tenderbatchprocessing::infrastructure::accessor::omgsuppliercalc::lit::SAVE_OMG_SUPPLIER_CALC_TRANSFERRED;

		basar::db::aspect::ExecuteResultInfo executeResultInfo;
		try
		{
			executeResultInfo = m_Accessor->execute( SAVE_OMG_SUPPLIER_CALC_TRANSFERRED, yit );
		}
		catch( basar::Exception& e )
		{
			libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SAVE_OMG_SUPPLIER_CALC_TRANSFERRED, __FILE__, __LINE__ );
		}
		if( executeResultInfo.hasError() )
		{
			THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_OMG_SUPPLIER_CALC_TRANSFERRED, executeResultInfo )
		}
	}
}

void OMGSupplierCalcDM::save( basar::db::aspect::AccessorPropertyTable_YIterator yitSave )
{
    METHODNAME_DEF( OMGSupplierCalcDM, save )
    BLOG_TRACE_METHOD( m_Logger, fun );

    using tenderbatchprocessing::infrastructure::accessor::omgsuppliercalc::lit::SAVE_OMG_SUPPLIER_CALC;

    basar::db::aspect::ExecuteResultInfo executeResultInfo;
    try
    {
        executeResultInfo = m_Accessor->execute( SAVE_OMG_SUPPLIER_CALC, yitSave );
    }
    catch( basar::Exception& e )
    {
        libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SAVE_OMG_SUPPLIER_CALC, __FILE__, __LINE__ );
    }
    if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_OMG_SUPPLIER_CALC, executeResultInfo )
    }
}

void OMGSupplierCalcDM::findForTransfer()
{
    METHODNAME_DEF( OMGSupplierCalcDM, findForTransfer )
    BLOG_TRACE_METHOD( m_Logger, fun );

    using tenderbatchprocessing::infrastructure::accessor::omgsuppliercalc::lit::SELECT_OMG_CALC_FOR_TRANSFER;
    getSearchYIterator()->reset();

    basar::db::aspect::ExecuteResultInfo executeResultInfo;
    try
    {
        m_Accessor->getPropertyTable().clear();
        executeResultInfo = m_Accessor->execute( SELECT_OMG_CALC_FOR_TRANSFER, true, true );
    }
    catch( basar::Exception & e  )
    {
        libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_OMG_CALC_FOR_TRANSFER, __FILE__, __LINE__ );
        throw;
    }

    if( executeResultInfo.hasError() )
    {
        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_OMG_CALC_FOR_TRANSFER, executeResultInfo )
    }
}

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

