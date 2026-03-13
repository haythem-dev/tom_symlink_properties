#include "batchorderhead.h"

#include "infrastructure/accessor/batchorder/batchorderheadacc_definitions.h"
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
    const libutil::misc::ClassInfo& BatchOrderHead::getClassInfo()
    {
        static const libutil::misc::ClassInfo info( "BatchOrderHead", libutil::misc::Version( 1, 0, 0, 0 ) );
        return info;
    }

   BatchOrderHead::BatchOrderHead()
       : m_Logger( LoggerPool::loggerLibAbbaUW )
    {
        METHODNAME_DEF( BatchOrderHead, BatchOrderHead )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    BatchOrderHead::~BatchOrderHead()
    {
        METHODNAME_DEF( BatchOrderHead, ~BatchOrderHead )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    void BatchOrderHead::injectBatchOrderHeadAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
    {
        m_Accessor = accessor;
    }

    libutil::infrastructure::accessor::IAccessorPtr BatchOrderHead::getAccessor() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
        return m_Accessor;
    }

    basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderHead::get() const
    {
        return getAccessor()->getPropertyTable().begin();
    }

    bool BatchOrderHead::isEmpty() const
    {
        return 0 == getAccessor().get() || m_Accessor->getPropertyTable().isNull() || m_Accessor->getPropertyTable().isEmpty();
    }

    void BatchOrderHead::resetSearchYit()
    {
        getSearchYIterator()->reset();
    }

    basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderHead::getSearchYit()
    {
        return getSearchYIterator()->get();
    }

    void BatchOrderHead::findByKey( basar::db::aspect::AccessorPropertyTable_YIterator searchYit )
    {
        METHODNAME_DEF( BatchOrderHead, findByKey )
        BLOG_TRACE_METHOD( getLogger(), fun );

        using infrastructure::accessor::batchOrder::lit::SELECT_BATCH_ORDER_HEAD_BY_KEY;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            executeResultInfo = getAccessor()->execute( SELECT_BATCH_ORDER_HEAD_BY_KEY, searchYit );
        }
        catch( basar::Exception & e  )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, getLogger(), fun, SELECT_BATCH_ORDER_HEAD_BY_KEY, 
                                                                             __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_BATCH_ORDER_HEAD_BY_KEY, executeResultInfo )
        }
    }

    libutil::domMod::SearchYIteratorPtr BatchOrderHead::getSearchYIterator()
    {
        if( m_SearchYIterator.get() == NULL )
        {
            m_SearchYIterator = boost::make_shared< libutil::domMod::SearchYIterator >(
                                                        properties::BRANCHNO.toDescriptionString()        +
                                                        properties::ORDERNO.toDescriptionString()
            );
        }

        return m_SearchYIterator;
    }

    const log4cplus::Logger& BatchOrderHead::getLogger() const
    {
        return m_Logger;
    }
} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw
