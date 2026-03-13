#ifndef GUARD_LIBABBAUW_DOMMOD_CSCORDER_CSCORDERDMPTR_H
#define GUARD_LIBABBAUW_DOMMOD_CSCORDER_CSCORDERDMPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//

namespace libabbauw
{
namespace domMod
{
namespace cscorder
{
    class CSCOrderDM;
    typedef boost::shared_ptr< CSCOrderDM > CSCOrderDMPtr;

} // end namespace cscorder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CSCORDER_CSCORDERDMPTR_H
