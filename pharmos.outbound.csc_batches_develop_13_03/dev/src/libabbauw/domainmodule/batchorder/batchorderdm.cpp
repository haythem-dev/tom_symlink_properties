#include "batchorderdm.h"

#include "ibatchorderheadfinder.h"
#include "ibatchorderpositioncollectionfinder.h"
#include "icscorderreleaser.h"

#include "libabbauw_properties_definitions.h"
#include "loggerpool/libabbauw_loggerpool.h"

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

#include <boost/make_shared.hpp>

namespace libabbauw
{
namespace domMod
{
namespace batchOrder
{
    const libutil::misc::ClassInfo& BatchOrderDM::getClassInfo()
    {
        static const libutil::misc::ClassInfo info( "BatchOrderDM", libutil::misc::Version( 1, 0, 0, 0 ) );
        return info;
    }

   BatchOrderDM::BatchOrderDM()
       : m_Logger( LoggerPool::loggerLibAbbaUW )
    {
        METHODNAME_DEF( BatchOrderDM, BatchOrderDM )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    BatchOrderDM::~BatchOrderDM()
    {
        METHODNAME_DEF( BatchOrderDM, ~BatchOrderDM )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    void BatchOrderDM::injectBatchOrderHeadFinder( IBatchOrderHeadFinderPtr batchOrderHeadFinder )
    {
        m_BatchOrderHeadFinder = batchOrderHeadFinder;
    }

    IBatchOrderHeadFinderPtr BatchOrderDM::getBatchOrderHeadFinder()
    {
        CHECK_INSTANCE_EXCEPTION( m_BatchOrderHeadFinder.get() );
        return m_BatchOrderHeadFinder;
    }

    void BatchOrderDM::injectBatchOrderPositionCollectionFinder( IBatchOrderPositionCollectionFinderPtr batchOrderPositionCollectionFinder )
    {
        m_BatchOrderPositionCollectionFinder = batchOrderPositionCollectionFinder;
    }

    IBatchOrderPositionCollectionFinderPtr BatchOrderDM::getBatchOrderPositionCollectionFinder()
    {
        CHECK_INSTANCE_EXCEPTION( m_BatchOrderPositionCollectionFinder.get() );
        return m_BatchOrderPositionCollectionFinder;
    }

    void BatchOrderDM::injectCscOrderReleaser( cscorder::ICSCOrderReleaserPtr orderReleaser )
    {
        m_CscOrderReleaser = orderReleaser;
    }

    cscorder::ICSCOrderReleaserPtr BatchOrderDM::getCSCOrderReleaser()
    {
        CHECK_INSTANCE_EXCEPTION( m_CscOrderReleaser.get() );
        return m_CscOrderReleaser;
    }

    basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderDM::getHead()
    {
        return getBatchOrderHeadFinder()->get();
    }

    basar::db::aspect::AccessorPropertyTableRef BatchOrderDM::getPositionCollection()
    {
        return getBatchOrderPositionCollectionFinder()->get();
    }

    bool BatchOrderDM::isEmpty() const
    {
        return m_BatchOrderHeadFinder->isEmpty();
    }

    void BatchOrderDM::resetSearchYit()
    {
        getBatchOrderHeadFinder()->resetSearchYit();
    }

    basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderDM::getSearchYit()
    {
        return getBatchOrderHeadFinder()->getSearchYit();
    }

    void BatchOrderDM::findByKey( basar::db::aspect::AccessorPropertyTable_YIterator searchYit )
    {
        METHODNAME_DEF( BatchOrderDM, findByKey )
        BLOG_TRACE_METHOD( getLogger(), fun );

        getBatchOrderHeadFinder()->findByKey( searchYit );
        getBatchOrderPositionCollectionFinder()->findByKey( searchYit );
        getBatchOrderHeadFinder()->get().setDecimal( properties::ORDERVALUE, getOrderValue() );
    }

    void BatchOrderDM::release()
    {
        METHODNAME_DEF( BatchOrderDM, release )
        BLOG_TRACE_METHOD( getLogger(), fun );

        getCSCOrderReleaser()->release( false );
    }

    basar::Decimal BatchOrderDM::getOrderValue() const
    {
        basar::Decimal value;
        for( basar::db::aspect::AccessorPropertyTable_YIterator yit = m_BatchOrderPositionCollectionFinder->get().begin(); ! yit.isEnd(); ++yit )
        {
            value += ( yit.getDecimal( properties::ARTICLEPRICE ) * yit.getInt32( properties::ORDEREDQTY ) );
        }

        return value;
    }

    const log4cplus::Logger& BatchOrderDM::getLogger() const
    {
        return m_Logger;
    }
} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw
