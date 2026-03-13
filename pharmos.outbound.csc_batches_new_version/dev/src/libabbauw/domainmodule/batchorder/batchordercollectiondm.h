#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERCOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERCOLLECTIONDM_H

#include "ibatchordercollectionfinder.h"
#include "ibatchorderptr.h"

#include "componentmanager/ibatchorderconnectedfactoryptr.h"

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

    class BatchOrderCollectionDM : public IBatchOrderCollectionFinder
    {
    public:
        static const libutil::misc::ClassInfo& getClassInfo();

    public:
        BatchOrderCollectionDM();
        ~BatchOrderCollectionDM();

        void                                                        injectBatchOrderCollectionAccessor( libutil::infrastructure::accessor::IAccessorPtr );
        void                                                        injectBatchOrderConnectedFactory( componentManager::IBatchOrderConnectedFactoryPtr );

        // from IBatchOrderCollectionFinder interface
        basar::db::aspect::AccessorPropertyTableRef                 get()     const;
        bool                                                        isEmpty() const;

        void                                                        resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();

        void                                                        findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator );
        IBatchOrderPtr                                              getOrder( basar::db::aspect::AccessorPropertyTable_YIterator );

    private:
        // forbidden
        BatchOrderCollectionDM( const BatchOrderCollectionDM& );
        BatchOrderCollectionDM& operator = ( const BatchOrderCollectionDM& );

        libutil::infrastructure::accessor::IAccessorPtr             getAccessor() const;
        libutil::domMod::SearchYIteratorPtr                         getSearchYIterator();
        componentManager::IBatchOrderConnectedFactoryPtr            getBatchOrderConnectedFactory();
                    
        const log4cplus::Logger&                                    getLogger() const;

    private:
        const log4cplus::Logger&                                    m_Logger;
        libutil::domMod::SearchYIteratorPtr                         m_SearchYIterator;
        libutil::infrastructure::accessor::IAccessorPtr             m_Accessor;
        componentManager::IBatchOrderConnectedFactoryPtr            m_BatchOrderConnectedFactory;
    };

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERCOLLECTIONDM_H
