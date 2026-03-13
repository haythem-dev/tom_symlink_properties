//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "orderproposalcollectionprotectionproxy.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

#include <libutil/exception.h>
#include <libutil/util.h>

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasardbaspect_accessorpropertytable.h>

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
    const libutil::misc::ClassInfo & OrderProposalCollectionProtectionProxy::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "OrderProposalCollectionProtectionProxy", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

    OrderProposalCollectionProtectionProxy::OrderProposalCollectionProtectionProxy()
		: m_Logger( libabbauw::LoggerPool::loggerOrderProposal )
	{
        METHODNAME_DEF( OrderProposalCollectionProtectionProxy, OrderProposalCollectionProtectionProxy )
        BLOG_TRACE_METHOD(getLogger(), fun );
	}

	OrderProposalCollectionProtectionProxy::~OrderProposalCollectionProtectionProxy()
	{
        METHODNAME_DEF( OrderProposalCollectionProtectionProxy, ~OrderProposalCollectionProtectionProxy )
        BLOG_TRACE_METHOD(getLogger(), fun );
	}

    const log4cplus::Logger& OrderProposalCollectionProtectionProxy::getLogger() const
    {
        return m_Logger;
    }

    void OrderProposalCollectionProtectionProxy::injectOrderProposalCollection( IOrderProposalCollectionPtr orderProposalCollection )
    {
        m_OrderProposalCollection = orderProposalCollection;
    }

    IOrderProposalCollectionPtr OrderProposalCollectionProtectionProxy::getOrderProposalCollection() const
    {
        CHECK_INSTANCE_EXCEPTION( m_OrderProposalCollection.get() );
        return m_OrderProposalCollection;
    }

    void OrderProposalCollectionProtectionProxy::resetSearchYit()
    {
        getOrderProposalCollection()->resetSearchYit();
    }

    basar::db::aspect::AccessorPropertyTable_YIterator	OrderProposalCollectionProtectionProxy::getSearchYit()
    {
        return getOrderProposalCollection()->getSearchYit();
    }

    bool OrderProposalCollectionProtectionProxy::hasOrder( basar::Int32 orderNo )
    {
        for( basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin(); ! yit.isEnd(); ++yit )
        {
            if( yit.getInt32( properties::ORDERNO ) == orderNo )
            {
                return true;
            }
        }

        return false;
    }

    void OrderProposalCollectionProtectionProxy::releaseLock( basar::Int32 orderNo )
    {
        METHODNAME_DEF( OrderProposalCollectionProtectionProxy, releaseLock );
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_LockGuardCollection.erase( orderNo );
    }

    basar::db::aspect::AccessorPropertyTableRef OrderProposalCollectionProtectionProxy::get()
	{
		return getOrderProposalCollection()->get();
	}
    
    basar::db::aspect::AccessorPropertyTableRef OrderProposalCollectionProtectionProxy::get() const
	{
		return getOrderProposalCollection()->get();
	}

    void OrderProposalCollectionProtectionProxy::findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
	{
        METHODNAME_DEF( OrderProposalCollectionProtectionProxy, findByKey )
        BLOG_TRACE_METHOD( getLogger(), fun );

        getOrderProposalCollection()->findByKey( yitSearch );
	}

    basar::db::aspect::AccessorPropertyTable_YIterator OrderProposalCollectionProtectionProxy::addNewItem()
    {
        METHODNAME_DEF( OrderProposalCollectionProtectionProxy, addNewItem )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return getOrderProposalCollection()->addNewItem();
    }

    void OrderProposalCollectionProtectionProxy::removeItem( basar::db::aspect::AccessorPropertyTable_YIterator orderPropYit )
    {
        METHODNAME_DEF( OrderProposalCollectionProtectionProxy, removeItem )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if( orderPropYit.isEnd() )
        {
            return;
        }
        const basar::Int32 orderNo = orderPropYit.getInt32( properties::ORDERNO );

        // remove order lock if this item was the last one for this order
        getOrderProposalCollection()->removeItem( orderPropYit );
        if( ! hasOrder( orderNo ) )
        {
            releaseLock( orderNo );
        }
    }

    void OrderProposalCollectionProtectionProxy::save()
    {
        METHODNAME_DEF( OrderProposalCollectionProtectionProxy, save )
        BLOG_TRACE_METHOD( getLogger(), fun );

        getOrderProposalCollection()->save();
    }

    void OrderProposalCollectionProtectionProxy::passLockGuard( basar::Int32 orderNo, libLockManager::ILockGuardPtr lockGuard )
    {
        if( m_LockGuardCollection.find( orderNo ) == m_LockGuardCollection.end() )
        {
            m_LockGuardCollection[ orderNo ] = lockGuard;
        }
    }
} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw
