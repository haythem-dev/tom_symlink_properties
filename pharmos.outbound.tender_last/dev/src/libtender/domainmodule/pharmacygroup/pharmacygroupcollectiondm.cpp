#include "pharmacygroupcollectiondm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/pharmacygroup/pharmacygroupcollectionacc_definitions.h>

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

namespace libtender
{
namespace domMod
{
namespace pharmacychain
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
namespace pharmacygroup
{
    const libutil::misc::ClassInfo& PharmacyGroupCollectionDM::getClassInfo()
    {
        static const libutil::misc::ClassInfo info( "PharmacyGroupCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );
        return info;
    }
    
    PharmacyGroupCollectionDM::PharmacyGroupCollectionDM()
    : m_Logger( libtender::LoggerPool::getLoggerDomModules() )
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, PharmacyGroupCollectionDM )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    PharmacyGroupCollectionDM::~PharmacyGroupCollectionDM()
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, ~PharmacyGroupCollectionDM )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    void PharmacyGroupCollectionDM::injectPharmacyGroupAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor)
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, ~injectPharmacyGroupAccessor )
        BLOG_TRACE_METHOD( getLogger(), fun );
        
        m_Accessor = accessor;
    }
    
    const log4cplus::Logger& PharmacyGroupCollectionDM::getLogger() const
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, getLogger )
        BLOG_TRACE_METHOD( m_Logger, fun );
        
        return m_Logger;
    }

    void PharmacyGroupCollectionDM::resetSearchYit()
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, resetSearchYit )
        BLOG_TRACE_METHOD( m_Logger, fun );
        
        getSearchYIterator()->reset();
    }
    
     basar::db::aspect::AccessorPropertyTable_YIterator PharmacyGroupCollectionDM::getSearchYit() const
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, getSearchYit )
        BLOG_TRACE_METHOD( m_Logger, fun );
        
        return getSearchYIterator()->get();
    }
    
    void PharmacyGroupCollectionDM::setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, setSearchYit )
        BLOG_TRACE_METHOD( m_Logger, fun );
        
        getSearchYit() = yit;
    }

    libutil::domMod::SearchYIteratorPtr PharmacyGroupCollectionDM::getSearchYIterator() const
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, getSearchYIterator )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if( NULL == m_SearchYIterator.get() )
        {
            m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
                new libutil::domMod::SearchYIterator( properties::PHARMACY_GROUPID.toDescriptionString() +
                                                      properties::CUSTOMER_NO.toDescriptionString() +
                                                      properties::BRANCH_NO.toDescriptionString() +
                                                      properties::PHARMACY_GROUP_NAME.toDescriptionString()));
        }
        
        return m_SearchYIterator;
    }
    
    libutil::infrastructure::accessor::IAccessorPtr PharmacyGroupCollectionDM::getPharmacyGroupCollectionAccessor() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
        
        return m_Accessor;
    }

    void PharmacyGroupCollectionDM::findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, findByPattern )
        BLOG_TRACE_METHOD( getLogger(), fun );

        using libtender::infrastructure::accessor::pharmacygroup::lit::SELECT_PHARMACY_GROUP_BY_PATTERN;
        
        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        
        try
        {
            executeResultInfo = m_Accessor->execute( SELECT_PHARMACY_GROUP_BY_PATTERN, yitSearch );
        }
        catch( basar::Exception & e )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PHARMACY_GROUP_BY_PATTERN, __FILE__, __LINE__ );
            throw;
        }
        
        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PHARMACY_GROUP_BY_PATTERN, executeResultInfo )
        }
    }

    void PharmacyGroupCollectionDM::findPharmacyGroupCustomerAssignmentByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, findPharmacyGroupCustomerAssignmentByPattern )
        BLOG_TRACE_METHOD( getLogger(), fun );

        using libtender::infrastructure::accessor::pharmacygroup::lit::SELECT_PHARMACY_GROUP_CUSTOMERS_ASSIGNMENT_BY_PATTERN;
        
        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        
        try
        {
            executeResultInfo = m_Accessor->execute( SELECT_PHARMACY_GROUP_CUSTOMERS_ASSIGNMENT_BY_PATTERN, yitSearch );
        }
        catch( basar::Exception & e )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PHARMACY_GROUP_CUSTOMERS_ASSIGNMENT_BY_PATTERN, __FILE__, __LINE__ );
            throw;
        }
        
        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PHARMACY_GROUP_CUSTOMERS_ASSIGNMENT_BY_PATTERN, executeResultInfo )
        }
    }
    
    void PharmacyGroupCollectionDM::findByPharmacyGroupId(  basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, findByPharmacyGroupId )
        BLOG_TRACE_METHOD( getLogger(), fun );
        
        if( !yitSearch.isContainedAndSet( properties::PHARMACY_GROUPID ) )
            return;
        
        using libtender::infrastructure::accessor::pharmacygroup::lit::SELECT_PHARMACY_GROUP_BY_GROUP_ID;
        
        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        
        try
        {
        	executeResultInfo = m_Accessor->execute(SELECT_PHARMACY_GROUP_BY_GROUP_ID, yitSearch );
        }
        catch( basar::Exception & e )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PHARMACY_GROUP_BY_GROUP_ID, __FILE__, __LINE__ );
            throw;
        }
        
        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PHARMACY_GROUP_BY_GROUP_ID, executeResultInfo )
        }
    }

    void PharmacyGroupCollectionDM::findByPseudoCustomerNo(  basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, findByPseudoCustomerNo )
        BLOG_TRACE_METHOD( getLogger(), fun );
        
        if( !yitSearch.isContainedAndSet( properties::CUSTOMER_NO ) )
            return;
        using libtender::infrastructure::accessor::pharmacygroup::lit::SELECT_PHARMACY_GROUP_BY_PSEUDOCUSTOMER_NO;
        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        
        try
        {
            executeResultInfo = m_Accessor->execute(SELECT_PHARMACY_GROUP_BY_PSEUDOCUSTOMER_NO, yitSearch );
        }
        catch( basar::Exception & e )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_PHARMACY_GROUP_BY_PSEUDOCUSTOMER_NO, __FILE__, __LINE__ );
            throw;
        }
        
        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_PHARMACY_GROUP_BY_PSEUDOCUSTOMER_NO, executeResultInfo )
        }
    }

    basar::db::aspect::AccessorPropertyTableRef PharmacyGroupCollectionDM::get() const
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, get )
        BLOG_TRACE_METHOD( getLogger(), fun );
        
        return m_Accessor->getPropertyTable();
    }
    
    bool PharmacyGroupCollectionDM::isEmpty() const
    {
        METHODNAME_DEF( PharmacyGroupCollectionDM, isEmpty )
        BLOG_TRACE_METHOD( getLogger(), fun );
        
        if ( 0 == getPharmacyGroupCollectionAccessor().get() || 
             m_Accessor->getPropertyTable().isNull()   ||
             m_Accessor->getPropertyTable().isEmpty() )
        {
            return true;
        }
        
        return false;
    }

} // end namespace PharmacyChain
} // end namespace domMod
} // end namespace libtender
