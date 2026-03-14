#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSCOLLECTIONPTR_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSCOLLECTIONPTR_H

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
namespace transferOrder
{
    class TransferOrderPosCollection;
    typedef boost::shared_ptr< TransferOrderPosCollection > TransferOrderPosCollectionPtr;    

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSCOLLECTIONPTR_H
