#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERSPLITDMPTR_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERSPLITDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace transferOrder
{
    class ITransferOrderSplitDM;
    typedef boost::shared_ptr< ITransferOrderSplitDM > ITransferOrderSplitDMPtr;
	
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERSPLITDMPTR_H
