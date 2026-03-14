#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERHEADFINDERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERHEADFINDERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod
{
namespace batchOrder
{

    class IBatchOrderHeadFinder;
    typedef boost::shared_ptr< IBatchOrderHeadFinder > IBatchOrderHeadFinderPtr;

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERHEADFINDERPTR_H
