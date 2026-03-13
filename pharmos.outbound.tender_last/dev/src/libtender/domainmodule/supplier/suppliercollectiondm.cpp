#include "suppliercollectiondm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/supplier/suppliercollectionacc_definitions.h>

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

namespace libtender
{
namespace domMod
{
namespace supplier
{
    using basar::db::aspect::AccessorPropertyTable_YIterator;
    using basar::db::aspect::AccessorPropertyTableRef;
}
}
}

namespace libtender
{
namespace domMod
{
namespace supplier
{
    const libutil::misc::ClassInfo& SupplierCollectionDM::getClassInfo()
    {
        static const libutil::misc::ClassInfo info( "SupplierCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );
        return info;
    }

    SupplierCollectionDM::SupplierCollectionDM()
    : m_Logger( libtender::LoggerPool::getLoggerDomModules() )
    {
        METHODNAME_DEF( SupplierCollectionDM, SupplierCollectionDM )
        BLOG_TRACE_METHOD( getLogger(), fun );

    }

    SupplierCollectionDM::~SupplierCollectionDM()
    {
        METHODNAME_DEF( SupplierCollectionDM, ~SupplierCollectionDM )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    void SupplierCollectionDM::injectSupplierAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor)
    {
        METHODNAME_DEF( SupplierCollectionDM, injectSupplierAccessor )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_Accessor = accessor;
    }

    const log4cplus::Logger& SupplierCollectionDM::getLogger() const
    {
        METHODNAME_DEF( SupplierCollectionDM, getLogger )
        BLOG_TRACE_METHOD( m_Logger, fun );

        return m_Logger;
    }

    void SupplierCollectionDM::resetSearchYit()
    {
        METHODNAME_DEF( SupplierCollectionDM, resetSearchYit )
        BLOG_TRACE_METHOD( m_Logger, fun );

        getSearchYIterator()->reset();
    }

    AccessorPropertyTable_YIterator SupplierCollectionDM::getSearchYit() const
    {
        METHODNAME_DEF( SupplierCollectionDM, getSearchYit )
        BLOG_TRACE_METHOD( m_Logger, fun );

        return getSearchYIterator()->get();
    }

    void SupplierCollectionDM::setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        METHODNAME_DEF( SupplierCollectionDM, setSearchYit )
        BLOG_TRACE_METHOD( m_Logger, fun );

        getSearchYit() = yit;
    }

    libutil::domMod::SearchYIteratorPtr SupplierCollectionDM::getSearchYIterator() const
    {
        METHODNAME_DEF( SupplierCollectionDM, getSearchYIterator )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if( NULL == m_SearchYIterator.get() )
        {
            m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
            new libutil::domMod::SearchYIterator( properties::SUPPLIER_NO.toDescriptionString() +
                                                  properties::SUPPLIER_NAME.toDescriptionString()
                                                 ));
        }
    
        return m_SearchYIterator;
    }

    libutil::infrastructure::accessor::IAccessorPtr SupplierCollectionDM::getSupplierCollectionAccessor() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );

        return m_Accessor;
    }

    void SupplierCollectionDM::findByPattern( AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( SupplierCollectionDM, findByPattern )
        BLOG_TRACE_METHOD( getLogger(), fun );

        using libtender::infrastructure::accessor::supplier::lit::SELECT_SUPPLIER_BY_PATTERN;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;

        try
        {
            executeResultInfo = m_Accessor->execute( SELECT_SUPPLIER_BY_PATTERN, yitSearch );
        }
        catch( basar::Exception & e )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_SUPPLIER_BY_PATTERN, __FILE__, __LINE__ );
            throw;
        }
        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_SUPPLIER_BY_PATTERN, executeResultInfo )
        }

    }

    void SupplierCollectionDM::findBySupplierNo( AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( SupplierCollectionDM, findBySupplierNo )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if( !yitSearch.isContainedAndSet( properties::SUPPLIER_NO ) )
            return;

        using libtender::infrastructure::accessor::supplier::lit::SELECT_SUPPLIER_BY_SUPPLIER_NO;
        
        basar::db::aspect::ExecuteResultInfo executeResultInfo;

        try
        {
            executeResultInfo = m_Accessor->execute(SELECT_SUPPLIER_BY_SUPPLIER_NO, yitSearch );
        }
        catch( basar::Exception & e )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_SUPPLIER_BY_SUPPLIER_NO, __FILE__, __LINE__ );
            throw;
        }
        
        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_SUPPLIER_BY_SUPPLIER_NO, executeResultInfo )
        }
    }

    basar::db::aspect::AccessorPropertyTableRef SupplierCollectionDM::get() const
    {
        METHODNAME_DEF( SupplierCollectionDM, get )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return m_Accessor->getPropertyTable();
    }

    bool SupplierCollectionDM::isEmpty() const
    {
        METHODNAME_DEF( SupplierCollectionDM, isEmpty )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if ( 0 == getSupplierCollectionAccessor().get() || 
            m_Accessor->getPropertyTable().isNull()   ||
             m_Accessor->getPropertyTable().isEmpty() )
        {
            return true;
        }

        return false;
    }

} // end namespace supplier
} // end namespace domMod
} // end namespace libtender
