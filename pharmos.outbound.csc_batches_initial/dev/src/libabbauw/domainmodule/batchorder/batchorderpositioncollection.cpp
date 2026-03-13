#include "batchorderpositioncollection.h"

#include "infrastructure/accessor/batchorder/batchorderpositioncollectionacc_definitions.h"
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
    const libutil::misc::ClassInfo& BatchOrderPositionCollection::getClassInfo()
    {
        static const libutil::misc::ClassInfo info( "BatchOrderPositionCollection", libutil::misc::Version( 1, 0, 0, 0 ) );
        return info;
    }

   BatchOrderPositionCollection::BatchOrderPositionCollection()
       : m_Logger( LoggerPool::loggerLibAbbaUW )
    {
        METHODNAME_DEF( BatchOrderPositionCollection, BatchOrderPositionCollection )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    BatchOrderPositionCollection::~BatchOrderPositionCollection()
    {
        METHODNAME_DEF( BatchOrderPositionCollection, ~BatchOrderPositionCollection )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    void BatchOrderPositionCollection::injectPositionCollectionAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
    {
        m_Accessor = accessor;
    }

    libutil::infrastructure::accessor::IAccessorPtr BatchOrderPositionCollection::getAccessor() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
        return m_Accessor;
    }

    basar::db::aspect::AccessorPropertyTableRef BatchOrderPositionCollection::get() const
    {
        return getAccessor()->getPropertyTable();
    }

    bool BatchOrderPositionCollection::isEmpty() const
    {
        return 0 == getAccessor().get() || m_Accessor->getPropertyTable().isNull() || m_Accessor->getPropertyTable().isEmpty();
    }

    void BatchOrderPositionCollection::resetSearchYit()
    {
        getSearchYIterator()->reset();
    }

    basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderPositionCollection::getSearchYit()
    {
        return getSearchYIterator()->get();
    }

    void BatchOrderPositionCollection::findByKey( basar::db::aspect::AccessorPropertyTable_YIterator searchYit )
    {
        METHODNAME_DEF( BatchOrderPositionCollection, findByKey )
        BLOG_TRACE_METHOD( getLogger(), fun );

        using infrastructure::accessor::batchOrder::lit::SELECT_BATCH_ORDER_POSITION_COLLECTION_BY_KEY;

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
        try
        {
            executeResultInfo = getAccessor()->execute( SELECT_BATCH_ORDER_POSITION_COLLECTION_BY_KEY, searchYit );
        }
        catch( basar::Exception & e  )
        {
            libutil::exceptions::ExceptionHandler::processAccessorException( e, getLogger(), fun, SELECT_BATCH_ORDER_POSITION_COLLECTION_BY_KEY, 
                                                                             __FILE__, __LINE__ );
            throw;
        }

        if( executeResultInfo.hasError() )
        {
            THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_BATCH_ORDER_POSITION_COLLECTION_BY_KEY, executeResultInfo )
        }
    }

    libutil::domMod::SearchYIteratorPtr BatchOrderPositionCollection::getSearchYIterator()
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

    const log4cplus::Logger& BatchOrderPositionCollection::getLogger() const
    {
        return m_Logger;
    }
} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw
