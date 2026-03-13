#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERCOLLECTIONFINDERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERCOLLECTIONFINDERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod
{
namespace batchOrder
{

    class IBatchOrderCollectionFinder;
    typedef boost::shared_ptr< IBatchOrderCollectionFinder > IBatchOrderCollectionFinderPtr;

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERCOLLECTIONFINDERPTR_H
