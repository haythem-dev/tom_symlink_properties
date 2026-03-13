#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod
{
namespace batchOrder
{

    class IBatchOrder;
    typedef boost::shared_ptr< IBatchOrder > IBatchOrderPtr;

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERPTR_H
