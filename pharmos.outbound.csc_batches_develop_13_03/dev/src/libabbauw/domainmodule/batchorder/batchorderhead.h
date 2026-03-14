#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERHEAD_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERHEAD_H

#include "ibatchorderheadfinder.h"

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>
#include <libutil/util.h>

namespace libabbauw
{
namespace domMod
{
namespace batchOrder
{

    class BatchOrderHead : public IBatchOrderHeadFinder
    {
    public:
        static const libutil::misc::ClassInfo& getClassInfo();

    public:
        BatchOrderHead();
        ~BatchOrderHead();

        void                                                        injectBatchOrderHeadAccessor( libutil::infrastructure::accessor::IAccessorPtr );

        // from IBatchOrderCollectionFinder interface
        basar::db::aspect::AccessorPropertyTable_YIterator          get()     const;
        bool                                                        isEmpty() const;

        void                                                        resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();

        void                                                        findByKey( basar::db::aspect::AccessorPropertyTable_YIterator );

    private:
        // forbidden
        BatchOrderHead( const BatchOrderHead& );
        BatchOrderHead& operator = ( const BatchOrderHead& );

        libutil::infrastructure::accessor::IAccessorPtr             getAccessor() const;
        libutil::domMod::SearchYIteratorPtr                         getSearchYIterator();

        const log4cplus::Logger&                                    getLogger() const;

    private:
        const log4cplus::Logger&                                    m_Logger;
        libutil::domMod::SearchYIteratorPtr                         m_SearchYIterator;
        libutil::infrastructure::accessor::IAccessorPtr             m_Accessor;
    };

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERHEAD_H
