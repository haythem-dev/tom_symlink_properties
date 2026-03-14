#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERGETTERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace transferOrder
{
    class ITransferOrderGetter;
    typedef boost::shared_ptr< ITransferOrderGetter > ITransferOrderGetterPtr;
	
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERGETTERPTR_H
