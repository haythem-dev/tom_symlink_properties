#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONFINDERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONFINDERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace transferOrder
{
    class ITransferOrderHeadCollectionFinder;
    typedef boost::shared_ptr< ITransferOrderHeadCollectionFinder > ITransferOrderHeadCollectionFinderPtr;
	
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONFINDERPTR_H
