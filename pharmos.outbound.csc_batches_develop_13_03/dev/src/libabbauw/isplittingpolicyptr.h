#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_ISPLITTINGPOLICYPTR_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_ISPLITTINGPOLICYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace transferOrder
{
namespace splittingPolicy
{
    class ISplittingPolicy;
    typedef boost::shared_ptr< ISplittingPolicy > ISplittingPolicyPtr;
	
} // end namespace splittingPolicy
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_ISPLITTINGPOLICYPTR_H
