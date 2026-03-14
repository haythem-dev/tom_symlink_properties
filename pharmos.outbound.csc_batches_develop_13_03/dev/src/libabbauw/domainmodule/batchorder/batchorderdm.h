#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERDM_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERDM_H

#include "ibatchorder.h"

#include "ibatchorderheadfinderptr.h"
#include "ibatchorderpositioncollectionfinderptr.h"
#include "icscorderreleaserptr.h"

#include <libutil/classinfo.h>
#include <libutil/util.h>

namespace libabbauw
{
namespace domMod
{
namespace batchOrder
{

    class BatchOrderDM : public IBatchOrder
    {
    public:
        static const libutil::misc::ClassInfo& getClassInfo();

    public:
        BatchOrderDM();
        ~BatchOrderDM();

        void                                                        injectBatchOrderHeadFinder( IBatchOrderHeadFinderPtr );
        void                                                        injectBatchOrderPositionCollectionFinder( IBatchOrderPositionCollectionFinderPtr );
        void                                                        injectCscOrderReleaser( cscorder::ICSCOrderReleaserPtr );

        // from IBatchOrder interface
        basar::db::aspect::AccessorPropertyTable_YIterator          getHead();
        basar::db::aspect::AccessorPropertyTableRef                 getPositionCollection();

        bool                                                        isEmpty() const;

        void                                                        resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();

        void                                                        findByKey( basar::db::aspect::AccessorPropertyTable_YIterator );
        
        void                                                        release();
        
        basar::Decimal                                              getOrderValue() const;

    private:
        // forbidden
        BatchOrderDM( const BatchOrderDM& );
        BatchOrderDM& operator = ( const BatchOrderDM& );
        
        IBatchOrderHeadFinderPtr                                    getBatchOrderHeadFinder();
        IBatchOrderPositionCollectionFinderPtr                      getBatchOrderPositionCollectionFinder();
        cscorder::ICSCOrderReleaserPtr                              getCSCOrderReleaser();

        const log4cplus::Logger&                                    getLogger() const;

    private:
        const log4cplus::Logger&                                    m_Logger;

        IBatchOrderHeadFinderPtr                                    m_BatchOrderHeadFinder;
        IBatchOrderPositionCollectionFinderPtr                      m_BatchOrderPositionCollectionFinder;
        cscorder::ICSCOrderReleaserPtr                              m_CscOrderReleaser;
    };

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERDM_H
