#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERPOSITIONCOLLECTION_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERPOSITIONCOLLECTION_H

#include "ibatchorderpositioncollectionfinder.h"

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

    class BatchOrderPositionCollection : public IBatchOrderPositionCollectionFinder
    {
    public:
        static const libutil::misc::ClassInfo& getClassInfo();

    public:
        BatchOrderPositionCollection();
        ~BatchOrderPositionCollection();

        void                                                        injectPositionCollectionAccessor( libutil::infrastructure::accessor::IAccessorPtr );

        // from IBatchOrderPositionCollectionFinder interface
        basar::db::aspect::AccessorPropertyTableRef                 get()     const;
        bool                                                        isEmpty() const;

        void                                                        resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();

        void                                                        findByKey( basar::db::aspect::AccessorPropertyTable_YIterator );

    private:
        // forbidden
        BatchOrderPositionCollection( const BatchOrderPositionCollection& );
        BatchOrderPositionCollection& operator = ( const BatchOrderPositionCollection& );

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

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERPOSITIONCOLLECTION_H
